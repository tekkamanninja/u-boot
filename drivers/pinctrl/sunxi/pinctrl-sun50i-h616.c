// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <dm/pinctrl.h>
#include <asm/gpio.h>

#include "pinctrl-sunxi.h"

static const struct sunxi_pinctrl_desc sun50i_h616_pinctrl_desc = {
	.first_bank	= SUNXI_GPIO_A,
	.num_banks	= 9,
};

static const struct udevice_id sun50i_h616_pinctrl_ids[] = {
	{
		.compatible = "allwinner,sun50i-h616-pinctrl",
		.data = (ulong)&sun50i_h616_pinctrl_desc,
	},
	{}
};

U_BOOT_DRIVER(sun50i_h616_pinctrl) = {
	.name		= "sun50i-h616-pinctrl",
	.id		= UCLASS_PINCTRL,
	.of_match	= sun50i_h616_pinctrl_ids,
	.bind		= sunxi_pinctrl_bind,
	.plat_auto	= sizeof(struct sunxi_pinctrl_plat),
	.ops		= &sunxi_pinctrl_ops,
};
