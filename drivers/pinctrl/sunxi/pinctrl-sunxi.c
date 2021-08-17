// SPDX-License-Identifier: GPL-2.0

#include <dm.h>
#include <dm/device-internal.h>
#include <dm/lists.h>
#include <dm/pinctrl.h>
#include <errno.h>
#include <malloc.h>
#include <asm/gpio.h>

#include "pinctrl-sunxi.h"

extern U_BOOT_DRIVER(gpio_sunxi);

static int sunxi_pinctrl_get_pins_count(struct udevice *dev)
{
	const struct sunxi_pinctrl_desc *desc = dev_get_priv(dev);

	return desc->num_banks * SUNXI_GPIOS_PER_BANK;
}

static const char *sunxi_pinctrl_get_pin_name(struct udevice *dev,
					      uint pin_selector)
{
	const struct sunxi_pinctrl_desc *desc = dev_get_priv(dev);
	static char pin_name[sizeof("PN31")];

	snprintf(pin_name, sizeof(pin_name), "P%c%d",
		 pin_selector / SUNXI_GPIOS_PER_BANK + desc->first_bank + 'A',
		 pin_selector % SUNXI_GPIOS_PER_BANK);

	return pin_name;
}

static int sunxi_pinctrl_get_functions_count(struct udevice *dev)
{
	const struct sunxi_pinctrl_desc *desc = dev_get_priv(dev);

	return desc->num_functions;
}

static const char *sunxi_pinctrl_get_function_name(struct udevice *dev,
						   uint func_selector)
{
	const struct sunxi_pinctrl_desc *desc = dev_get_priv(dev);

	return desc->functions[func_selector].name;
}

static int sunxi_pinctrl_pinmux_set(struct udevice *dev, uint pin_selector,
				    uint func_selector)
{
	const struct sunxi_pinctrl_desc *desc = dev_get_priv(dev);
	struct sunxi_pinctrl_plat *plat = dev_get_plat(dev);
	int bank = pin_selector / SUNXI_GPIOS_PER_BANK;
	int pin	 = pin_selector % SUNXI_GPIOS_PER_BANK;

	debug("set mux: %-4s => %s (%d)\n",
	      sunxi_pinctrl_get_pin_name(dev, pin_selector),
	      sunxi_pinctrl_get_function_name(dev, func_selector),
	      desc->functions[func_selector].mux);

	sunxi_gpio_set_cfgbank(plat->base + bank, pin,
			       desc->functions[func_selector].mux);

	return 0;
}

const struct pinctrl_ops sunxi_pinctrl_ops = {
	.get_pins_count		= sunxi_pinctrl_get_pins_count,
	.get_pin_name		= sunxi_pinctrl_get_pin_name,
	.get_functions_count	= sunxi_pinctrl_get_functions_count,
	.get_function_name	= sunxi_pinctrl_get_function_name,
	.pinmux_set		= sunxi_pinctrl_pinmux_set,
	.set_state		= pinctrl_generic_set_state,
};

int sunxi_pinctrl_bind(struct udevice *dev)
{
	struct sunxi_pinctrl_plat *plat = dev_get_plat(dev);
	struct sunxi_pinctrl_desc *desc;
	struct sunxi_gpio_plat *gpio_plat;
	struct udevice *gpio_dev;
	int i, ret;

	desc = (void *)dev_get_driver_data(dev);
	if (!desc)
		return -EINVAL;
	dev_set_priv(dev, desc);

	plat->base = dev_read_addr_ptr(dev);

	ret = device_bind_driver_to_node(dev, "gpio_sunxi", dev->name,
					 dev_ofnode(dev), &gpio_dev);
	if (ret)
		return ret;

	for (i = 0; i < desc->num_banks; ++i) {
		gpio_plat = malloc(sizeof(*gpio_plat));
		if (!gpio_plat)
			return -ENOMEM;

		gpio_plat->regs = plat->base + i;
		gpio_plat->bank_name[0] = 'P';
		gpio_plat->bank_name[1] = 'A' + desc->first_bank + i;
		gpio_plat->bank_name[2] = '\0';

		ret = device_bind(gpio_dev, DM_DRIVER_REF(gpio_sunxi),
				  gpio_plat->bank_name, gpio_plat,
				  ofnode_null(), NULL);
		if (ret)
			return ret;
	}

	return 0;
}
