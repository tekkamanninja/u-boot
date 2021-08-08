// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2018 Arm Ltd.
 * (C) Copyright 2020-2021 Samuel Holland <samuel@sholland.org>
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <image.h>
#include <sunxi_image.h>

#include "imagetool.h"
#include "mkimage.h"

#define SECTOR_SIZE			512

struct item_desc {
	const char	*name;
	const char	*file;
	unsigned long	addr;
	long		length;
};

static uint32_t toc1_header_length(uint32_t num_items)
{
	return ALIGN(sizeof(struct toc1_main_info) +
		     sizeof(struct toc1_item_info) * num_items, SECTOR_SIZE);
}

static int toc1_parse_cfg(const char *file, struct item_desc **desc,
			  uint32_t *main_length, uint32_t *num_items)
{
	struct item_desc *descs = NULL;
	int ret = EXIT_FAILURE;
	FILE *cfg, *fp = NULL;
	uint32_t ndescs = 0;
	char *line = NULL;
	size_t len = 0;

	*desc = NULL;
	*main_length = 0;
	*num_items = 0;

	cfg = fopen(file, "r");
	if (!cfg)
		return ret;

	while (getline(&line, &len, cfg) > 0) {
		char *end, *s;

		if (line[0] == '[') {
			s = line + 1;
			end = strchr(s, ']');
			if (!end || end[1] != '\n')
				goto err;
			end[0] = '\0';

			ndescs++;
			descs = reallocarray(descs, ndescs, sizeof(*descs));
			if (!descs)
				goto err;

			descs[ndescs - 1].name = strdup(s);
		} else if (line[0] != '#' && line[0] != '\n') {
			s = strchr(line, '=');
			if (!s)
				goto err;
			while ((++s)[0] == ' ')
				;
			end = strchr(s, '\n');
			if (!end)
				goto err;
			end[0] = '\0';

			if (!strncmp(line, "file", strlen("file"))) {
				fp = fopen(s, "rb");
				if (!fp)
					goto err;
				if (fseek(fp, 0, SEEK_END) < 0)
					goto err;
				descs[ndescs - 1].file = strdup(s);
				descs[ndescs - 1].length = ftell(fp);
				*main_length += ALIGN(descs[ndescs - 1].length,
						      SECTOR_SIZE);
				fclose(fp);
				fp = NULL;
			} else if (!strncmp(line, "addr", strlen("addr"))) {
				descs[ndescs - 1].addr = strtoul(s, NULL, 0);
			} else {
				goto err;
			}
		}
	}

	*desc = descs;
	*main_length += toc1_header_length(ndescs);
	*num_items = ndescs;

	ret = EXIT_SUCCESS;

err:
	if (fp)
		fclose(fp);
	if (ret)
		free(descs);
	free(line);
	fclose(cfg);

	return ret;
}

static int toc1_create(uint8_t *buf, uint32_t len,
		       const struct item_desc *desc, uint32_t num_items)
{
	struct toc1_main_info *main = (void *)buf;
	struct toc1_item_info *item = (void *)(main + 1);
	uint32_t item_offset, item_length;
	uint32_t *buf32 = (void *)buf;
	int ret = EXIT_FAILURE;
	uint32_t checksum = 0;
	FILE *fp = NULL;
	int i;

	/* Create the main TOC1 header. */
	main->magic	= cpu_to_le32(TOC0_MAIN_INFO_MAGIC);
	main->checksum	= cpu_to_le32(BROM_STAMP_VALUE);
	main->num_items	= cpu_to_le32(num_items);
	memcpy(main->end, TOC0_MAIN_INFO_END, sizeof(main->end));

	item_offset = 0;
	item_length = toc1_header_length(num_items);

	for (i = 0; i < num_items; ++i, ++item, ++desc) {
		item_offset = item_offset + item_length;
		item_length = desc->length;

		/* Create the item header. */
		memcpy(item->name, desc->name,
		       strnlen(desc->name, sizeof(item->name)));
		item->offset = cpu_to_le32(item_offset);
		item->length = cpu_to_le32(item_length);
		item->load_addr = cpu_to_le32(desc->addr);
		memcpy(item->end, TOC0_ITEM_INFO_END, sizeof(item->end));

		/* Read in the data. */
		fp = fopen(desc->file, "rb");
		if (!fp)
			goto err;
		if (!fread(buf + item_offset, item_length, 1, fp))
			goto err;
		fclose(fp);
		fp = NULL;

		/* Pad the sectors with 0xff to be flash-friendly. */
		item_offset = item_offset + item_length;
		item_length = ALIGN(item_offset, SECTOR_SIZE) - item_offset;
		memset(buf + item_offset, 0xff, item_length);
	}

	/* Fill in the total padded file length. */
	item_offset = item_offset + item_length;
	main->length = cpu_to_le32(item_offset);

	/* Verify enough space was provided when creating the image. */
	assert(len >= item_offset);

	/* Calculate the checksum. Yes, it's that simple. */
	for (i = 0; i < item_offset / 4; ++i)
		checksum += le32_to_cpu(buf32[i]);
	main->checksum = cpu_to_le32(checksum);

	ret = EXIT_SUCCESS;

err:
	if (fp)
		fclose(fp);

	return ret;
}

static int toc1_verify(const uint8_t *buf, uint32_t len)
{
	const struct toc1_main_info *main = (void *)buf;
	const struct toc1_item_info *item = (void *)(main + 1);
	uint32_t checksum = BROM_STAMP_VALUE;
	uint32_t main_length, num_items;
	uint32_t *buf32 = (void *)buf;
	int ret = EXIT_FAILURE;
	int i;

	num_items   = le32_to_cpu(main->num_items);
	main_length = le32_to_cpu(main->length);

	if (len < main_length || main_length < toc1_header_length(num_items))
		goto err;

	/* Verify the main header. */
	if (le32_to_cpu(main->magic) != TOC0_MAIN_INFO_MAGIC)
		goto err;
	/* Verify the checksum without modifying the buffer. */
	for (i = 0; i < main_length / 4; ++i)
		checksum += le32_to_cpu(buf32[i]);
	if (checksum != 2 * le32_to_cpu(main->checksum))
		goto err;
	/* The length must be at least 512 byte aligned. */
	if (main_length % SECTOR_SIZE)
		goto err;
	if (memcmp(main->end, TOC0_MAIN_INFO_END, sizeof(main->end)))
		goto err;

	/* Verify each item header. */
	for (i = 0; i < num_items; ++i, ++item)
		if (memcmp(item->end, TOC0_ITEM_INFO_END, sizeof(item->end)))
			goto err;

	ret = EXIT_SUCCESS;

err:
	return ret;
}

static int toc1_check_params(struct image_tool_params *params)
{
	if (!params->dflag)
		return -EINVAL;

	return 0;
}

static int toc1_verify_header(unsigned char *buf, int image_size,
			      struct image_tool_params *params)
{
	return toc1_verify(buf, image_size);
}

static void toc1_print_header(const void *buf)
{
	const struct toc1_main_info *main = buf;
	const struct toc1_item_info *item = (void *)(main + 1);
	uint32_t head_length, main_length, num_items;
	uint32_t item_offset, item_length, item_addr;
	int i;

	num_items   = le32_to_cpu(main->num_items);
	head_length = sizeof(*main) + num_items * sizeof(*item);
	main_length = le32_to_cpu(main->length);

	printf("Allwinner TOC1 Image\n"
	       "Size: %d bytes\n"
	       "Contents: %d items\n"
	       " 00000000:%08x Headers\n",
	       main_length, num_items, head_length);

	for (i = 0; i < num_items; ++i, ++item) {
		item_offset = le32_to_cpu(item->offset);
		item_length = le32_to_cpu(item->length);
		item_addr   = le32_to_cpu(item->load_addr);

		printf(" %08x:%08x => %08x %s\n",
		       item_offset, item_length, item_addr, item->name);
	}
}

static void toc1_set_header(void *buf, struct stat *sbuf, int ifd,
			    struct image_tool_params *params)
{
	/* Image is already written below. */
}

static int toc1_check_image_type(uint8_t type)
{
	return type == IH_TYPE_SUNXI_TOC1 ? 0 : 1;
}

static int toc1_vrec_header(struct image_tool_params *params,
			    struct image_type_params *tparams)
{
	uint32_t main_length, num_items;
	struct item_desc *desc;
	int ret;

	/* This "header" contains the entire image. */
	params->skipcpy = 1;

	ret = toc1_parse_cfg(params->datafile, &desc, &main_length, &num_items);
	if (ret)
		exit(ret);

	tparams->header_size = main_length;
	tparams->hdr = calloc(tparams->header_size, 1);
	if (!tparams->hdr)
		exit(ret);

	ret = toc1_create(tparams->hdr, tparams->header_size, desc, num_items);
	if (ret)
		exit(ret);

	return 0;
}

U_BOOT_IMAGE_TYPE(
	sunxi_toc1,
	"Allwinner TOC1 Boot Image support",
	0,
	NULL,
	toc1_check_params,
	toc1_verify_header,
	toc1_print_header,
	toc1_set_header,
	NULL,
	toc1_check_image_type,
	NULL,
	toc1_vrec_header
);
