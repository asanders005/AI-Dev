#include "CAScene.h"
#include "Random.h"

color_t white{ 255, 255, 255, 255 };
color_t black{ 0, 0, 0, 255 };

bool CAScene::Initialize()
{
	m_renderer.Initialize();
	m_renderer.CreateWindow("Example", 800, 600);

	m_input.Initialize();
	m_input.Update();

	m_framebuffer = std::make_unique<Framebuffer>(m_renderer, m_renderer.m_width / 2, m_renderer.m_height / 2);

	m_cells = std::make_unique<Cells<bool>>(m_renderer.m_width / 2, m_renderer.m_height / 2);

	return true;
}

void CAScene::Update()
{
	m_time.Tick();
	m_input.Update();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			m_quit = true;
		}
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
		{
			m_quit = true;
		}
	}

	m_cells->Write(m_cells->m_width / 2, 0, 1);

	int rule = 30;
	for (int y = 0; y < m_cells->m_height - 1; y++)
	{
		for (int x = 1; x < m_cells->m_width - 1; x++)
		{
			int i = 0; // 0-7

			i |= m_cells->Read(x - 1, y) << 2;
			i |= m_cells->Read(x, y) << 1;
			i |= m_cells->Read(x + 1, y);

			uint8_t state = (rule & 1 << i);
			m_cells->Write(x, y + 1, state);
		}
	}

	m_framebuffer->Clear(color_t{ 0, 0, 0, 255 });
	for (int i = 0; i < m_cells->m_data.size(); i++)
	{
		m_framebuffer->m_buffer[i] = (m_cells->m_data[i]) ? white : black;
	}
}

void CAScene::Draw()
{
	// update framebuffer
	m_framebuffer->Update();

	// show screen
	m_renderer = *m_framebuffer;
	SDL_RenderPresent(m_renderer.m_renderer);
}
