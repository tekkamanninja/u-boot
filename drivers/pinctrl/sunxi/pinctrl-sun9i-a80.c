// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <dm/pinctrl.h>
#include <asm/gpio.h>

#include "pinctrl-sunxi.h"

static const struct sunxi_pinctrl_function sun9i_a80_pinctrl_functions[] = {
	{ "gpio_in",	0 },
	{ "gpio_out",	1 },
};

static const struct sunxi_pinctrl_desc sun9i_a80_pinctrl_desc = {
	.functions	= sun9i_a80_pinctrl_functions,
	.num_functions	= ARRAY_SIZE(sun9i_a80_pinctrl_functions),
	.first_bank	= SUNXI_GPIO_A,
	.num_banks	= 8,
};

static const struct udevice_id sun9i_a80_pinctrl_ids[] = {
	{
		.compatible = "allwinner,sun9i-a80-pinctrl",
		.data = (ulong)&sun9i_a80_pinctrl_desc,
	},
	{}
};

U_BOOT_DRIVER(sun9i_a80_pinctrl) = {
	.name		= "sun9i-a80-pinctrl",
	.id		= UCLASS_PINCTRL,
	.of_match	= sun9i_a80_pinctrl_ids,
	.bind		= sunxi_pinctrl_bind,
	.plat_auto	= sizeof(struct sunxi_pinctrl_plat),
	.ops		= &sunxi_pinctrl_ops,
};
