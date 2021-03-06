#pragma once
#include "UIObject.h"
#include "Interfaces/ILabelable.h"

class UILabel : public UIObject, public ILabelable
{
public:
	UILabel(Font& font, const std::string& txt)
		:
		m_font(font)
	{
		UILabel::setText(txt);
	}
	virtual ~UILabel() override
	{

	}
	virtual void draw(Drawing& draw) override
	{
		if (!isVisible()) return;

		m_font.write(draw, getText(), getOrigin());
	}

	virtual void setText(const std::string& text) override
	{
		setDim(m_font.getDim(text));
		ILabelable::setText(text);
	}
private:
	Font& m_font;
};
