// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <errno.h>
#include <remoteproc.h>
#include <asm/io.h>

#define SUNXI_SCP_MAGIC			0xb4400012

#define OR1K_VEC_FIRST			0x01
#define OR1K_VEC_LAST			0x0e
#define OR1K_VEC_ADDR(n)		(0x100 * (n))

struct sun6i_ar100_rproc_priv {
	void	*cfg_base;
	ulong	sram_base;
};

static int sun6i_ar100_rproc_load(struct udevice *dev, ulong addr, ulong size)
{
	struct sun6i_ar100_rproc_priv *priv = dev_get_priv(dev);

	/* Check for a valid SCP firmware. */
	if (readl_relaxed(addr) != SUNXI_SCP_MAGIC)
		return -ENOENT;

	/* Program exception vectors to the firmware entry point. */
	for (u32 i = OR1K_VEC_FIRST; i <= OR1K_VEC_LAST; ++i) {
		ulong vector = priv->sram_base + OR1K_VEC_ADDR(i);
		ulong offset = addr - vector;

		writel_relaxed(offset >> 2, vector);
	}

	return 0;
}

static int sun6i_ar100_rproc_start(struct udevice *dev)
{
	struct sun6i_ar100_rproc_priv *priv = dev_get_priv(dev);

	setbits_le32(priv->cfg_base, BIT(0));

	return 0;
}

static int sun6i_ar100_rproc_stop(struct udevice *dev)
{
	struct sun6i_ar100_rproc_priv *priv = dev_get_priv(dev);

	clrbits_le32(priv->cfg_base, BIT(0));

	return 0;
}

static int sun6i_ar100_rproc_reset(struct udevice *dev)
{
	int ret;

	ret = sun6i_ar100_rproc_stop(dev);
	if (ret)
		return ret;

	return sun6i_ar100_rproc_start(dev);
}

static int sun6i_ar100_rproc_is_running(struct udevice *dev)
{
	struct sun6i_ar100_rproc_priv *priv = dev_get_priv(dev);

	return !(readl_relaxed(priv->cfg_base) & BIT(0));
}

static const struct dm_rproc_ops sun6i_ar100_rproc_ops = {
	.load		= sun6i_ar100_rproc_load,
	.start		= sun6i_ar100_rproc_start,
	.stop		= sun6i_ar100_rproc_stop,
	.reset		= sun6i_ar100_rproc_reset,
	.is_running	= sun6i_ar100_rproc_is_running,
};

static int sun6i_ar100_rproc_probe(struct udevice *dev)
{
	struct sun6i_ar100_rproc_priv *priv = dev_get_priv(dev);
	struct ofnode_phandle_args sram_handle;
	int ret;

	priv->cfg_base = dev_read_addr_ptr(dev);

	ret = dev_read_phandle_with_args(dev, "sram", NULL, 0, 0, &sram_handle);
	if (ret)
		return ret;

	priv->sram_base = ofnode_get_addr(sram_handle.node);

	return 0;
}

static const struct udevice_id sun6i_ar100_rproc_ids[] = {
	{ .compatible = "allwinner,sun6i-a31-ar100" },
	{ }
};

U_BOOT_DRIVER(sun6i_ar100_rproc) = {
	.name		= "sun6i_ar100_rproc",
	.id		= UCLASS_REMOTEPROC,
	.of_match	= sun6i_ar100_rproc_ids,
	.probe		= sun6i_ar100_rproc_probe,
	.priv_auto	= sizeof(struct sun6i_ar100_rproc_priv),
	.ops		= &sun6i_ar100_rproc_ops,
};
