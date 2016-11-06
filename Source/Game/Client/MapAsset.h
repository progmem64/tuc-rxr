#pragma once
#include "MapChunk.h"
#include "../Shared/MapLoader.h"

class MapAsset
{
public:
	MOVE_ONLY(MapAsset);
	MapAsset();
	void loadChunk(const std::vector<std::pair<Point3S, CubeDesc>>& cubes);
	void addInstance(glm::vec3 pos, float theta, float phi, float scale);
	void draw(Drawing& draw, Mesh& cube, ShaderCubeMap& shader);
	void setAnimation(AssetAnimation a);
private:
	MapChunk m_chunk;
	std::vector<glm::mat4> m_instRot; // rotation for instance
	AssetAnimation m_animation = AssetAnimation::None;
};
