/*
 * Copyright (C) 2021 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "egt/detail/enum.h"
#include "egt/detail/string.h"
#include "sliderb.h"

namespace egt
{
inline namespace v1
{

template<>
void SliderBType<float>::draw_label(Painter& painter, float value, const Rect& rect)
{
    const auto text = detail::format(value, 1);
    const auto f = TextWidget::scale_font(rect.size(), text, this->font());
    painter.set(f);

    const auto text_size = painter.text_size(text);
    const auto target = detail::align_algorithm(text_size,
                        rect,
                        AlignFlag::center,
                        5);
    painter.draw(target.point());
    painter.draw(text);
}

template<>
void SliderBType<double>::draw_label(Painter& painter, double value, const Rect& rect)
{
    const auto text = detail::format(value, 1);
    const auto f = TextWidget::scale_font(rect.size(), text, this->font());
    painter.set(f);

    const auto text_size = painter.text_size(text);
    const auto target = detail::align_algorithm(text_size,
                        rect,
                        AlignFlag::center,
                        5);
    painter.draw(target.point());
    painter.draw(text);
}

template<>
const std::pair<SliderBBase::SliderBFlag, char const*> detail::EnumStrings<SliderBBase::SliderBFlag>::data[] =
{
    {SliderBBase::SliderBFlag::show_labels, "show_labels"},
    {SliderBBase::SliderBFlag::show_value, "show_value"},
    {SliderBBase::SliderBFlag::bump_top, "bump_top"},
    {SliderBBase::SliderBFlag::bump_right, "bump_right"},
	{SliderBBase::SliderBFlag::highlight_value, "highlight_value"},
};

}
}

