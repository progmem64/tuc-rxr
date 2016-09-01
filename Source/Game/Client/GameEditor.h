#pragma once
#include "../../Framework/UI/UIObject.h"
#include "../Shared/Map.h"
#include "LightManager.h"
#include "../../System/Input.h"
#include "../../Framework/UI/Callback.h"

class GameEditor : public IDrawable, public Input::IReceiver
{
	CALLBACK(Capture, bool);
public:
	GameEditor();
	virtual ~GameEditor() override;
	virtual void draw(Drawing& draw) override;

	virtual bool mouseMove(const PointF& mpos, const PointF& mdiff, bool handled) override;
	virtual bool mouseDown(const PointF& mpos, Input::Mouse button) override;
	virtual bool mouseUp(const PointF& mpos, Input::Mouse button) override;
	virtual bool wheel(const PointF& mpos, float amount) override;
	virtual bool keyDown(SDL_Scancode s) override;
	virtual bool keyUp(SDL_Scancode s) override;

	void setCurrentBlock(const CubeDesc& c);
	void reset();
private:
	void drawGrid(Drawing& draw) const;
	void drawLineBox(Drawing& draw, const Point3F& p1, const Point3F& p2, const Color& c) const;
	void releaseCapture();
	void takeCapture();
private:
	std::unique_ptr<Map> m_pMap;
	Mutex m_muMap;

	std::unique_ptr<Camera> m_pCam;
	std::unique_ptr<LightManager> m_pLight;
	bool m_hasCapture = false;
	bool m_isCaptureTemp = false;
	bool m_hover = false;
	bool m_setDown = false;
	bool m_eraseDown = false;
	bool m_dragDown = false;

	// mouse positioning
	PointF m_prevClientMouse;
	PointF m_mouseDiff;
	float m_wheelDiff = 0.0f;
	Point3I m_blockPos;
	Point3I m_downPos;

	CubeDesc m_curCubeDesc;
};
