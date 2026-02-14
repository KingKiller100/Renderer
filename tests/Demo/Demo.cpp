module Demo;

Demo::Demo(std::unique_ptr<Gfx::iGfxDevice> gfxDevice)
	: Gfx(std::move(gfxDevice))
	, Running(true)
{}

bool Demo::IsRunning()
{
	return Running;
}

void Demo::Load()
{
	Canvas = Gfx->CreateCanvas({
		.Title = "Demo"
		, .Size = {800, 600}
		});
}

void Demo::Update()
{
	Gfx->BeginDraw();
	Gfx->EndDraw();
	Canvas->Process();
}
