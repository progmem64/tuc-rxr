#pragma once
#include "UIContainer.h"
#include "UIButtonText.h"

class UIWindow : public UIContainer
{
public:
	UIWindow(bool show)
		:
		UIContainer(show),
		m_btnCancel(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "X")
	{
		//m_btnCancel.setZIndex(getZIndex() + 1);
		m_btnCancel.adjustToFontHeadline();
		m_btnCancel.registerMe(this);

		m_btnCancel.setOnClickCallback([this](IClickable*)
		{
			hide();
		});
	}

	virtual ~UIWindow() override
	{
		m_btnCancel.unregisterMe();
		// window will unregister automatically
	}

	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		UIContainer::draw(draw);
		// smaller clipping rect for drawing cancel button outside container
		pushDrawTransforms(draw, PointF(-30.0f));
		m_btnCancel.draw(draw);
		popDrawTransforms(draw);
	}

	// input
	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		bool handled = sendMouseDown(mpos, button);
		return m_isMouseInside || handled;
	}

	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		bool handled = sendMouseUp(mpos, button);
		return m_isMouseInside || handled;
	}

	virtual bool mouseMove(const PointF& mpos, const PointF& mdiff, bool handled) override
	{
		handled = sendMouseMove(mpos, mdiff, handled);
		m_isMouseInside = getRect().isPointInside(mpos);
		return m_isMouseInside || handled;
	}

	virtual bool keyUp(SDL_Scancode s) override
	{
		bool handled = sendKeyUp(s);

		if (!handled)
		{
			if (s == SDL_SCANCODE_ESCAPE)
			{
				hide();
				return true;
			}
		}
		return handled;
	}

	virtual void setDim(const PointF& d) override
	{
		UIContainer::setDim(d);
		m_btnCancel.setOrigin({ d.x - m_btnCancel.getDim().x + 30.0f, -20.0f });
	}

protected:
	// sets client area size and returns origin of the client area
	virtual PointF setClientArea(const PointF& dim)
	{
		// TODO fix
		setDim(dim + PointF(20.0f));
		return PointF(10.0f);
	}
	virtual RectF getClientArea() const
	{
		return RectF(PointF(10.0f), getDim() - PointF(10.0f));
	}
protected:	
	UIButtonText m_btnCancel;
};
