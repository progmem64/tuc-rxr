#pragma once
#include "../../ecs/entitycs.h"
#include <glm/glm.hpp>
#include "CubeDesc.h"
#include "../../Framework/OpenGL/Shader/InstancingArray.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../Utility/AABox.h"

using HealthT = int_fast32_t;
using ColorT = uint32_t;

struct Transform
{
	glm::vec3 pos;
	glm::vec3 scale;
	glm::vec2 rotation;

	// TODO scale add rotaion
	glm::mat4 getMatrix() const
	{
		return glm::scale(glm::translate(glm::mat4(), pos),scale);
	}
};

struct Movement
{
	glm::vec3 velocity;
	glm::vec3 acceleration;
};

struct Collision
{
	// axis aligned box
	AABox aabox;
	// bounce factor when hitting other object [0.0f,-1.0f]
	float bounce = -1.0f;
};

// this objects cant move
struct StaticCollision
{
	// axis aligned box
	AABox aabox;
};

struct Health
{
	HealthT hp;
	HealthT armor;
};

struct Damage
{
	HealthT truedmg;
	HealthT magicmg;
	HealthT physicaldmg;
};

struct CubeShape
{
	ColorT diffuse;
	ColorT specular;
	float gloss;
	uint_least8_t flags;
	CubeShader shader;
	BlockType type;
};

struct MapChunkInfo
{
	uint_least8_t neighbors;
	enum Side
	{
		Left = 1,
		Right = 2,
		Top = 4,
		Bottom = 8,
		Front = 16,
		Back = 32
	};
};

struct AssetChunkInfo
{
	size_t assetID;
};

struct ChunkData
{
	// pretend that this is a float because opengl would convert it otherwise...
	// draw access -> draw thread | setData access -> main thread
	using GpuArray = InstancingArray<glm::ivec3, 3, GL_FLOAT>;
	std::unique_ptr<GpuArray> m_iArray;
	std::unique_ptr<GpuArray> m_iTransArray;
};

using GameManager =	ecs::Manager<Transform, Movement, Health, Damage, CubeShape, MapChunkInfo, ChunkData, AssetChunkInfo, Collision, StaticCollision>;
using GameEntity =	ecs::Entity< Transform, Movement, Health, Damage, CubeShape, MapChunkInfo, ChunkData, AssetChunkInfo, Collision, StaticCollision>;
using GameSystem =	ecs::System< Transform, Movement, Health, Damage, CubeShape, MapChunkInfo, ChunkData, AssetChunkInfo, Collision, StaticCollision>;
using GameScript =	ecs::Script< Transform, Movement, Health, Damage, CubeShape, MapChunkInfo, ChunkData, AssetChunkInfo, Collision, StaticCollision>;