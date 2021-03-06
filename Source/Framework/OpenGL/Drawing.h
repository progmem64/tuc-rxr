#pragma once
#include "GLObject.h"
#include "../../Utility/Rect.h"
#include "../Color.h"
#include "Geometry/MeshCube.h"
#include "Geometry/MeshLineBox.h"
#include "Shader/ShaderCubeMap.h"
#include "Shader/ShaderButton.h"
#include "Font.h"
#include "Shader/UniformBlockTransforms.h"
#include "Shader/UniformBlockMaterial.h"
#include "Shader/UniformBlockLight.h"
#include "Camera.h"
#include "Shader/UniformBlockMapInfo.h"
#include "../../Utility/Point3.h"
#include "VolumeTextureMap.h"
#include "Texture.h"
#include "Shader/UniformBlockFramework.h"
#include "Shader/ShaderHSVPickerCircle.h"
#include "Shader/ShaderColor.h"
#include "Shader/ShaderColor2.h"
#include "Shader/ShaderCube.h"
#include "Shader/ShaderHSVPickerSquare.h"
#include "Shader/ShaderDisk.h"
#include "glFramebufferObject.h"
#include "Shader/UniformBlockFramebuffer.h"
#include "Shader/ShaderFXAA.h"
#include "Shader/ShaderBloom1.h"
#include "Shader/ShaderTransEnd.h"
#include "Shader/ShaderCubeTrans.h"

class Drawing : public GLObject
{
	friend class Graphics;
	friend Font; // because flushing transforms + clipping
	Drawing();
public:
	~Drawing();

	virtual void create() override;
	virtual void dispose() override;

	void init(FT_Library ftlib);

	// basic
	void rect(const RectF& r, const Color& c);
	void line(PointF p1, PointF p2, float thickness, const Color& color);
	void line(const glm::vec3& p1, const glm::vec3& p2, float thikness, const Color& c);
	void disc(const PointF& midPos, float r, const Color& color);
	void lineBox(const Point3F& p1, const Point3F& p2, const Color& c);

	// ui
	void buttonRoyal(const RectF& r, bool down);

	void hsvPicker(const PointF& midPos, float r, const Color& color);

	// cubes
	void shaderedCube(const glm::mat4& mat, Shader& shader);
	void setCubeMaterial(const Color& diffuse, const Color& specular, float gloss);
	void setMapInfo(const Point3S& dim);
	void setLights(const Color& ambient, const std::vector<LightSource>& lights, const glm::vec3& eye);

	// camera
	UniformBlockTransforms& getTransform();
	void pushClippingRect(RectF rect);
	void popClippingRect();
	void pushIgnoreRect();

	UniformBlockLight& getLightUniform();

	Camera& getUiCam();
	Texture& getWaterTexture();
	Texture& getWaterfallTexture();

	// shader
	ShaderCubeMap& getShaderCubeMap();
	ShaderCubeTrans& getShaderCubeTrans();
	ShaderCube& getShaderCubeDefault();
	MeshCube& getCubeMesh();

	void beginGameShader();
	void beginGameTransparency();
	void endGameTransparency();
	void endGameShader();

	// static
	static Font& getFont(Font::Style style, Font::Size size);
	static size_t getThreadID();

	// memory
	void addToDisposeStack(gl::Disposeable d);
private:
	static Drawing& get();
	void prepareDraw();
	void beginFrame();
	void endFrame();
	void resize(GLsizei width, GLsizei height);
private:
	Camera m_uiCam;

	MeshCube m_meshCube;
	MeshLineBox m_lineBox;

	ShaderCubeMap m_shCubeMap;
	ShaderCube m_shCube;
	ShaderButton m_shButton;
	ShaderColor m_shColor;
	ShaderColor2 m_shColor2;
	ShaderHSVPickerCircle m_shHSVPickerCircle;
	ShaderHSVPickerSquare m_shHSVPickerSquare;
	ShaderDisk m_shDisc;
	ShaderFXAA m_shFxaa;
	ShaderBloom1 m_shBloom1;
	ShaderBloom2 m_shBloom2;
	ShaderBloom3 m_shBloom3;
	ShaderTransEnd m_shTransEnd;
	ShaderCubeTrans m_shCubeTrans;

	UniformBlockTransforms m_trans;
	UniformBlockMaterial m_material;
	UniformBlockLight m_lights;
	UniformBlockMapInfo m_mapInfo;
	UniformBlockFramework m_blockFramework;
	UniformBlockFramebuffer m_blockFramebuffer;

	// fonts
	Font m_fontHeadS;
	Font m_fontHeadM;
	Font m_fontHeadL;
	Font m_fontTextS;
	Font m_fontTextM;
	Font m_fontTextL;

	// textures
	Texture m_texBtnSide;
	Texture m_texBtnMid;
	Texture m_texBtnBumpLeft;
	Texture m_texBtnBumpRight;
	Texture m_texBtnBumpMid;
	Texture m_texBtnBumpLeftDown;
	Texture m_texBtnBumpRightDown;
	Texture m_texBtnBumpMidDown;

	// super cool shader textures
	Texture m_texWater;
	Texture m_texWaterfall;

	// init speedup
	const std::vector<Shader*> m_shaders;

	// memory
	Mutex m_muDisposeStack;
	std::stack<gl::Disposeable> m_disposeStack;
	PointS m_resolution;


	FramebufferObject m_fboImagePlusBlur; // render targets: image, to be blurred parts, depth
	FramebufferObject m_fboTransparentAccumulator; //  render targets: accum, revealage, blurAccum, blurReveallage
	FramebufferObject m_fboBlurX; // render targets: blurTex
	FramebufferObject m_fboBlurY; // render targets: blurTex
	FramebufferObject m_fboFinal; // render targets: final image (no fxaa)
	bool m_gameActive = false;
	bool m_transparentActive = false;
};
