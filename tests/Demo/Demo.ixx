export module Demo;

import Gfx;
import std.compat;

class Demo
{
public:
	Demo(std::unique_ptr<Gfx::iGfxDevice> gfxDevice);

	bool IsRunning();
	void Load();
	void Update();

private:
	std::unique_ptr<Gfx::iGfxDevice> Gfx;
	std::shared_ptr<Gfx::iCanvas> Canvas;
	bool Running;
};
