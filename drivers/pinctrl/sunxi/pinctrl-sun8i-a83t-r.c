// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <dm/pinctrl.h>
#include <asm/gpio.h>

#include "pinctrl-sunxi.h"

static const struct sunxi_pinctrl_function sun8i_a83t_r_pinctrl_functions[] = {
	{ "gpio_in",	0 },
	{ "gpio_out",	1 },
	{ "s_uart",	2 },	/* PL2-PL3 */
};

static const struct sunxi_pinctrl_desc sun8i_a83t_r_pinctrl_desc = {
	.functions	= sun8i_a83t_r_pinctrl_functions,
	.num_functions	= ARRAY_SIZE(sun8i_a83t_r_pinctrl_functions),
	.first_bank	= SUNXI_GPIO_L,
	.num_banks	= 1,
};

static const struct udevice_id sun8i_a83t_r_pinctrl_ids[] = {
	{
		.compatible = "allwinner,sun8i-a83t-r-pinctrl",
		.data = (ulong)&sun8i_a83t_r_pinctrl_desc,
	},
	{}
};

U_BOOT_DRIVER(sun8i_a83t_r_pinctrl) = {
	.name		= "sun8i-a83t-r-pinctrl",
	.id		= UCLASS_PINCTRL,
	.of_match	= sun8i_a83t_r_pinctrl_ids,
	.bind		= sunxi_pinctrl_bind,
	.plat_auto	= sizeof(struct sunxi_pinctrl_plat),
	.ops		= &sunxi_pinctrl_ops,
};
