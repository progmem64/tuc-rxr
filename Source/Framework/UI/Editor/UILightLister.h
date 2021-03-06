#pragma once
#include "../UIContainerLister.h"
#include "../UILabel.h"
#include "../UIButtonText.h"

class UILightLister : public UIContainerLister
{
	class Item : public UIContainer
	{
	public:
		Item(const LightSource& l, bool show)
			:
			UIContainer(show),
			m_lblType(getFont(),getType(l.type)),
			m_btnClose(UIButton::Style::Royal,getFont(),"X"),
			m_lblOrigin(getFont(),getOriginText(l.type, l.origin)),
			m_extra(getFont(),getExtraText(l)),
			m_color(Color(l.color.r, l.color.g, l.color.b).fromGamma()),
			m_light(l)
		{
			m_objs.add(m_lblType.getRef());
			m_objs.add(m_btnClose.getRef());
			m_objs.add(m_lblOrigin.getRef());
			m_btnClose->adjustToFontHeadline();
			if (m_extra->getText().length())
				m_objs.add(m_extra.getRef());
			m_objs.registerAll(this);

			m_btnClose->setOnClickCallback([this](IClickable*)
			{
				m_plsDelete = true;
			});
		}
		virtual ~Item()
		{
			IReceiver::unregisterMe();
		}
		bool plsErase() const
		{
			return m_plsDelete;
		}
		virtual void setDim(const PointF& d) override
		{
			// only width matters
			float w = d.x;

			m_lblType->setOrigin({ 10,10 });
			m_lblOrigin->setOrigin(m_lblType->getRect().getBottomLeft() + PointF(0.0f, 10.0f));
			m_btnClose->setOrigin({ w - 10.0f - m_btnClose->getDim().x,10 });

			float y = m_btnClose->getRect().getBottomRight().y + 10.0f;

			if (m_extra->getText().length())
			{
				m_extra->setOrigin(m_lblOrigin->getRect().getBottomLeft() + PointF(0.0f, 10.0f));
				y = m_extra->getRect().getBottomRight().y + 10.0f;
			}
			UIContainer::setDim({ w,y });
		}

		virtual void draw(Drawing& draw) override
		{
			pushDrawTransforms(draw, PointF(0.0f));
			m_objs.draw(draw);
			draw.rect({ 100.0f,10.0f,200.0f,30.0f }, m_color);
			popDrawTransforms(draw);
		}

		static Font& getFont()
		{
			return Drawing::getFont(Font::Style::Text, Font::Size::S);
		}

		static std::string getType(LightType t)
		{
			switch (t)
			{
			case LightType::Directional: return "Directional";
			case LightType::PointLight: return "Point";
			}
			return "ERROR";
		}

		static std::string getOriginText(LightType t, const glm::vec3& p)
		{
			std::string txt = t == LightType::PointLight ? "Origin" : "Direction";
			txt += ": ";
			txt += std::to_string(p.x) + " | " + std::to_string(p.y) + " | " + std::to_string(p.z);
			return txt;
		}
		static std::string getExtraText(const LightSource& l)
		{
			std::string t;
			if(l.type == LightType::PointLight)
			{
				t = "attenuation: " + std::to_string(l.attenuation);
			}
			return t;
		}
		const LightSource& getLight() const
		{
			return m_light;
		}
	private:
		UIObjectList m_objs;
		refable<UILabel> m_lblType;
		refable<UIButtonText> m_btnClose;
		refable<UILabel> m_lblOrigin;
		refable<UILabel> m_extra;
		const Color m_color;
		bool m_plsDelete = false;
		const LightSource m_light;
	};
public:
	UILightLister(bool show)
		:
		UIContainerLister(show)
	{
		
	}
	void add(const LightSource& l)
	{
		addContainer(owner_ptr<UIObject>(new Item(l, true)));
	}

	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		bool h = UIContainer::mouseUp(mpos, button);
		if(h)
		{
			// was some item deleted?
			update();
		}
		return h;
	}
	std::vector<LightSource> getLights() const
	{
		std::vector<LightSource> s;
		for(const auto& c : m_cons)
		{
			const Item* pi = dynamic_cast<const Item*>(c.get());
			if(pi)
			{
				s.push_back(pi->getLight());
			}
		}
		return s;
	}
private:
	void update()
	{
		for(auto& c : m_cons)
		{
			Item* pi = dynamic_cast<Item*>(c.get());
			if(pi && pi->plsErase())
			{
				erase(c.getRef());
				break;
			}
		}
	}
};
