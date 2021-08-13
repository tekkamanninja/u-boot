// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <dm/pinctrl.h>
#include <asm/gpio.h>

#include "pinctrl-sunxi.h"

static const struct sunxi_pinctrl_desc sun5i_a13_pinctrl_desc = {
	.first_bank	= SUNXI_GPIO_A,
	.num_banks	= 7,
};

static const struct udevice_id sun5i_a13_pinctrl_ids[] = {
	{
		.compatible = "allwinner,sun5i-a10s-pinctrl",
		.data = (ulong)&sun5i_a13_pinctrl_desc,
	},
	{
		.compatible = "allwinner,sun5i-a13-pinctrl",
		.data = (ulong)&sun5i_a13_pinctrl_desc,
	},
	{}
};

U_BOOT_DRIVER(sun5i_a13_pinctrl) = {
	.name		= "sun5i-a13-pinctrl",
	.id		= UCLASS_PINCTRL,
	.of_match	= sun5i_a13_pinctrl_ids,
	.bind		= sunxi_pinctrl_bind,
	.plat_auto	= sizeof(struct sunxi_pinctrl_plat),
	.ops		= &sunxi_pinctrl_ops,
};
