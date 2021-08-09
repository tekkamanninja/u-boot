// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2017 Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#include <common.h>
#include <dm.h>
#include <sysreset.h>
#include <linux/errno.h>
#include <asm/sbi.h>

static int sbi_sysreset_request(struct udevice *dev, enum sysreset_t type)
{
	unsigned long reason = SBI_SRST_RESET_REASON_NONE;

	switch (type) {
	case SYSRESET_WARM:
		sbi_srst_reset(SBI_SRST_RESET_TYPE_WARM_REBOOT, reason);
		break;
	case SYSRESET_COLD:
		sbi_srst_reset(SBI_SRST_RESET_TYPE_COLD_REBOOT, reason);
		break;
	case SYSRESET_POWER_OFF:
		sbi_srst_reset(SBI_SRST_RESET_TYPE_SHUTDOWN, reason);
		break;
	default:
		return -ENOSYS;
	}

	return -EINPROGRESS;
}

static struct sysreset_ops sbi_sysreset_ops = {
	.request = sbi_sysreset_request,
};

U_BOOT_DRIVER(sbi_sysreset) = {
	.name = "sbi-sysreset",
	.id = UCLASS_SYSRESET,
	.ops = &sbi_sysreset_ops,
};
