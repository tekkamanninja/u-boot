// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <dm/pinctrl.h>
#include <asm/gpio.h>

#include "pinctrl-sunxi.h"

static const struct sunxi_pinctrl_function sun50i_a64_r_pinctrl_functions[] = {
	{ "gpio_in",	0 },
	{ "gpio_out",	1 },
};

static const struct sunxi_pinctrl_desc sun50i_a64_r_pinctrl_desc = {
	.functions	= sun50i_a64_r_pinctrl_functions,
	.num_functions	= ARRAY_SIZE(sun50i_a64_r_pinctrl_functions),
	.first_bank	= SUNXI_GPIO_L,
	.num_banks	= 1,
};

static const struct udevice_id sun50i_a64_r_pinctrl_ids[] = {
	{
		.compatible = "allwinner,sun50i-a64-r-pinctrl",
		.data = (ulong)&sun50i_a64_r_pinctrl_desc,
	},
	{}
};

U_BOOT_DRIVER(sun50i_a64_r_pinctrl) = {
	.name		= "sun50i-a64-r-pinctrl",
	.id		= UCLASS_PINCTRL,
	.of_match	= sun50i_a64_r_pinctrl_ids,
	.bind		= sunxi_pinctrl_bind,
	.plat_auto	= sizeof(struct sunxi_pinctrl_plat),
	.ops		= &sunxi_pinctrl_ops,
};
