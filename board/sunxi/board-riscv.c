#include <spl.h>

int board_init(void)
{
	return 0;
}

uint32_t spl_boot_device(void)
{
	return BOOT_DEVICE_MMC1;
}
