// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <dm/pinctrl.h>
#include <asm/gpio.h>

#include "pinctrl-sunxi.h"

static const struct sunxi_pinctrl_function sun6i_a31_r_pinctrl_functions[] = {
	{ "gpio_in",	0 },
	{ "gpio_out",	1 },
	{ "s_i2c",	2 },	/* PL0-PL1 */
	{ "s_uart",	2 },	/* PL2-PL3 */
};

static const struct sunxi_pinctrl_desc sun6i_a31_r_pinctrl_desc = {
	.functions	= sun6i_a31_r_pinctrl_functions,
	.num_functions	= ARRAY_SIZE(sun6i_a31_r_pinctrl_functions),
	.first_bank	= SUNXI_GPIO_L,
	.num_banks	= 2,
};

static const struct udevice_id sun6i_a31_r_pinctrl_ids[] = {
	{
		.compatible = "allwinner,sun6i-a31-r-pinctrl",
		.data = (ulong)&sun6i_a31_r_pinctrl_desc,
	},
	{}
};

U_BOOT_DRIVER(sun6i_a31_r_pinctrl) = {
	.name		= "sun6i-a31-r-pinctrl",
	.id		= UCLASS_PINCTRL,
	.of_match	= sun6i_a31_r_pinctrl_ids,
	.bind		= sunxi_pinctrl_bind,
	.plat_auto	= sizeof(struct sunxi_pinctrl_plat),
	.ops		= &sunxi_pinctrl_ops,
};
