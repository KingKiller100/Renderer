module;
#include <glad/glad.h>
#include <GLFW/glfw3.h>
export module Gfx:OpenGl;
import Core;
import <memory>;
import <vector>;
import <unordered_map>;

namespace Gfx
{
	export class OpenGLDevice;
	export class OpenGLCanvas;

	namespace
	{
		std::unordered_map<GLFWwindow*, std::weak_ptr<OpenGLCanvas>> GlobalCanvasRegister;
	}

	export class OpenGLCanvas : public iCanvas
	{
		friend OpenGLDevice;
	public:
		OpenGLCanvas(Options opts)
			: Opts(std::move(opts))
			, Window(nullptr)
		{}
		bool Init()
		{
			const auto& [title, position, size] = Opts;
			if (Window = glfwCreateWindow(size.X, size.Y, Opts.Title.data(), nullptr, nullptr))
			{
				glfwMakeContextCurrent(Window);
				glViewport(position.X, position.Y, size.X, size.Y);

				glfwSetWindowPosCallback(Window, [](GLFWwindow* window, int x, int y)
				{
					if (auto canvas = GlobalCanvasRegister.at(window).lock())
					{
						const auto size = canvas->Opts.Size;
						glViewport(x, y, size.X, size.Y);
					}
				});

				glfwSetFramebufferSizeCallback(Window, [](GLFWwindow* window, int width, int height)
				{
					if (auto canvas = GlobalCanvasRegister.at(window).lock())
					{
						const auto pos = canvas->Opts.Position;
						glViewport(pos.X, pos.Y, width, height);
					}
				});

				return true;
			}
			return false;
		}
		void Process() override
		{
			glfwSwapBuffers(Window);
			glfwPollEvents();
		}

	private:
		Options Opts;
		GLFWwindow* Window;
	};

	export class OpenGLDevice : public iGfxDevice
	{
	public:
		struct Config
		{};

	public:
		static std::unique_ptr<iGfxDevice> OpenGLDevice::Create(Config config)
		{
			std::unique_ptr<OpenGLDevice> self{ new OpenGLDevice{std::move(config)} };
			self->Init();
			return self;
		}

		~OpenGLDevice() override
		{
			if (Initialized)
				glfwTerminate();
		}

		bool Init()
		{
			glfwInit();

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				return false;
			}

#ifdef __APPLE
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

			Initialized = true;

			return true;
		}

		std::shared_ptr<iCanvas> CreateCanvas(iCanvas::Options opts) override
		{
			auto canvas = std::make_shared<OpenGLCanvas>(opts);
			canvas->Init();
			Canvases.emplace_back(canvas);
			GlobalCanvasRegister.emplace(canvas->Window, canvas);
			return canvas;
		}

		bool BeginDraw() override { return false; }
		void EndDraw() override {}
		void Present() override {}

	private:
		OpenGLDevice(Config config);

	private:
		std::vector<std::weak_ptr<iCanvas>> Canvases;
		bool Initialized;
	};
}
