/*
 * Copyright (C) 2021 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 * custom_theme.h
 *
 *  Created on: 25 Mar 2021
 *      Author: masters
 */

#ifndef CUSTOM_THEME_H_
#define CUSTOM_THEME_H_

#include <egt/theme.h>

using namespace egt;

/* black background theme with white text and cyan highlight */
class CustomTheme1 : public Theme
{
	using Theme::Theme;
public:

	/*
	 * override the palette with custom colors
	 */
	void update_palette()
	{
		palette().set(Palette::ColorId::button_bg,
					  Palette::GroupId::normal,
					  Palette::black);
		palette().set(Palette::ColorId::text,
					  Palette::GroupId::normal,
					  Palette::white);
		palette().set(Palette::ColorId::text_highlight,
					  Palette::GroupId::normal,
					  Color(0x37949eff));
		palette().set(Palette::ColorId::button_fg,
					  Palette::GroupId::normal,
					  Color(0x37949eff));

		palette().set(Palette::ColorId::button_bg,
					  Palette::GroupId::active,
					  Palette::black);
		palette().set(Palette::ColorId::text,
					  Palette::GroupId::active,
					  Palette::white);
		palette().set(Palette::ColorId::text_highlight,
					  Palette::GroupId::active,
					  Color(0x37949eff));
		palette().set(Palette::ColorId::button_fg,
					  Palette::GroupId::active,
					  Color(0x37949eff));
	}

	/*
	 * perform default init of the palette
	 */
	void init_palette() override
	{
		Theme::init_palette();

		update_palette();
	}

	/*
	 * Copy the global palette and then override colors
	 */
	void duplicate_global()
	{
		this->palette(global_theme().palette());

		update_palette();
	}

};

class CustomTheme2 : public Theme
{
	using Theme::Theme;
public:

	/*
	 * override the palette with custom colors
	 */
	void update_palette()
	{
		palette().set(Palette::ColorId::button_bg,
					  Palette::GroupId::normal,
					  Color(0xab7e6dff));
		palette().set(Palette::ColorId::text,
					  Palette::GroupId::normal,
					  Palette::white);
		palette().set(Palette::ColorId::text_highlight,
					  Palette::GroupId::normal,
					  Color(0x825639ff));
		palette().set(Palette::ColorId::button_fg,
					  Palette::GroupId::normal,
					  Color(0x825639ff));

		palette().set(Palette::ColorId::button_bg,
					  Palette::GroupId::active,
					  Color(0xab7e6dff));
		palette().set(Palette::ColorId::text,
					  Palette::GroupId::active,
					  Palette::white);
		palette().set(Palette::ColorId::text_highlight,
					  Palette::GroupId::active,
					  Color(0x825639ff));
		palette().set(Palette::ColorId::button_fg,
					  Palette::GroupId::active,
					  Color(0x825639ff));
	}

	/*
	 * perform default init of the palette
	 */
	void init_palette() override
	{
		Theme::init_palette();

		update_palette();
	}

	/*
	 * Copy the global palette and then override colors
	 */
	void duplicate_global()
	{
		this->palette(global_theme().palette());

		update_palette();
	}

};

#endif /* CUSTOM_THEME_H_ */
