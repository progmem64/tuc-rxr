#include "Cube.h"
#include "../../Framework/OpenGL/Shader/Shader.h"
#include "../../Framework/OpenGL/Shader/ShaderCubeMap.h"
#include <glm/gtc/matrix_transform.hpp>

Cube::Cube(const CubeDesc& desc, const glm::vec3& pos, bool isMapCube, float scalar)
	:
	m_desc(desc),
	m_diffuse(desc.diffuse),
	m_specular(desc.spec),
	m_pos(pos),
	m_rot(0.0f, 0.0f, 0.0f),
	m_scalar(scalar * 0.5f + 0.001f),
	m_isMapCube(isMapCube)
{
	recalcMatrix();
}

Cube::~Cube()
{
}

void Cube::setPos(const glm::vec3& pos)
{
	m_pos = pos;
	recalcMatrix();
}

void Cube::setRot(const glm::vec3& rot)
{
	m_rot = rot;
	recalcMatrix();
}

const glm::vec3& Cube::getPos() const
{
	return m_pos;
}

const glm::vec3& Cube::getRot() const
{
	return  m_rot;
}

const glm::mat4& Cube::getTransform() const
{
	return m_matTrans;
}

#ifdef _CLIENT
void Cube::draw(Drawing& draw)
{
	Shader* shader = nullptr;
	if (m_isMapCube)
		shader = &(draw.getShaderCubeMap());
	else
		shader = &(draw.getShaderCubeDefault());

	draw.setCubeMaterial(m_diffuse, m_specular, m_desc.gloss);

	draw.shaderedCube(m_matTrans, *shader);
}

CubeDesc Cube::getDesc() const
{
	return m_desc;
}
#endif // _CLIENT

void Cube::recalcMatrix()
{
	m_matTrans = glm::translate(glm::mat4(),m_pos);
	m_matTrans *= glm::scale(glm::mat4(),glm::vec3(m_scalar));
	if (m_rot.x != 0.0f)
		m_matTrans *= glm::rotate(glm::mat4(),m_rot.x, glm::vec3(1.0f, 0.0f, 0.0f));

	if (m_rot.y != 0.0f)
		m_matTrans *= glm::rotate(glm::mat4(), m_rot.y, glm::vec3(0.0f, 1.0f, 0.0f));

	if (m_rot.z != 0.0f)
		m_matTrans *= glm::rotate(glm::mat4(), m_rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
}
