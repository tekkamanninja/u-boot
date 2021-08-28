// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <dm/pinctrl.h>
#include <asm/gpio.h>

#include "pinctrl-sunxi.h"

static const struct sunxi_pinctrl_function sun6i_a31_pinctrl_functions[] = {
	{ "gmac",	2 },	/* PA0-PA27 */
	{ "gpio_in",	0 },
	{ "gpio_out",	1 },
	{ "i2c0",	2 },	/* PH14-PH15 */
	{ "i2c1",	2 },	/* PH16-PH17 */
	{ "mmc0",	2 },	/* PF0-PF5 */
	{ "mmc1",	2 },	/* PG0-PG5 */
	{ "mmc2",	3 },	/* PC6-PC15, PC24 */
	{ "mmc3",	4 },	/* PC6-PC15, PC24 */
#if IS_ENABLED(CONFIG_UART0_PORT_F)
	{ "uart0",	3 },	/* PF2-PF4 */
#else
	{ "uart0",	2 },	/* PH20-PH21 */
#endif
};

static const struct sunxi_pinctrl_desc sun6i_a31_pinctrl_desc = {
	.functions	= sun6i_a31_pinctrl_functions,
	.num_functions	= ARRAY_SIZE(sun6i_a31_pinctrl_functions),
	.first_bank	= SUNXI_GPIO_A,
	.num_banks	= 8,
};

static const struct udevice_id sun6i_a31_pinctrl_ids[] = {
	{
		.compatible = "allwinner,sun6i-a31-pinctrl",
		.data = (ulong)&sun6i_a31_pinctrl_desc,
	},
	{
		.compatible = "allwinner,sun6i-a31s-pinctrl",
		.data = (ulong)&sun6i_a31_pinctrl_desc,
	},
	{}
};

U_BOOT_DRIVER(sun6i_a31_pinctrl) = {
	.name		= "sun6i-a31-pinctrl",
	.id		= UCLASS_PINCTRL,
	.of_match	= sun6i_a31_pinctrl_ids,
	.bind		= sunxi_pinctrl_bind,
	.plat_auto	= sizeof(struct sunxi_pinctrl_plat),
	.ops		= &sunxi_pinctrl_ops,
};
