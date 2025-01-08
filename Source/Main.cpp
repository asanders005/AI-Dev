#include "CAScene.h"
#include <memory>
#include <SDL.h>

int main(int, char**)
{
	std::unique_ptr<CAScene> scene = std::make_unique<CAScene>();

	scene->Initialize();
	while (!scene->IsQuit())
	{
		scene->Update();
		scene->Draw();
	}

	return 0;
}