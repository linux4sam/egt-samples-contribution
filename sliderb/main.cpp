/*
 * Copyright (C) 2021 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * main.cpp
 *
 *  Created on: 20 Mar 2021
 *      Author: masters
 */

#include <egt/themes/coconut.h>
#include <egt/themes/lapis.h>
#include <egt/themes/midnight.h>
#include <egt/themes/shamrock.h>
#include <egt/themes/sky.h>
#include <egt/themes/ultraviolet.h>
#include <egt/ui>

#include "sliderb.h"

#include <functional>
#include <iostream>

using namespace egt;

/* black background theme with white text and cyan highlight */
static void update_palette(Palette& palette)
{
    palette.set(Palette::ColorId::button_bg,
                Palette::GroupId::normal,
                Palette::black);
    palette.set(Palette::ColorId::text,
                Palette::GroupId::normal,
                Palette::white);
    palette.set(Palette::ColorId::text_highlight,
                Palette::GroupId::normal,
                Color(0x37949eff));
    palette.set(Palette::ColorId::button_fg,
                Palette::GroupId::normal,
                Color(0x37949eff));

    palette.set(Palette::ColorId::button_bg,
                Palette::GroupId::active,
                Palette::black);
    palette.set(Palette::ColorId::text,
                Palette::GroupId::active,
                Palette::white);
    palette.set(Palette::ColorId::text_highlight,
                Palette::GroupId::active,
                Color(0x37949eff));
    palette.set(Palette::ColorId::button_fg,
                Palette::GroupId::active,
                Color(0x37949eff));
}

static void update_palette_left(Palette& palette)
{
    palette.set(Palette::ColorId::button_bg,
                Palette::GroupId::normal,
                Color(0xab7e6dff));
    palette.set(Palette::ColorId::text,
                Palette::GroupId::normal,
                Palette::white);
    palette.set(Palette::ColorId::text_highlight,
                Palette::GroupId::normal,
                Color(0x825639ff));
    palette.set(Palette::ColorId::button_fg,
                Palette::GroupId::normal,
                Color(0x825639ff));

    palette.set(Palette::ColorId::button_bg,
                Palette::GroupId::active,
                Color(0xab7e6dff));
    palette.set(Palette::ColorId::text,
                Palette::GroupId::active,
                Palette::white);
    palette.set(Palette::ColorId::text_highlight,
                Palette::GroupId::active,
                Color(0x825639ff));
    palette.set(Palette::ColorId::button_fg,
                Palette::GroupId::active,
                Color(0x825639ff));
}

static void update_local_palette(Widget& widget, const std::function<void(Palette&)>& update)
{
    widget.reset_palette();
    Palette local_palette(widget.palette());
    update(local_palette);
    widget.palette(local_palette);
}

int main(int argc, char** argv)
{
	egt::Application app(argc,argv);
	egt::TopWindow win;

	// create the logo in the center of the screen
	egt::ImageLabel logo(egt::Image("icon:egt_logo_black.png;128"));
	logo.align(egt::AlignFlag::center);
	win.add(logo);

	// create a combo box to allow changing of the theme
    const std::pair<std::string, std::function<std::unique_ptr<egt::Theme>()>> combo_items[] =
    {
        {"Default Theme", []{ return std::make_unique<egt::Theme>(); }},
        {"Lapis", []{ return std::make_unique<egt::LapisTheme>(); }},
        {"Midnight", []{ return std::make_unique<egt::MidnightTheme>(); }},
        {"Sky", []{ return std::make_unique<egt::SkyTheme>(); }},
        {"Shamrock", []{ return std::make_unique<egt::ShamrockTheme>(); }},
        {"Coconut", []{ return std::make_unique<egt::CoconutTheme>(); }},
        {"Ultra Violet", []{ return std::make_unique<egt::UltraVioletTheme>(); }},
    };

    egt::ComboBox themeCombo;
    for (const auto& i : combo_items)
    	themeCombo.add_item(std::make_shared<egt::StringItem>(i.first));
    themeCombo.align(egt::AlignFlag::top | egt::AlignFlag::center);
    themeCombo.margin(5);
    win.add(themeCombo);

	// create a text label to show the value of the slider
	auto label1 = std::make_shared<egt::Label>("Set", egt::Rect(400, 270, 100, 20));
	label1->fill_flags(egt::Theme::FillFlag::blend);
	label1->text_align(egt::AlignFlag::center);
	win.add(label1);


	// create horizontal slider 1
	auto slider1 = std::make_shared<egt::SliderB>(egt::Rect(100, 300, 600, 80), 0, 100, 50);
//	slider1->slider_flags().set({egt::SliderB::SliderBFlag::show_labels});
	slider1->label_interval(10);
	slider1->live_update(true);
	update_local_palette(*slider1.get(), update_palette);
	win.add(slider1);

	// create horizontal slider 2
	auto slider2 = std::make_shared<egt::SliderBF>(egt::Rect(100, 390, 600, 80), 0, 200, 50);
	slider2->slider_flags().set({egt::SliderB::SliderBFlag::bump_top, egt::SliderB::SliderBFlag::show_value, egt::SliderB::SliderBFlag::show_labels});
	slider2->padding(10);
	slider2->label_interval(20);
	update_local_palette(*slider2.get(), update_palette);
	win.add(slider2);

	// create vertical slider 3
	auto slider3 = std::make_shared<egt::SliderB>(egt::Rect(-8, -8, 100, 496), 5, 40, 5, egt::Orientation::vertical);
	slider3->slider_flags().set({egt::SliderB::SliderBFlag::bump_right, egt::SliderB::SliderBFlag::show_labels});
	slider3->label_interval(5);
	slider3->padding(8);
	update_local_palette(*slider3.get(), update_palette_left);
	win.add(slider3);

	//create vertical slider 4
	auto slider4 = std::make_shared<egt::SliderB>(egt::Rect(708, -8, 100, 496), 0, 10, 5, egt::Orientation::vertical);
	slider4->slider_flags().set({egt::SliderB::SliderBFlag::show_labels, egt::SliderB::SliderBFlag::highlight_value});
	slider4->label_interval(1);
	slider4->padding(8);
	update_local_palette(*slider4.get(), update_palette);
	slider4->font(egt::Font(25, egt::Font::Weight::bold));
	win.add(slider4);

    themeCombo.on_selected_changed([&combo_items, &themeCombo, &win, &slider1, &slider2, &slider3, &slider4] ()
	{
    	const auto s = themeCombo.item_at(themeCombo.selected());
    	const auto i = std::find_if(begin(combo_items), end(combo_items), [&s](const auto & element)
		{
    		return s->text() == element.first;
		});
    	if (i != end(combo_items))
    		global_theme(i->second());

    	update_local_palette(*slider1.get(), update_palette);
    	update_local_palette(*slider2.get(), update_palette);
    	update_local_palette(*slider3.get(), update_palette_left);
    	update_local_palette(*slider4.get(), update_palette);

    	// force redraw (caching) of the handles to correctly copy new global
    	// background color into handle theme
    	slider1->redraw_handle();
    	slider2->redraw_handle();
    	slider3->redraw_handle();
    	slider4->redraw_handle();

    	win.damage();
    });


	// propagate value changes to the text control
	slider1->on_value_changed([slider1, label1] ()
	{
		label1->text(std::to_string(slider1->value()));
	});

	win.show();
	return app.run();
}

