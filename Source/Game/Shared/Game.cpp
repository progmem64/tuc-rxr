#include "Game.h"

Game::Game()
{
	m_pMap = std::unique_ptr<Map>(new Map(20, 10, 4));
	for (size_t x = 0; x < m_pMap->getWidth(); ++x)
	{
		for (size_t y = 0; y < 5; ++y)
		{
			for (size_t z = 0; z < m_pMap->getDepth(); ++z)
			{
				m_pMap->setCube(x, y, z, new Cube(CubeDesc(Color::Green().toDWORD()), glm::vec3(x, y, z)));
			}
		}

		for (size_t z = 1; z < m_pMap->getDepth(); ++z)
		{
			m_pMap->setCube(x, 9, z, new Cube(CubeDesc(Color::Green().toDWORD()), glm::vec3(x, 9, z)));
		}
	}
}

Game::~Game()
{
}


#ifdef _CLIENT
void Game::draw(Drawing& draw)
{
	m_pMap->draw(draw);
}
#endif // _CLIENT
