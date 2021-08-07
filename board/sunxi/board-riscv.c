#include <common.h>
#include <cpu.h>
#include <spl.h>

int board_init(void)
{
	/* https://lore.kernel.org/u-boot/31587574-4cd1-02da-9761-0134ac82b94b@sholland.org/ */
	return cpu_probe_all();
}

uint32_t spl_boot_device(void)
{
	return BOOT_DEVICE_MMC1;
}
