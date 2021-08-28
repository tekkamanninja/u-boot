// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <dm/pinctrl.h>
#include <asm/gpio.h>

#include "pinctrl-sunxi.h"

static const struct sunxi_pinctrl_function sun7i_a20_pinctrl_functions[] = {
	{ "emac",	2 },	/* PA0-PA17 */
	{ "gmac",	5 },	/* PA0-PA17 */
	{ "gpio_in",	0 },
	{ "gpio_out",	1 },
	{ "i2c0",	2 },	/* PB0-PB1 */
	{ "i2c1",	2 },	/* PB18-PB19 */
	{ "mmc0",	2 },	/* PF0-PF5 */
#if IS_ENABLED(CONFIG_MMC1_PINS_PH)
	{ "mmc1",	5 },	/* PH22-PH27 */
#else
	{ "mmc1",	4 },	/* PG0-PG5 */
#endif
	{ "mmc2",	3 },	/* PC5-PC15, PC24 */
#if IS_ENABLED(CONFIG_UART0_PORT_F)
	{ "uart0",	4 },	/* PF2-PF4 */
#else
	{ "uart0",	2 },	/* PB22-PB23 */
#endif
};

static const struct sunxi_pinctrl_desc sun7i_a20_pinctrl_desc = {
	.functions	= sun7i_a20_pinctrl_functions,
	.num_functions	= ARRAY_SIZE(sun7i_a20_pinctrl_functions),
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
