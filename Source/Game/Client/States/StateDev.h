#pragma once
#include "../../../Framework/GameState.h"
#include "../../../Framework/UI/UIButtonText.h"
#include "../../../Framework/UI/UIMessageBox.h"
#include "../../../Framework/UI/UITextBox.h"
#include "../../../Framework/UI/UIObjectList.h"
#include "../../../Framework/UI/UICheckBox.h"
#include "../../../Framework/UI/UITabView.h"
#include "../../../Framework/UI/Editor/UIContainerMaterial.h"

class StateDev : public GameState
{
public:
	StateDev()
		:
		m_uiList({ &m_lblHeadS, &m_lblHeadM, &m_lblHeadL, &m_lblTextS, &m_lblTextM, &m_lblTextL,
			&m_btnBack, &m_btnDlg, &m_btnMsgBox, &m_dlgTest, &m_msgBox, &m_txtBox, &m_msgBoxResult,
			&m_hsvPicker, &m_checkBox, &m_tabView
		}),
		m_lblHeadS(Drawing::getFont(Font::Style::Headline, Font::Size::S), "ABCxyz"),
		m_lblHeadM(Drawing::getFont(Font::Style::Headline, Font::Size::M), "ABCxyz"),
		m_lblHeadL(Drawing::getFont(Font::Style::Headline, Font::Size::L), "ABCxyz"),
		m_lblTextS(Drawing::getFont(Font::Style::Text, Font::Size::S), "ABCxyz"),
		m_lblTextM(Drawing::getFont(Font::Style::Text, Font::Size::M), "ABCxyz"),
		m_lblTextL(Drawing::getFont(Font::Style::Text, Font::Size::L), "ABCxyz"),
		m_btnBack(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Back"),
		m_btnDlg(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "Dialog"),
		m_btnMsgBox(UIButton::Style::Royal, Drawing::getFont(Font::Style::Headline, Font::Size::S), "MessageBox"),
		m_dlgTest(),
		m_msgBox("naaaanaanaaanaaaananananaaaananananaaaa hey jude ....", UIMessageBox::Buttons::AbortRetryIgnore),
		m_txtBox(Drawing::getFont(Font::Style::Text, Font::Size::M)),
		m_msgBoxResult(Drawing::getFont(Font::Style::Text, Font::Size::M), "MsgBox result: ")
	{
		m_lblHeadS.setOrigin({ 50.0f, 50.0f });
		m_lblHeadM.setOrigin({ 50.0f, m_lblHeadS.getRect().y2 + 10.0f });
		m_lblHeadL.setOrigin({ 50.0f, m_lblHeadM.getRect().y2 + 10.0f });
		m_lblTextS.setOrigin({ m_lblHeadL.getRect().x2 + 10.0f, 50.0f });
		m_lblTextM.setOrigin({ m_lblHeadL.getRect().x2 + 10.0f, m_lblTextS.getRect().y2 + 10.0f });
		m_lblTextL.setOrigin({ m_lblHeadL.getRect().x2 + 10.0f, m_lblTextM.getRect().y2 + 10.0f });

		m_btnBack.adjustToFontHeadline();
		m_btnBack.setOrigin({ 10.0f, Framework::STD_DRAW_Y - (m_btnBack.getDim().y + 10.0f) });
		m_btnDlg.adjustToFontHeadline();
		m_btnDlg.setOrigin({ Framework::STD_DRAW_X - (m_btnDlg.getDim().x + 10.0f), 10.0f });
		m_btnMsgBox.adjustToFontHeadline();
		m_btnMsgBox.setOrigin({ Framework::STD_DRAW_X - (m_btnMsgBox.getDim().x + 10.0f), 100.0f });
		m_dlgTest.setDim({ 300.0f, 300.0f });
		m_dlgTest.setCenter(Framework::getScreenCenter());
		m_dlgTest.setZIndex(1);
		//m_msgBox.setDim({ 600.0f, 300.0f });
		m_msgBox.setCenter(Framework::getScreenCenter());
		m_msgBox.setZIndex(1);

		m_txtBox.setText("Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.");
		m_txtBox.setDim({ 300.0f, 300.0f });
		m_txtBox.setOrigin({ 50.0f, 300.0f });

		m_msgBoxResult.setOrigin({ 800.0f, 100.0f });

		m_hsvPicker.setCenter(Framework::getScreenCenter());
		m_hsvPicker.setZIndex(1);
		//m_hsvPicker.show();

		m_checkBox.setOrigin({ 600.0f, 450.0f });
		m_checkBox.setOnValueCallback([this](IValueHolder<bool>* sender)
		{
			if (sender->getValue())
				m_hsvPicker.show();
		});

		m_tabView.setOrigin(PointF(40.0f));
		m_tabView.setDim(PointF(500.0f, 550.0f));
		m_tabView.setZIndex(100);
		m_tabView.addTab(&m_containerMaterial, "Material");
		//m_containerMaterial.orderItems();
		m_tabView.addTab(&m_containerMaterial2, "Material2");
		//m_containerMaterial2.orderItems();
		m_tabView.show();

		m_uiList.sort();
		m_uiList.registerAll(this);
	}

	virtual ~StateDev() override
	{}

	virtual bool update(float dt) override
	{
		if (m_btnBack.isClicked(true))
			setNextState(TransitionState::Discard);

		if (m_btnDlg.isClicked(true))
			m_dlgTest.show();

		if (m_btnMsgBox.isClicked(true))
			//m_msgBox.show("The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the lazy dog.", UIMessageBox::Buttons::OKCancel);
			m_msgBox.show();

		std::string msgBoxRes;
		switch(m_msgBox.getResult())
		{
		case UIDialog::Result::None: msgBoxRes = "None"; break;
		case UIDialog::Result::OK: msgBoxRes = "OK"; break;
		case UIDialog::Result::Cancel: msgBoxRes = "Cancel"; break;
		case UIDialog::Result::Yes: msgBoxRes = "Yes"; break;
		case UIDialog::Result::No: msgBoxRes = "No"; break;
		case UIDialog::Result::Retry: msgBoxRes = "Retry"; break;
		case UIDialog::Result::Ignore: msgBoxRes = "Ignore"; break;
		}
		m_msgBoxResult.setText("MsgBox result: " + msgBoxRes);

		return false;
	}

	virtual void composeFrame(Drawing& draw, float dt) override
	{
		/*Drawing::getFont(Font::Style::Headline, Font::Size::S).write(draw, "ghi\njkl", { 800.0f, 800.0f });
		m_lblHeadS.draw(draw);
		m_lblHeadM.draw(draw);
		m_lblHeadL.draw(draw);
		m_lblTextS.draw(draw);
		m_lblTextM.draw(draw);
		m_lblTextL.draw(draw);
		m_txtBox.draw(draw);
		m_msgBoxResult.draw(draw);

		m_btnBack.draw(draw);
		m_btnDlg.draw(draw);
		m_btnMsgBox.draw(draw);
		m_dlgTest.draw(draw);
		m_msgBox.draw(draw);
		m_hsvPicker.draw(draw);*/

		m_uiList.draw(draw);
	}

	// Input handling
	virtual bool keyDown(SDL_Scancode s) override
	{
		GameState::keyDown(s);
		return true;
	}

	virtual bool keyUp(SDL_Scancode s) override
	{
		GameState::keyUp(s);
		return true;
	}

	virtual bool charDown(char c) override
	{
		GameState::charDown(c);
		return true;
	}

	virtual bool mouseMove(const PointF& mpos, const PointF& mdiff, bool handled) override
	{
		GameState::mouseMove(mpos, mdiff, handled);
		return true;
	}

	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override
	{
		GameState::mouseDown(mpos, button);
		return true;
	}

	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override
	{
		GameState::mouseUp(mpos, button);
		return true;
	}

	virtual bool wheel(const PointF& mpos, float amount) override
	{
		GameState::wheel(mpos, amount);
		return true;
	}

private:
	UIObjectList m_uiList;
	UILabel m_lblHeadS;
	UILabel m_lblHeadM;
	UILabel m_lblHeadL;
	UILabel m_lblTextS;
	UILabel m_lblTextM;
	UILabel m_lblTextL;
	UIButtonText m_btnBack;
	UIButtonText m_btnDlg;
	UIButtonText m_btnMsgBox;
	UIDialog m_dlgTest;
	UIMessageBox m_msgBox;
	UITextBox m_txtBox;
	UILabel m_msgBoxResult;
	UIDialogColorPicker m_hsvPicker;
	UICheckBox m_checkBox;
	UITabView m_tabView;
	UIContainerMaterial m_containerMaterial;
	UIContainerMaterial m_containerMaterial2;
};
