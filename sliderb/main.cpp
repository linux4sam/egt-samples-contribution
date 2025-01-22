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

#include <egt/ui>
#include <egt/uiloader.h>
#include <iostream>

int main(int argc, char** argv)
{
	egt::Application app(argc, argv);
#ifdef EXAMPLEDATA
	egt::add_search_path(EXAMPLEDATA);
#endif

	egt::experimental::UiLoader loader;
	auto win = std::static_pointer_cast<egt::TopWindow>(loader.load("file:ui.xml"));

	auto slider1 = win->find_child<egt::Slider>("slider1");
	if (!slider1)
	{
		std::cerr << "\"slider1\" egt::Slider widget not found!" << std::endl;
		return 1;
	}
	slider1->live_update(true);

	auto label1 = win->find_child<egt::Label>("label1");
	if (!label1)
	{
		std::cerr << "\"label1\" egt::Label widget not found!" << std::endl;
		return 1;
	}
	label1->text(std::to_string(slider1->value()));

	// propagate value changes to the text control
	slider1->on_value_changed([slider1, label1] ()
	{
		label1->text(std::to_string(slider1->value()));
	});

	win->show();
	return app.run();
}

