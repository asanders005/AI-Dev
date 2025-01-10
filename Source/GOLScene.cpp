#include "GOLScene.h"
#include "Random.h"

const color_t white{ 255, 255, 255, 255 };
const color_t black{ 0, 0, 0, 255 };

bool GOLScene::Initialize()
{
	m_renderer.Initialize();
	m_renderer.CreateWindow("Game of Life", 800, 600);

	m_input.Initialize();
	m_input.Update();

	m_framebuffer = std::make_unique<Framebuffer>(m_renderer, m_renderer.m_width / 2, m_renderer.m_height / 2);

	m_cellsA = std::make_unique<Cells<uint8_t>>(m_framebuffer->m_width, m_framebuffer->m_height);
	m_cellsB = std::make_unique<Cells<uint8_t>>(m_framebuffer->m_width, m_framebuffer->m_height);

	for (int i = 0; i < m_cellsA->m_data.size(); i++)
	{
		if (random(10) > 8)
		{
			m_cellsA->m_data[i] = 1;
		}
	}

	return true;
}

void GOLScene::Update()
{
	Scene::Update();
	frameCount++;

	Cells<uint8_t>* currentCells = (frameCount & 1) ? m_cellsA.get() : m_cellsB.get();
	Cells<uint8_t>* nextCells = (frameCount & 1) ? m_cellsB.get() : m_cellsA.get();

	std::fill(nextCells->m_data.begin(), nextCells->m_data.end(), 0);

	if (m_input.GetKeyDown(SDL_SCANCODE_SPACE))
	{
		for (int i = 0; i < 100; i++)
		{
			nextCells->Write(random(nextCells->m_width - 2) + 1, random(nextCells->m_height - 2) + 1, 1);
		}
	}

	for (int y = 1; y < currentCells->m_height - 1; y++)
	{
		for (int x = 1; x < currentCells->m_width - 1; x++)
		{
			int count = 0;

			count += currentCells->Read(x - 1, y - 1);
			count += currentCells->Read(x, y - 1);
			count += currentCells->Read(x + 1, y - 1);
			count += currentCells->Read(x - 1, y);
			count += currentCells->Read(x + 1, y);
			count += currentCells->Read(x - 1, y + 1);
			count += currentCells->Read(x, y + 1);
			count += currentCells->Read(x + 1, y + 1);

			uint8_t currentState = currentCells->Read(x, y);
			if (currentState)
			{
				nextCells->Write(x, y, (count == 2 || count == 3) ? 1 : 0);
			}
			else
			{
				if (count == 3)
				{
					nextCells->Write(x, y, 1);
				}
			}
		}
	}
	
	// Write cells to framebuffer
	m_framebuffer->Clear(color_t{ 0, 0, 0, 255 });
	for (int i = 0; i < nextCells->m_data.size(); i++)
	{
		m_framebuffer->m_buffer[i] = (nextCells->m_data[i]) ? white : black;
	}
}

void GOLScene::Draw()
{
	// update framebuffer
	m_framebuffer->Update();

	// show screen
	m_renderer = *m_framebuffer;
	SDL_RenderPresent(m_renderer.m_renderer);
}
