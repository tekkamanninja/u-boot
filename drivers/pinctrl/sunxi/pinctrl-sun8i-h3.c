// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <dm/pinctrl.h>
#include <asm/gpio.h>

#include "pinctrl-sunxi.h"

static const struct sunxi_pinctrl_desc sun8i_h3_pinctrl_desc = {
	.first_bank	= SUNXI_GPIO_A,
	.num_banks	= 7,
};

static const struct udevice_id sun8i_h3_pinctrl_ids[] = {
	{
		.compatible = "allwinner,sun8i-h3-pinctrl",
		.data = (ulong)&sun8i_h3_pinctrl_desc,
	},
	{}
};

U_BOOT_DRIVER(sun8i_h3_pinctrl) = {
	.name		= "sun8i-h3-pinctrl",
	.id		= UCLASS_PINCTRL,
	.of_match	= sun8i_h3_pinctrl_ids,
	.bind		= sunxi_pinctrl_bind,
	.plat_auto	= sizeof(struct sunxi_pinctrl_plat),
	.ops		= &sunxi_pinctrl_ops,
};
