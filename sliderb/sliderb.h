/*
 * Copyright (C) 2021 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * References to SliderB are shorthand for SliderBump
 * Named because of Bump type handle
 */

#ifndef SLIDERB_H_
#define SLIDERB_H_

#include <cairo/cairo.h>
#include <egt/detail/alignment.h>
#include <egt/detail/enum.h>
#include <egt/detail/math.h>
#include <egt/detail/meta.h>
#include <egt/flags.h>
#include <egt/frame.h>
#include <egt/painter.h>
#include <egt/serialize.h>
#include <egt/textwidget.h>
#include <egt/valuewidget.h>
#include <egt/widgetflags.h>
#include <egt/types.h>


namespace egt
{
inline namespace v1
{

/**
 * Base class for SilderType
 * Different flags due to differences over standard slider
 */
struct SliderBBase
{
	// Slider flags
	enum class SliderBFlag
	{
		// show range labels
		show_labels = detail::bit(0),

		// show value label replacing button in bump
		show_value = detail::bit(1),

		// for horizontal slider draw bump on top (default is on bottom)
		bump_top = detail::bit(2),

		// for vertical slider draw bump on right (default is on left)
		bump_right = detail::bit(3),

		// draw the current value in a highlight color
		highlight_value = detail::bit(4),

	};

	using SliderBFlags = Flags<SliderBBase::SliderBFlag>;
};


// Bump slider that can be used to select value from a range
template <class T>
class SliderBType : protected SliderBBase, public ValueRangeWidget<T>
{
public:

	using SliderBBase::SliderBFlags;
	using SliderBBase::SliderBFlag;

    /**
     * @param[in] rect Rectangle for the widget.
     * @param[in] start Starting value for the range.
     * @param[in] end Ending value in the range.
     * @param[in] value Current value in the range.
     * @param[in] orient Vertical or horizontal Orientation.
     */
    explicit SliderBType(const Rect& rect, T start = 0, T end = 100, T value = 0,
                        Orientation orient = Orientation::horizontal) noexcept;

    /**
     * @param[in] start Starting value for the range.
     * @param[in] end Ending value in the range.
     * @param[in] value Current value in the range.
     * @param[in] orient Vertical or horizontal Orientation.
     */
    explicit SliderBType(T start = 0, T end = 100, T value = 0,
                        Orientation orient = Orientation::horizontal) noexcept
        : SliderBType(Rect(), start, end, value, orient)
    {
    }

    /**
     * @param[in] parent The parent Frame.
     * @param[in] rect Rectangle for the widget.
     * @param[in] start Starting value for the range.
     * @param[in] end Ending value in the range.
     * @param[in] value Current value in the range.
     * @param[in] orient Vertical or horizontal Orientation.
     */
    SliderBType(Frame& parent, const Rect& rect, T start = 0, T end = 100, T value = 0,
               Orientation orient = Orientation::horizontal) noexcept
        : SliderBType(rect, start, end, value, orient)
    {
        parent.add(*this);
    }

    /**
     * @param[in] parent The parent Frame.
     * @param[in] start Starting value for the range.
     * @param[in] end Ending value in the range.
     * @param[in] value Current value in the range.
     * @param[in] orient Vertical or horizontal Orientation.
     */
    explicit SliderBType(Frame& parent, T start = 0, T end = 100, T value = 0,
                        Orientation orient = Orientation::horizontal) noexcept
        : SliderBType(Rect(), start, end, value, orient)
    {
        parent.add(*this);
    }

    // handle events for the bump slider
    void handle(Event& event) override
    {
        Widget::handle(event);

        switch (event.id())
        {
        case EventId::raw_pointer_up:
            if (m_invoke_pending)
            {
                m_invoke_pending = false;
                this->on_value_changed.invoke();
            }
            break;
        case EventId::pointer_drag_start:
            m_start_offset = to_offset(this->m_value);
            break;
        case EventId::pointer_drag:
            if (m_orient == Orientation::horizontal)
            {
                const auto diff = event.pointer().point - event.pointer().drag_start;
                update_value(to_value(m_start_offset + diff.x()));
            }
            else
            {
                const auto diff = event.pointer().point - event.pointer().drag_start;
                update_value(to_value(m_start_offset - diff.y()));
            }
            break;
        default:
            break;
        }
    }

    void draw(Painter& painter, const Rect& rect) override;

    using ValueRangeWidget<T>::value;

    T value(T value) override
    {
        T orig = this->value();

        update_value(value);

        if (m_invoke_pending)
        {
            m_invoke_pending = false;
            this->on_value_changed.invoke();
        }

        return orig;
    }

    /**
     *  Enable or disable the live update feature.
     *
     *  When enabled, the on_value_changed event happens also during
     *  cursor dragging.
     *
     *  @param[in] enable Enable or disable the live update mode.
     */
    void live_update(bool enable)
    {
        m_live_update = enable;
    }

    /**
     * Get the Orientation.
     */
    EGT_NODISCARD Orientation orient() const { return m_orient; }

    /**
     * Set the Orientation.
     *
     * @param[in] orient Vertical or horizontal Orientation.
     */
    void orient(Orientation orient)
    {
        if (detail::change_if_diff<>(m_orient, orient))
        {
    		m_handle_surface = nullptr;
            this->damage();
        }
    }

    /**
     * Set the detail or line width
     */
    void detail_width(DefaultDim detail_width)
    {
    	if (detail::change_if_diff<>(m_detail_width, detail_width))
    	{
    		m_handle_surface = nullptr;
    		this->damage();
    	}
    }

    /**
     * Get the detail width
     */
    EGT_NODISCARD DefaultDim detail_width() const { return m_detail_width; }

    /*
     * Get the label interval
     */
    EGT_NODISCARD T label_interval() const { return m_label_interval; }

    /*
     * Set the label interval
     */
    void label_interval(T label_interval)
    {
    	if (detail::change_if_diff(m_label_interval, label_interval))
    	{
    		this->damage();
    	}
    }

    // force a redraw of the handle bump
    void redraw_handle()
    {
    	if (m_handle_surface)
    	{
    		m_handle_surface = nullptr;
    	}
    }

    /// Get the current slider flags.
    EGT_NODISCARD const SliderBFlags& slider_flags() const { return m_slider_flags; }

    /// Get the current slider flags.
    SliderBFlags& slider_flags() { return m_slider_flags; }

    void serialize(Serializer& serializer) const override;

protected:

    // cached image of the handle
    shared_cairo_surface_t m_handle_surface;

    EGT_NODISCARD shared_cairo_surface_t handle_surface() const
    {
    	return m_handle_surface;
    }

    //  Draw  the bump part of the handle to an off-screen cairo surface
    shared_cairo_surface_t draw_cached_handle();

    /// Convert a value to an offset.
    EGT_NODISCARD int to_offset(int value) const
    {
        const auto b = this->content_area();
        if (m_orient == Orientation::horizontal)
            return egt::detail::normalize<float>(value, this->m_start, this->m_end,
                                                 0, b.width());
        else
            return egt::detail::normalize<float>(value, this->m_start, this->m_end,
                                                 0, b.height());
    }

    // Convert an offset to value.
    EGT_NODISCARD T to_value(int offset) const
    {
        const auto b = this->content_area();
        if (m_orient == Orientation::horizontal)
            return egt::detail::normalize<float>(offset, 0, b.width(),
                                                 this->m_start, this->m_end);
        else
            return egt::detail::normalize<float>(offset, 0, b.height(),
                                                 this->m_start, this->m_end);
    }

    /// Update the value without notifying the handlers.
    void update_value(T value)
    {
        if (this->m_start < this->m_end)
            value = detail::clamp<T>(value, this->m_start, this->m_end);
        else
            value = detail::clamp<T>(value, this->m_end, this->m_start);

        if (detail::change_if_diff<>(this->m_value, value))
        {
            this->damage();
            if (m_live_update)
            {
                this->on_value_changed.invoke();
            }
            else
            {
                m_invoke_pending = true;
            }
        }
    }

    /// Get the calculated handle width.
    EGT_NODISCARD int handle_width() const;

    /// Get the calculated handle height.
    EGT_NODISCARD int handle_height() const;

    /// Get the handle box for the current value.
    EGT_NODISCARD Rect handle_box() const
    {
        return handle_box(this->m_value);
    }

    /// Get the handle box for the specified value.
    EGT_NODISCARD Rect handle_box(T value) const;

    /// Draw a label in the specified rect (int)
    void draw_label(Painter& painter, T value, const Rect& rect)
    {
        const auto text = std::to_string(value);
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

    /// Draw the handle.
    void draw_handle(Painter& painter);

	/// Draw the line.
	void draw_scale(Painter& painter);

	// draw a single scale item
	void draw_scale_item(Painter& painter, T value);

	// Draw the button or value
	void draw_button(Painter& painter);

	/// Orientation of the slider.
	Orientation m_orient{Orientation::horizontal};

	/// When true, an invoke of events has been queued to occur.
	bool m_invoke_pending{false};

	/// When true, notify handlers even during drag.
	bool m_live_update{false};

	/// Slider flags.
	SliderBFlags m_slider_flags{};

    /// When dragging, the offset at the drag start.
    int m_start_offset{0};

    // Interval marker for scale
    T m_label_interval{0};

    /**
     * Detail width for internal lines
     */
    DefaultDim m_detail_width{2};

private:

    void deserialize(Serializer::Properties& props) override;

};

/**
 * This is a slider that can be used to select integer value from a range.
 *
 * @ingroup controls
 */
class EGT_API SliderB : public SliderBType<int>
{
public:
    using SliderBType<int>::SliderBType;

    EGT_NODISCARD std::string type() const override
    {
        return "SliderB";
    }
};

/**
 * This is a slider that can be used to select floating value from a range.
 *
 * @ingroup controls
 */
class EGT_API SliderBF : public SliderBType<float>
{
public:
    using SliderBType<float>::SliderBType;

    EGT_NODISCARD std::string type() const override
    {
        return "SliderBF";
    }
};

/**
 * This is a slider that can be used to select double value from a range.
 *
 * @ingroup controls
 */
class EGT_API SliderBD : public SliderBType<double>
{
public:
    using SliderBType<double>::SliderBType;

    EGT_NODISCARD std::string type() const override
    {
        return "SliderBD";
    }
};

template <class T>
SliderBType<T>::SliderBType(const Rect& rect, T start, T end, T value,
                          Orientation orient) noexcept
    : ValueRangeWidget<T>(rect, start, end, value),
      m_orient(orient),
	  m_label_interval(0),
	  m_detail_width(2)
{
    this->name("SliderB" + std::to_string(this->m_widgetid));
    this->fill_flags(Theme::FillFlag::solid);
    this->grab_mouse(true);
    this->border_radius(4.0);

    // default label interval is half of range
    this->label_interval((end - start) / 2.);
}

template <class T>
void SliderBType<T>::draw(Painter& painter, const Rect& /*rect*/)
{
	// draw the bounding box
	this->draw_box(painter, Palette::ColorId::bg, Palette::ColorId::border);

    // draw scale
    draw_scale(painter);

    // draw handle
    draw_handle(painter);

    // draw the button on the handle (or the value)
    draw_button(painter);
}

// handle width is always calculate as width of the bump when viewed horizontally
template <class T>
int SliderBType<T>::handle_width() const
{
	const auto b = this->content_area();

	// handle width is proportional to height of the entire widget when viewed horizontally
	auto width = b.width();
	auto height = b.height();

	if (m_orient == Orientation::horizontal)
		return height * 2;
	else
		return width * 2;
}

// handle height is always calculated as height of the bump when viewed horizontally
template <class T>
int SliderBType<T>::handle_height() const
{
    const auto b = this->content_area();
    auto bw = b.width();
    auto bh = b.height();

    if (slider_flags().is_set(SliderBFlag::show_labels))
    {
    	if (m_orient == Orientation::horizontal)
    		bh = bh * 0.4;
    	else
    		bh = bw * 0.4;
    }
    else
    {
    	// no labels
    	if (m_orient == Orientation::horizontal)
    		bh = bh * 0.5;
    	else
    		bh = bw * 0.5;
    }

    return bh;
}

template <class T>
Rect SliderBType<T>::handle_box(T value) const
{
    const auto b = this->content_area();
    const auto dimw = handle_width();
	const auto dimh = handle_height();

    if (m_orient == Orientation::horizontal)
    {
    	// horizontal aligned slider
	   	auto value_x = b.x() + to_offset(value);

	   	auto handle_x = value_x - dimw / 2;
	   	auto handle_y = b.y();

	   	// if handle is on the bottom adjust top corner
	   	// to be relative to bottom of widget
	   	if (!slider_flags().is_set(SliderBFlag::bump_top))
	   		handle_y += b.height() - dimh;

	    return { handle_x, handle_y, dimw, dimh};
    }
    else
    {
    	// vertical aligned slider
    	auto value_y = b.bottom() - to_offset(value);

    	auto handle_x = b.x();
    	auto handle_y = value_y - dimw / 2;

    	// if handle is on the right adjust top corner
    	// to be relative to right of widget
    	if (slider_flags().is_set(SliderBFlag::bump_right))
    		handle_x += b.width() - dimh;

    	return { handle_x, handle_y, dimh, dimw};
    }
}

// draw a single scale item
template <class T>
void SliderBType<T>::draw_scale_item(Painter& painter, T value)
{
	const auto b = this->content_area();

	Point a1, a2;
	Rect target_rect;
	Size label_size(0, 0);

    if (m_orient == Orientation::horizontal)
    {
    	// horizontal aligned slider
	   	auto value_x = b.x() + to_offset(value);
	   	const auto base_inset = b.height() * 0.2;
	   	auto line_end = b.height();
	   	if (slider_flags().is_set(SliderBFlag::show_labels))
	   		line_end *= 0.6;
	   	else
	   		line_end *= 0.9;

	   	if (slider_flags().is_set(SliderBFlag::bump_top))
	   	{
	   		a1 = Point(value_x, b.y() + base_inset);
	   		a2 = Point(value_x, b.y() + line_end);
	   		target_rect = Rect(value_x - 40, b.y() + line_end, 80, b.height() - line_end);
	   	}
	   	else
	   	{
	   		a1 = Point(value_x, b.bottom() - base_inset);
	   		a2 = Point(value_x, b.bottom() - line_end);
	   		target_rect = Rect(value_x - 40, b.y(), 80, line_end);
	   	}

    	// calculated label size even if not being drawn
    	label_size = Size(0, b.height() * 0.4);
    }
    else
    {
    	auto value_y = b.bottom() - to_offset(value);
	   	const auto base_inset = b.width() * 0.2;
	   	auto line_end = b.width();
	   	if (slider_flags().is_set(SliderBFlag::show_labels))
	   		line_end *= 0.6;
	   	else
	   		line_end *= 0.9;

	   	if (slider_flags().is_set(SliderBFlag::bump_right))
	   	{
	   		a1 = Point(b.right() - base_inset, value_y);
	   		a2 = Point(b.right() - line_end, value_y);
	   		target_rect = Rect(b.x(), value_y - 40, b.width() - line_end, 80);
	   	}
	   	else
	   	{
	   		a1 = Point(b.x() + base_inset, value_y);
	   		a2 = Point(b.x() + line_end, value_y);
	   		target_rect = Rect(b.x() + line_end, value_y - 40, b.width() * 0.4, 80);
	   	}

    	label_size = Size(b.width() * 0.4, 0);
    }

    if (slider_flags().is_set(SliderBFlag::highlight_value) &&
    	(value == this->m_value))
    {
    	painter.set(this->color(Palette::ColorId::text_highlight));
    }
    else
    {
    	painter.set(this->color(Palette::ColorId::text));
    }

	painter.line_width(this->detail_width());
    painter.draw(a1, a2);
    painter.stroke();

    if (slider_flags().is_set(SliderBFlag::show_labels))
    {
    	draw_label(painter, value, target_rect);
    }
}

// draw the scale
template <class T>
void SliderBType<T>::draw_scale(Painter& painter)
{
	// the scale contains the tick marks and corresponding text (if required)
	// it is inset from the border and has a theme defined background
	this->theme().draw_box(painter,
						   Theme::FillFlag::solid,
						   this->content_area(),
						   Palette::transparent,
						   this->color(Palette::ColorId::button_bg, Palette::GroupId::normal)
						   );

    if (this->m_label_interval == 0.)
    	return;

    // draw the label tick marks
    for(auto i = this->m_start; i <= this->m_end; i += this->m_label_interval)
    {
    	draw_scale_item(painter, i);
    }
}

/**
 * Draw  the bump part of the handle to an off-screen cairo surface and
 * cache it for later use
 */
template <class T>
shared_cairo_surface_t SliderBType<T>::draw_cached_handle()
{
	const auto handle_rect = handle_box();

	Point a1, a2, b1, b2; // start and end points for straight lines
	Point midPoint; // tip of the handle
	Point cp1, cp2, cp3, cp4; // control points for the curve

	cairo_matrix_t axis_matrix;
	cairo_matrix_init_identity(&axis_matrix);

	if (m_orient == Orientation::horizontal)
	{
		// horizontal aligned handle
		a2 = Point(0, handle_rect.height());
		midPoint = Point(handle_rect.width() / 2, 0);
		b1 = Point(handle_rect.width(), handle_rect.height());

		// calculate control points
		auto vx = handle_rect.width() / 4;
		cp1 = Point(vx, a2.y());
		cp2 = Point(midPoint.x() - vx, 0);
		cp3 = Point(midPoint.x() + vx, 0);
		cp4 = Point(b1.x() - vx, b1.y());

		if (slider_flags().is_set(SliderBFlag::bump_top))
		{
			axis_matrix.yy = -1.0;
			axis_matrix.y0 = handle_rect.height();
		}
	}
	else
	{
		// vertical aligned handle
		a2 = Point(0, handle_rect.height());
		midPoint = Point(handle_rect.width(), handle_rect.height() / 2);
		b1 = Point(0, 0);

		// calculate control points
		auto vy = handle_rect.height() / 4;
		cp1 = Point(0, a2.y() - vy);
		cp2 = Point(midPoint.x(), midPoint.y() + vy);
		cp3 = Point(midPoint.x(), midPoint.y() - vy);
		cp4 = Point(0, b1.y() + vy);

		if (slider_flags().is_set(SliderBFlag::bump_right))
		{
			axis_matrix.xx = -1.0;
			axis_matrix.x0 = handle_rect.width();
		}
	}

	this->m_handle_surface = shared_cairo_surface_t(cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
			handle_rect.width(),
			handle_rect.height()),
			cairo_surface_destroy);

	cairo_t* handle_context = cairo_create(this->m_handle_surface.get());

	// draw the filled part of the handle
	cairo_set_source(handle_context, this->color(Palette::ColorId::bg).pattern());
	cairo_set_line_width(handle_context, this->detail_width());
	cairo_save(handle_context);

	// apply the matrix transform to draw on the correct side
	cairo_set_matrix(handle_context, &axis_matrix);

	cairo_move_to(handle_context, a2.x(), a2.y());
	cairo_curve_to(handle_context,
			cp1.x(), cp1.y(),
			cp2.x(), cp2.y(),
			midPoint.x(), midPoint.y());
	cairo_curve_to(handle_context,
			cp3.x(), cp3.y(),
			cp4.x(), cp4.y(),
			b1.x(), b1.y());
	cairo_fill_preserve(handle_context);

	// draw the outline of the handle
	cairo_set_source(handle_context, this->color(Palette::ColorId::button_fg).pattern());
	cairo_stroke(handle_context);

	cairo_restore(handle_context);

	return m_handle_surface;
}

template <class T>
void SliderBType<T>::draw_handle(Painter& painter)
{
	const auto bb = this->content_area();
	const auto handle_rect = handle_box();

	Point a1, b2; // start and end points for straight lines

	if (!m_handle_surface)
	{
		draw_cached_handle();
	}

	//Calculate location of the base line
	if (m_orient == Orientation::horizontal)
	{
		if (slider_flags().is_set(SliderBFlag::bump_top))
		{
			a1 = bb.top_left();
			b2 = bb.top_right();
		}
		else
		{
			a1 = bb.bottom_left();
			b2 = bb.bottom_right();
		}
	}
	else
	{
		if (slider_flags().is_set(SliderBFlag::bump_right))
		{
			a1 = bb.bottom_right();
			b2 = bb.top_right();
		}
		else
		{
			a1 = bb.bottom_left();
			b2 = bb.top_left();
		}
	}

	painter.draw(Rect(bb));
	painter.clip();

	// set the color of the base line
	painter.set(this->color(Palette::ColorId::button_fg));
	painter.line_width(this->detail_width());

	// draw the baseline for the handle
	auto baseLine = egt::LineType<int>(a1, b2);
	painter.draw(baseLine);
	painter.stroke();

	// draw the bump by copying the cached image
	auto cr = painter.context().get();
	cairo_set_source_surface(cr, this->m_handle_surface.get(), handle_rect.x(), handle_rect.y());
	cairo_paint(cr);
}

// Draw the button or value in the handle
template <class T>
void SliderBType<T>::draw_button(Painter& painter)
{
	const auto handle_rect = handle_box();

	if (slider_flags().is_set(SliderBFlag::show_value))
	{
	    painter.set(this->color(Palette::ColorId::label_text));
		draw_label(painter, this->m_value, handle_rect);
	}
	else
	{
		// draw button
		Rect button_rect = Rect(handle_rect);

		if (m_orient == Orientation::horizontal)
		{
			button_rect.width(button_rect.width() / 6.);
			button_rect.height(button_rect.width());

			// offset button into center of bump
			button_rect.x(handle_rect.center().x() - button_rect.width() / 2.);

			// adjust y position of button
			if (!slider_flags().is_set(SliderBFlag::bump_top))
				button_rect.y(handle_rect.bottom() - button_rect.height());

			this->theme().draw_circle(painter,
									  Theme::FillFlag::solid,
									  button_rect,
									  this->color(Palette::ColorId::button_fg),
								      this->color(Palette::ColorId::bg),
									  this->detail_width());

			const auto bc = button_rect.center();
			const auto x1 = button_rect.width() * 0.1;
			const auto y1 = button_rect.height() * 0.3;
			const auto x2 = button_rect.width() * 0.35;

			painter.set(this->color(Palette::ColorId::button_fg));
			const shared_cairo_t cr = painter.context();
			cairo_set_line_cap(cr.get(), CAIRO_LINE_CAP_BUTT);
			cairo_move_to(cr.get(), bc.x() - x1, bc.y() + y1);
			cairo_line_to(cr.get(), bc.x() - x2, bc.y());
			cairo_line_to(cr.get(), bc.x() - x1, bc.y() - y1);
			painter.fill();

			cairo_move_to(cr.get(), bc.x() + x1, bc.y() + y1);
			cairo_line_to(cr.get(), bc.x() + x2, bc.y());
			cairo_line_to(cr.get(), bc.x() + x1, bc.y() - y1);
			painter.fill();
		}
		else
		{
			button_rect.height(button_rect.height() / 6);
			button_rect.width(button_rect.height());

			// offset button into center of bump
			button_rect.y(handle_rect.center().y() - button_rect.height() / 2.);
			button_rect.x(handle_rect.left());

			// adjust x position of button
			if (slider_flags().is_set(SliderBFlag::bump_right))
				button_rect.x(handle_rect.right() - button_rect.width());

			this->theme().draw_circle(painter,
									  Theme::FillFlag::solid,
									  button_rect,
									  this->color(Palette::ColorId::button_fg),
								      this->color(Palette::ColorId::bg),
									  this->detail_width());

			const auto bc = button_rect.center();
			const auto y1 = button_rect.height() * 0.1;
			const auto x1 = button_rect.width() * 0.3;
			const auto y2 = button_rect.height() * 0.35;

			painter.set(this->color(Palette::ColorId::button_fg));
			const shared_cairo_t cr = painter.context();
			cairo_set_line_cap(cr.get(), CAIRO_LINE_CAP_BUTT);
			cairo_move_to(cr.get(), bc.x() - x1, bc.y() - y1);
			cairo_line_to(cr.get(), bc.x(), bc.y() - y2);
			cairo_line_to(cr.get(), bc.x() + x1, bc.y() - y1);
			painter.fill();

			cairo_move_to(cr.get(), bc.x() - x1, bc.y() + y1);
			cairo_line_to(cr.get(), bc.x(), bc.y() + y2);
			cairo_line_to(cr.get(), bc.x() + x1, bc.y() + y1);
			painter.fill();
		}
	}
}

/// Draw the value label specialized for float.
template<>
EGT_API void SliderBType<float>::draw_label(Painter& painter, float value, const Rect& rect);

/// Draw the value label specialized for double.
template<>
EGT_API void SliderBType<double>::draw_label(Painter& painter, double value, const Rect& rect);

template <class T>
void SliderBType<T>::serialize(Serializer& serializer) const
{
    ValueRangeWidget<T>::serialize(serializer);

    serializer.add_property("sliderflags", m_slider_flags.to_string());
    serializer.add_property("orient", detail::enum_to_string(orient()));
}

template <class T>
void SliderBType<T>::deserialize(Serializer::Properties& props)
{
	props.erase(std::remove_if(props.begin(), props.end(), [&](auto & p)
	{
		if (std::get<0>(p) == "sliderflags")
		{
			m_slider_flags.from_string(std::get<1>(p));
			return true;
		}
		else if (std::get<0>(p) == "orient")
		{
			orient(detail::enum_from_string<Orientation>(std::get<1>(p)));
			return true;
		}
		return false;
	}), props.end());
}

/// Enum string conversion map
template<>
EGT_API const std::pair<SliderBBase::SliderBFlag, char const*> detail::EnumStrings<SliderBBase::SliderBFlag>::data[7];

}
}

#endif /* SLIDERB_H_ */
