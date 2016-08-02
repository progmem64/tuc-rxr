#pragma once
#include "Shader.h"
#include "../../../Utility/Point.h"
#include "../../../glm/detail/type_vec3.hpp"

class ShaderButton : public Shader
{
public:
	virtual ~ShaderButton() override;
	virtual void load() override;
	virtual void create() override;
	void setLightPos(const glm::vec3& pos);
protected:
	virtual void loadUniforms() override;
private:
	GLint m_texLoc = -1;
	GLint m_heightLoc = -1;
	GLint m_light = -1;
};
