#pragma once
import Core.Maths;
import <string>;
import <utility>;
import <memory>;

export namespace Gfx
{	
	class iCanvas
	{
	public:
		struct Options
		{
			std::string_view Title;
			Maths::Vector<std::uint32_t,2> Position;
			Maths::Vector<std::uint32_t, 2> Size;
		};

		virtual ~iCanvas() = default;
		virtual void Process() = 0;
	};

	class iGfxDevice
	{
	public:
		virtual ~iGfxDevice() = 0;

		virtual std::shared_ptr<iCanvas> CreateCanvas(iCanvas::Options opts) = 0;
		virtual bool BeginDraw() = 0;
		virtual void EndDraw() = 0;
		virtual void Present() = 0;
	};
}
