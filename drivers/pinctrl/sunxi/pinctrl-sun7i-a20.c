// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <dm/pinctrl.h>
#include <asm/gpio.h>

#include "pinctrl-sunxi.h"

static const struct sunxi_pinctrl_desc sun7i_a20_pinctrl_desc = {
	.first_bank	= SUNXI_GPIO_A,
	.num_banks	= 9,
};

static const struct udevice_id sun7i_a20_pinctrl_ids[] = {
	{
		.compatible = "allwinner,sun7i-a20-pinctrl",
		.data = (ulong)&sun7i_a20_pinctrl_desc,
	},
	{
		.compatible = "allwinner,sun8i-r40-pinctrl",
		.data = (ulong)&sun7i_a20_pinctrl_desc,
	},
	{}
};

U_BOOT_DRIVER(sun7i_a20_pinctrl) = {
	.name		= "sun7i-a20-pinctrl",
	.id		= UCLASS_PINCTRL,
	.of_match	= sun7i_a20_pinctrl_ids,
	.bind		= sunxi_pinctrl_bind,
	.plat_auto	= sizeof(struct sunxi_pinctrl_plat),
	.ops		= &sunxi_pinctrl_ops,
};
