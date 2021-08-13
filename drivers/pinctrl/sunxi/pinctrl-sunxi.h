/* SPDX-License-Identifier: GPL-2.0 */

struct sunxi_pinctrl_desc {
	u8					first_bank;
	u8					num_banks;
};

struct sunxi_pinctrl_plat {
	struct sunxi_gpio __iomem *base;
};

extern const struct pinctrl_ops sunxi_pinctrl_ops;

int sunxi_pinctrl_bind(struct udevice *dev);
