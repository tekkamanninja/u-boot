// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <dm/pinctrl.h>
#include <asm/gpio.h>

#include "pinctrl-sunxi.h"

static const struct sunxi_pinctrl_function sun8i_a23_pinctrl_functions[] = {
	{ "gpio_in",	0 },
	{ "gpio_out",	1 },
	{ "i2c0",	2 },	/* PH2-PH3 */
	{ "i2c1",	2 },	/* PH4-PH5 */
#if IS_ENABLED(CONFIG_UART0_PORT_F)
	{ "uart0",	3 },	/* PF2-PF4 */
#endif
	{ "uart1",	2 },	/* PG6-PG7 */
	{ "uart2",	2 },	/* PB0-PB1 */
};

static const struct sunxi_pinctrl_desc sun8i_a23_pinctrl_desc = {
	.functions	= sun8i_a23_pinctrl_functions,
	.num_functions	= ARRAY_SIZE(sun8i_a23_pinctrl_functions),
	.first_bank	= SUNXI_GPIO_A,
	.num_banks	= 8,
};

static const struct udevice_id sun8i_a23_pinctrl_ids[] = {
	{
		.compatible = "allwinner,sun8i-a23-pinctrl",
		.data = (ulong)&sun8i_a23_pinctrl_desc,
	},
	{}
};

U_BOOT_DRIVER(sun8i_a23_pinctrl) = {
	.name		= "sun8i-a23-pinctrl",
	.id		= UCLASS_PINCTRL,
	.of_match	= sun8i_a23_pinctrl_ids,
	.bind		= sunxi_pinctrl_bind,
	.plat_auto	= sizeof(struct sunxi_pinctrl_plat),
	.ops		= &sunxi_pinctrl_ops,
};
