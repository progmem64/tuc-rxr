#pragma once
#include "GameState.h"
#include "../UI/UIButton.h"
#include "../Sound/Sound.h"

class StateMenu : public GameState
{
public:
	StateMenu()
		:
		GameState(GameState::TransitionState::ForcePreserve)
	{
		m_myButt.setDim({ 300, 100 });
		m_myButt.center();

		m_myButt.registerMe(this);
		Sound::playMusic(Sound::Music::Hey);
	}
	virtual ~StateMenu()
	{}

	virtual bool update(float dt) override
	{
		if (m_myButt.isClicked(true))
			Sound::playMusic(Sound::Music::Theme);
			//Sound::playSound(Sound::Sfx::Plop);

		return false;
	}

	virtual void composeFrame(float dt) override
	{
		Drawing d;

		m_myButt.draw(d);

		d.rect(RectF::constructFromPoint(m_mpos, 10.0f), Color::Red());
	}
	virtual bool mouseMove(const PointF& mpos, bool handled) override
	{
		handled = GameState::mouseMove(mpos, handled);

		m_mpos = mpos;
		return handled;
	}
private:
	PointF m_mpos;
	UIButton m_myButt;
};
