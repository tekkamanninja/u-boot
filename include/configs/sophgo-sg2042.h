/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2023 FIXME, Inc
 *
 */

#ifndef __SOPHGO_SG2042_H
#define __SOPHGO_SG2042_H

#include <linux/sizes.h>

#define CFG_SYS_SDRAM_BASE		0x80000000

#define UART_BASE	0x7040000000
#define UART_REG_WIDTH  32

/* Environment options */

#define BOOT_TARGET_DEVICES(func) "/dev/dummy"

#include <config_distro_bootcmd.h>

#define PARTS_DEFAULT \
	"name=loader1,start=17K,size=1M,type=${type_guid_gpt_loader1};" \
	"name=loader2,size=4MB,type=${type_guid_gpt_loader2};" \
	"name=system,size=-,bootable,type=${type_guid_gpt_system};"

#define CFG_EXTRA_ENV_SETTINGS \
	"kernel_addr_r=0x84000000\0" \
	"kernel_comp_addr_r=0x88000000\0" \
	"kernel_comp_size=0x4000000\0" \
	"fdt_addr_r=0x8c000000\0" \
	"scriptaddr=0x8c100000\0" \
	"pxefile_addr_r=0x8c200000\0" \
	"ramdisk_addr_r=0x8c300000\0" \
	"kernel_addr=0x80200000\0" \
	"ramdisk_addr=0x88000000\0" \
	"ramdisk_size=8000000\0" \
	"dtb_addr=0x8E000000\0" \
	"mmcdev=0\0" \
	"mmcpart=1\0" \
	"kernel_image=/riscv64/Image\0" \
	"ramdisk_image=/riscv64/initrd.img\0" \
	"dtb_image=/riscv64/mango.dtb\0" \
	"loadimage=echo Load kernel image..; fatload mmc ${mmcdev}:${mmcpart} ${kernel_addr} ${kernel_image}\0" \
	"loadramdisk=echo Load ramdisk image..; fatload mmc ${mmcdev}:${mmcpart} ${ramdisk_addr} ${ramdisk_image}\0" \
	"loaddtb=echo Load dtb image..; fatload mmc ${mmcdev}:${mmcpart} ${dtb_addr} ${dtb_image}\0" \
	"bootsd=echo Boot from sd..; booti ${kernel_addr} ${ramdisk_addr}:${ramdisk_size} ${dtb_addr}\0" \
	"bootcmd=run loadimage; run loadramdisk; run loaddtb; run bootsd\0" \
	BOOTENV

#endif /* __SOPHGO_SG2042_H */
