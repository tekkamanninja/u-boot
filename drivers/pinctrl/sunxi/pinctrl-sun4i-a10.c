// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <dm/pinctrl.h>
#include <asm/gpio.h>

#include "pinctrl-sunxi.h"

static const struct sunxi_pinctrl_desc sun4i_a10_pinctrl_desc = {
	.first_bank	= SUNXI_GPIO_A,
	.num_banks	= 9,
};

static const struct udevice_id sun4i_a10_pinctrl_ids[] = {
	{
		.compatible = "allwinner,sun4i-a10-pinctrl",
		.data = (ulong)&sun4i_a10_pinctrl_desc,
	},
	{}
};

U_BOOT_DRIVER(sun4i_a10_pinctrl) = {
	.name		= "sun4i-a10-pinctrl",
	.id		= UCLASS_PINCTRL,
	.of_match	= sun4i_a10_pinctrl_ids,
	.bind		= sunxi_pinctrl_bind,
	.plat_auto	= sizeof(struct sunxi_pinctrl_plat),
	.ops		= &sunxi_pinctrl_ops,
};
