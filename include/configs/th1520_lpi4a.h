/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2023 Yixun Lan <dlan@gentoo.org>
 *
 */

#ifndef __TH1520_LPI4A_H
#define __TH1520_LPI4A_H

#include <linux/sizes.h>

#define CFG_SYS_SDRAM_BASE		0x00000000

#define UART_BASE	0xffe7014000
#define UART_REG_WIDTH  32

/* Environment options */

#define CFG_EXTRA_ENV_SETTINGS \
	"kernel_addr_r=0x82000000\0" \
	"kernel_comp_addr_r=0x88000000\0" \
	"kernel_comp_size=0x4000000\0" \
	"ramdisk_addr_r=0x8d300000\0" \
	"fdt_addr_r=0x8e000000\0" \
	"fdt_addr_fixed=0x20000000\0" \
	"scriptaddr=0x8e400000\0" \
	"scriptfile=uEnv_light_lpi4a.txt\0" \
	"devtype=mmc\0" \
	"devnum=0\0" \
	"bootpart=2\0" \
	"bootcmd=ext4load mmc ${devnum}:${bootpart} ${scriptaddr} ${scriptfile}; env import -t ${scriptaddr} ${filesize};boot\0" \
	"PS1=[LPi4A]# \0"

#endif /* __TH1520_LPI4A_H */
