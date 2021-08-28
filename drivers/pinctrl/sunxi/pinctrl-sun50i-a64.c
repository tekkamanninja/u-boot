// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <dm/pinctrl.h>
#include <asm/gpio.h>

#include "pinctrl-sunxi.h"

static const struct sunxi_pinctrl_function sun50i_a64_pinctrl_functions[] = {
	{ "emac",	4 },	/* PD8-PD23 */
	{ "gpio_in",	0 },
	{ "gpio_out",	1 },
	{ "i2c0",	2 },	/* PH0-PH1 */
	{ "i2c1",	2 },	/* PH2-PH3 */
	{ "mmc0",	2 },	/* PF0-PF5 */
	{ "mmc1",	2 },	/* PG0-PG5 */
	{ "mmc2",	3 },	/* PC1-PC16 */
	{ "pwm",	2 },	/* PD22 */
#if IS_ENABLED(CONFIG_UART0_PORT_F)
	{ "uart0",	3 },	/* PF2-PF4 */
#else
	{ "uart0",	4 },	/* PB8-PB9 */
#endif
	{ "uart1",	2 },	/* PG6-PG7 */
	{ "uart2",	2 },	/* PB0-PB1 */
};

static const struct sunxi_pinctrl_desc sun50i_a64_pinctrl_desc = {
	.functions	= sun50i_a64_pinctrl_functions,
	.num_functions	= ARRAY_SIZE(sun50i_a64_pinctrl_functions),
	.first_bank	= SUNXI_GPIO_A,
	.num_banks	= 8,
};

static const struct udevice_id sun50i_a64_pinctrl_ids[] = {
	{
		.compatible = "allwinner,sun50i-a64-pinctrl",
		.data = (ulong)&sun50i_a64_pinctrl_desc,
	},
	{}
};

U_BOOT_DRIVER(sun50i_a64_pinctrl) = {
	.name		= "sun50i-a64-pinctrl",
	.id		= UCLASS_PINCTRL,
	.of_match	= sun50i_a64_pinctrl_ids,
	.bind		= sunxi_pinctrl_bind,
	.plat_auto	= sizeof(struct sunxi_pinctrl_plat),
	.ops		= &sunxi_pinctrl_ops,
};
