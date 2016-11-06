#include "Map.h"
#include <assert.h>
#include "../../System/Exceptions/Exception.h"

Map::Map(Point3S dim)
	:
	m_dim(Point3S(0, 0, 0))
{
	setDim(dim);
}

Map::~Map()
{
}

void Map::setCube(Point3S pos, const CubeDesc& cd)
{
	assert(pos.z < DEPTH);
	if (pos.z >= DEPTH)
		return;
	// construct cube
	// at the moment just default cubes
	auto pCube = std::unique_ptr<CubeBase>(new CubeBase(cd));
	m_volumeTextureMap.setValue(pos, pCube->getShadow());
	setCube(pos, move(pCube));
}

void Map::setCube(Point3S pos, std::unique_ptr<CubeBase> c)
{
	// find chunk
	PointS chunk = PointS(pos.x, pos.y) / MapChunk::SIZE;
	assert(chunk.y * m_cdim.x + chunk.x < m_chunks.size());
	pos.x -= chunk.x * MapChunk::SIZE;
	pos.y -= chunk.y * MapChunk::SIZE;
	m_chunks[chunk.y * m_cdim.x + chunk.x].setCube(pos, move(c));
}

void Map::destroyBlock(const Point3S& pos)
{
	setCube(pos, nullptr);
	m_volumeTextureMap.setValue(pos, 0.0f);
}

void Map::draw(Drawing& draw)
{
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_DST_ALPHA, GL_ONE, GL_DST_ALPHA, GL_ZERO);
	LockGuard g(m_muMap);
	draw.setMapInfo(m_dim);
	// enable volume map
	if (!m_volumeTextureMap.isCreated())
		m_volumeTextureMap.create();

	auto& shader = draw.getShaderCubeMap();

	auto& meshCube = draw.getCubeMesh();

	shader.bind();
	m_volumeTextureMap.bind(0);
	//glTexSubImage3D()
	glm::mat4 transform;
	// TODO optimize draw range
	for(size_t y = 0; y < m_cdim.y; y++)
	{
		for(size_t x = 0; x < m_cdim.x; x++)
		{
			draw.getTransform().pushModel(transform);
			draw.getTransform().flush();
			m_chunks[y * m_cdim.x + x].draw(draw, meshCube);
			draw.getTransform().popModel();
			transform = glm::translate(transform,glm::vec3(float(MapChunk::SIZE), 0.0f, 0.0f));
		}
		transform = glm::translate(glm::vec3(0.0f, float(MapChunk::SIZE * (y+1)), 0.0f));
	}
	glDisable(GL_BLEND);
	// draw assets
	// TODO seperate lock?
	for(auto& a : m_assets)
	{
		a.draw(draw, meshCube, shader);
	}

	g.unlock();
	shader.unbind();
}

void Map::setDim(Point3S dim)
{
	assert(dim.z == DEPTH);
	// TODO lock mutex
	// allocate / deallocate chunks
	size_t nx = (dim.x + MapChunk::SIZE - 1) / MapChunk::SIZE;
	size_t ny = (dim.y + MapChunk::SIZE - 1) / MapChunk::SIZE;

	if (nx == m_cdim.x && ny == m_cdim.y)
		return;

	std::vector<MapChunk> newChunks;
	newChunks.reserve(nx * ny);

	LockGuard g(m_muMap);
	for (size_t y = 0; y < ny; y++)
	{
		for (size_t x = 0; x < nx; x++)
		{
			if(x < m_cdim.x && y < m_cdim.y)
			{
				// use old chunk
				newChunks.push_back(std::move(m_chunks.at(y * m_cdim.x + x)));
			}
			else newChunks.emplace_back();
		}
	}

	m_cdim.x = nx;
	m_cdim.y = ny;
	dim.x = nx * MapChunk::SIZE;
	dim.y = ny * MapChunk::SIZE;
	m_chunks = move(newChunks);
	dim.z = DEPTH; // use depth 16 for shadow map
	m_volumeTextureMap.resize(dim);
	m_dim = dim;
	g.unlock();
}

Point3S Map::getDim() const
{
	return m_dim;
}

std::vector<std::vector<std::pair<Point3S, CubeDesc>>> Map::getCubeInfos() const
{
	std::vector<std::vector<std::pair<Point3S, CubeDesc>>> d;
	for(const auto& c : m_chunks)
	{
		d.push_back(c.getCubes());
	}
	return d;
}

PointS Map::getChunkSize() const
{
	return m_cdim;
}

void Map::loadMapAndAssets(const MapLoader::MapInfo& i)
{
	// load chunks
	size_t idx = 0;
	for(const auto& c : i.chunkCubes)
	{
		PointS pos = i.nChunks.fromIndex(idx);
		m_chunks[m_cdim.calcIndex(pos)].loadChunk(c);
		// setting shadows
		for(const auto& b : c)
		{
			PointS off = pos * MapChunk::SIZE;
			m_volumeTextureMap.setValue(Point3S(off.x, off.y, 0) + b.first, CubeBase(b.second).getShadow());
		}
		idx++;
	}

	for(const auto& ass : i.assets)
	{
		if(ass.instances.size())
		{
			MapAsset a;
			a.loadChunk(ass.geometry);
			for (const auto& in : ass.instances)
				a.addInstance(in.pos, in.theta, in.phi, in.scale);
			a.setAnimation(ass.animation);

			m_assets.push_back(std::move(a));
		}
	}
}
