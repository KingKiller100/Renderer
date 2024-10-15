#include "../Demo/Demo.hpp"
#include "OpenGL/OpenGL_v33.hpp"

int main()
{
	auto gfx = Gfx::OpenGLDevice::Create({});

	auto demo = Demo{ (std::move(gfx)) };

	demo.Load();

	while ( demo.IsRunning() )
	{
		demo.Update();
	}
}
