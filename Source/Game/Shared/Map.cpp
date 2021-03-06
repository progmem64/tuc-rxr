#include "Map.h"
#include <assert.h>
#include "../../System/Exceptions/Exception.h"

Map::Map(GameManager& m)
	:
	m_dim(Point3S(0, 0, 0)),
	m_manager(m)
{}

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
	m_volumeTextureMap.setValue(pos, cd.shader == CubeShader::Default ? 1.0f : 0.5f);

	// find chunk
	PointS chunk = PointS(pos.x, pos.y) / MapChunk::SIZE;
	assert(chunk.y * m_cdim.x + chunk.x < m_chunks.size());
	pos.x -= chunk.x * MapChunk::SIZE;
	pos.y -= chunk.y * MapChunk::SIZE;
	m_chunks[chunk.y * m_cdim.x + chunk.x]->setCube(pos, &cd);
}

void Map::destroyBlock(Point3S pos)
{
	// TODO implement ;)
	assert(pos.z < DEPTH);
	if (pos.z >= DEPTH)
		return;
	// construct cube
	// at the moment just default cubes
	m_volumeTextureMap.setValue(pos, 0.0f);

	// find chunk
	PointS chunk = PointS(pos.x, pos.y) / MapChunk::SIZE;
	assert(chunk.y * m_cdim.x + chunk.x < m_chunks.size());
	pos.x -= chunk.x * MapChunk::SIZE;
	pos.y -= chunk.y * MapChunk::SIZE;
	m_chunks[chunk.y * m_cdim.x + chunk.x]->setCube(pos, nullptr);
}

void Map::bind(Drawing& draw)
{
	LockGuard g(m_muMap);
	draw.setMapInfo(m_dim);
	// enable volume map
	if (!m_volumeTextureMap.isCreated())
		m_volumeTextureMap.create();

	m_volumeTextureMap.bind(0);
}

void Map::setDim(Point3S dim)
{
	assert(dim.z == DEPTH);
	// allocate / deallocate chunks
	size_t nx = (dim.x + MapChunk::SIZE - 1) / MapChunk::SIZE;
	size_t ny = (dim.y + MapChunk::SIZE - 1) / MapChunk::SIZE;

	if (nx == m_cdim.x && ny == m_cdim.y)
		return;

	decltype(m_chunks) newChunks;
	newChunks.reserve(nx * ny);

	LockGuard g(m_muMap);
	for (size_t y = 0; y < ny; y++)
	{
		for (size_t x = 0; x < nx; x++)
		{
			if(x < m_cdim.x && y < m_cdim.y)
			{
				// use old chunk
				newChunks.push_back(move(m_chunks.at(y * m_cdim.x + x)));
				newChunks.back()->setChunkPosition(Point3S(x, y, 0)); // TODO the position didnt change??
			}
			else
			{
				// new chunk entity
				auto e = m_manager.addEntity();
				// attach script, the sript will add all components on begin
				auto script = std::make_shared<MapChunk>(Point3S(x, y, 0));
				e->addScript(script);
				newChunks.emplace_back(script);
			}
		}
	}
	// kill remaining chunks
	for (auto& c : m_chunks)
		if (c)
			c->kill();

	// set neighbors
	m_cdim.x = nx;
	m_cdim.y = ny;
	m_chunks = move(newChunks);
	for (size_t y = 0; y < ny; y++)
	{
		for (size_t x = 0; x < nx; x++)
		{
			MapChunk* left = nullptr;
			MapChunk* right = nullptr;
			MapChunk* top = nullptr;
			MapChunk* bottom = nullptr;
			if(x < nx -1)
				right = m_chunks[m_cdim.calcIndex({ x + 1,y })].get();
			if (x > 0)
				left = m_chunks[m_cdim.calcIndex({ x - 1,y })].get();
			if (y < ny - 1)
				bottom = m_chunks[m_cdim.calcIndex({ x,y+1 })].get();
			if (y > 0)
				top = m_chunks[m_cdim.calcIndex({ x,y-1 })].get();
			m_chunks[m_cdim.calcIndex({ x,y })]->setNeighbors(left, right, top, bottom);
		}
	}
	// TODO add case if neighbors were removed
	dim.x = nx * MapChunk::SIZE;
	dim.y = ny * MapChunk::SIZE;
	dim.z = DEPTH; // use depth 16 for shadow map
	m_volumeTextureMap.resize(dim);
	m_dim = dim;
	g.unlock();

	// this is needed to initialize the components.
	// since resizing will only occur in the editor and not in the game
	// it is okay do do it here
	m_manager.tick(0.0f);
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
		d.push_back(c->getCubes());
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
		m_chunks[m_cdim.calcIndex(pos)]->loadChunk(c);
		// setting shadows
		for(const auto& b : c)
		{
			PointS off = pos * MapChunk::SIZE;
			m_volumeTextureMap.setValue(Point3S(off.x, off.y, 0) + b.first, b.second.shader == CubeShader::Default? 1.0f : 0.5f);
		}
		idx++;
	}

	/*for(const auto& ass : i.assets)
	{
		if(ass.instances.size())
		{
			MapAsset a(m_manager);
			a.loadChunk(ass.geometry);
			for (const auto& in : ass.instances)
				a.addInstance(in.pos, in.theta, in.phi, in.scale);
			a.setAnimation(ass.animation);

			m_assets.push_back(std::move(a));
		}
	}*/
}

void Map::update()
{

}

void Map::dispose()
{
	for (auto& c : m_chunks)
		c->kill();
}
