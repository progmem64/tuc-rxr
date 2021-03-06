#pragma once
#include <string>

#include "../GLObject.h"
#include "../../../glew/glew.h"
#include "../../../System/Exceptions/GL_Exception.h"

class Shader : public GLObject
{
	friend class UniformBlock;

	enum class Type
	{
		Vertex,
		Fragment,

		Size
	};
public:
	class ILoadable
	{
	public:
		virtual ~ILoadable(){}
		virtual void load() = 0;
	};

public:
	Shader();
	virtual void create() override;
	virtual void dispose() override;
	void bind() const;
	void unbind() const;

protected:
	void load(const std::string& file);
	static std::string loadShader(const std::string& file);
	static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint createShader(Type ty, GLenum shadertype);

	void loadShaderSource(const std::string& filename);
	virtual void loadUniforms(){}
	GLint locateUniform(const char* name) const;
	GLint tryLocateUniform(const char* name) const;
protected:
	GLuint m_program = 0;

private:
	GLuint m_shaders[size_t(Type::Size)];

	std::string m_sources[size_t(Type::Size)];
	std::string m_filename;

	bool m_isLoaded = false;
};