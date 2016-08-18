#pragma once
#include "../../Game/Shared/CubeDesc.h"
#include "UINumUpDownEnum.h"

class UINumUpDownCubeShader : public UINumUpDownEnum<CubeShader>
{
public:

	UINumUpDownCubeShader(Font& font, CubeShader start, CubeShader min, CubeShader max, CubeShader step)
		: UINumUpDownEnum<CubeShader>(font, start, min, max)
	{
		UINumUpDown<CubeShader>::setValue(start);
	}

protected:
	virtual std::string numToSting(CubeShader n) const override
	{
		switch (n)
		{
		case CubeShader::Default: return "default";
		case CubeShader::Special: return "special";
		default:
			return "invalid";
		}
	}

	virtual CubeShader stringToNum(const std::string& s) const override
	{
		if (s == "default")
			return CubeShader::Default;
		if (s == "special")
			return CubeShader::Special;

		return CubeShader::Default;
	}
};
