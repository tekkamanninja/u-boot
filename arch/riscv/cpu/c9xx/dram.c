/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <fdtdec.h>
#include <init.h>
#include <linux/sizes.h>

DECLARE_GLOBAL_DATA_PTR;

int dram_init(void)
{
#ifdef CONFIG_DDR_BOARD_CONFIG
extern unsigned long get_ddr_density(void);
	// update ram_size from board config info
	gd->ram_size = get_ddr_density();
	return 0;
#else
	return fdtdec_setup_mem_size_base();
#endif
}

int dram_init_banksize(void)
{
#ifdef CONFIG_DDR_BOARD_CONFIG
	for (int bank = 0; bank < CONFIG_NR_DRAM_BANKS; bank++) {

		gd->bd->bi_dram[bank].start = (phys_addr_t)CONFIG_SYS_SDRAM_BASE
									+ gd->ram_size/CONFIG_NR_DRAM_BANKS*bank;
		gd->bd->bi_dram[bank].size = gd->ram_size / CONFIG_NR_DRAM_BANKS;
	}
	return 0;
#else
	return fdtdec_setup_memory_banksize();
#endif
}

ulong board_get_usable_ram_top(ulong total_size)
{
#ifdef CONFIG_64BIT
	/*
	 * Ensure that we run from first 4GB so that all
	 * addresses used by U-Boot are 32bit addresses.
	 *
	 * This in-turn ensures that 32bit DMA capable
	 * devices work fine because DMA mapping APIs will
	 * provide 32bit DMA addresses only.
	 */
	if (gd->ram_top > SZ_4G)
		return SZ_4G;
#endif
	return gd->ram_top;
}
