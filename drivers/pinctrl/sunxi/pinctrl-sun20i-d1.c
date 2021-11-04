// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <dm/pinctrl.h>
#include <asm/gpio.h>
#include "../../../arch/arm/include/asm/arch-sunxi/gpio.h"

#include "pinctrl-sunxi.h"

static const struct sunxi_pinctrl_function sun20i_d1_pinctrl_functions[] = {
	{ "emac",	8 },	/* PE0-PE15 */
	{ "gpio_in",	0 },
	{ "gpio_out",	1 },
	{ "i2c0",	4 },	/* PB10-PB11 */
	{ "mmc0",	2 },	/* PF0-PF5 */
	{ "mmc1",	2 },	/* PG0-PG5 */
	{ "mmc2",	3 },	/* PC2-PC7 */
	{ "spi0",	2 },	/* PC2-PC7 */
#if IS_ENABLED(CONFIG_UART0_PORT_F)
	{ "uart0",	3 },	/* PF2-PF4 */
#else
	{ "uart0",	6 },	/* PB8-PB9 */
#endif
	{ "uart1",	2 },	/* PG6-PG7 */
	{ "uart2",	7 },	/* PB0-PB1 */
};

static const struct sunxi_pinctrl_desc sun20i_d1_pinctrl_desc = {
	.functions	= sun20i_d1_pinctrl_functions,
	.num_functions	= ARRAY_SIZE(sun20i_d1_pinctrl_functions),
	.first_bank	= SUNXI_GPIO_A,
	.num_banks	= 7,
};

static const struct udevice_id sun20i_d1_pinctrl_ids[] = {
	{
		.compatible = "allwinner,sun20i-d1-pinctrl",
		.data = (ulong)&sun20i_d1_pinctrl_desc,
	},
	{}
};

U_BOOT_DRIVER(sun20i_d1_pinctrl) = {
	.name		= "sun20i-d1-pinctrl",
	.id		= UCLASS_PINCTRL,
	.of_match	= sun20i_d1_pinctrl_ids,
	.bind		= sunxi_pinctrl_bind,
	.plat_auto	= sizeof(struct sunxi_pinctrl_plat),
	.ops		= &sunxi_pinctrl_ops,
};
