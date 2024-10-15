#ifdef PLATFORM_WINDOWS
#include "../DisplayWindow.h"
#include "../Logger.h"

#include "../events/WindowClosedEvent.h"
#include "../events/WindowResizedEvent.h"
#include "../events/KeyPressedEvent.h"
#include "../events/KeyReleasedEvent.h"
#include "../events/MouseButtonPressedEvent.h"
#include "../events/MouseButtonReleasedEvent.h"
#include "../events/MouseMovedEvent.h"
#include "../events/MouseScrolledEvent.h"

#include "../layers/ImGuiLayer.h"
#include "graphics/opengl/OpenGLContext.h"

#include "GLFW/glfw3.h"

namespace engine {
	DisplayWindow::DisplayWindow() {
		_eventBus = std::make_unique<EventBus>();
		_layerGroup = std::make_unique<LayerGroup>();
	}

	void DisplayWindow::Init(int width, int height, const char* title) {
		EG_ASSERT(glfwInit() == GLFW_TRUE, "Failed to initialize GLFW");

		GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		EG_ASSERT(window != nullptr, "Failed to create window");

		_graphicsContext = new OpenGLContext(window);
		_graphicsContext->Init();

		glfwMakeContextCurrent(window);

		_window = window;
		_width = width;
		_height = height;

		glfwSetWindowUserPointer(window, this);

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
			DisplayWindow* displayWindow = (DisplayWindow*)glfwGetWindowUserPointer(window);
			displayWindow->GetEventBus()->Publish<WindowClosedEvent>();
		});

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
			DisplayWindow* displayWindow = (DisplayWindow*)glfwGetWindowUserPointer(window);
			displayWindow->GetEventBus()->Publish<WindowResizedEvent>(width, height);
		});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			DisplayWindow* displayWindow = (DisplayWindow*)glfwGetWindowUserPointer(window);
			switch(action) {
				case GLFW_PRESS:
					displayWindow->GetEventBus()->Publish<KeyPressedEvent>(key);
					break;
				case GLFW_RELEASE:
					displayWindow->GetEventBus()->Publish<KeyReleasedEvent>(key);
					break;
			}
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
			DisplayWindow* displayWindow = (DisplayWindow*)glfwGetWindowUserPointer(window);
			switch(action) {
				case GLFW_PRESS:
					displayWindow->GetEventBus()->Publish<MouseButtonPressedEvent>(button);
					break;
				case GLFW_RELEASE:
					displayWindow->GetEventBus()->Publish<MouseButtonReleasedEvent>(button);
					break;
			}
		});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
			DisplayWindow* displayWindow = (DisplayWindow*)glfwGetWindowUserPointer(window);
			displayWindow->GetEventBus()->Publish<MouseMovedEvent>(static_cast<int>(xpos), static_cast<int>(ypos));
		});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
			DisplayWindow* displayWindow = (DisplayWindow*)glfwGetWindowUserPointer(window);
			displayWindow->GetEventBus()->Publish<MouseScrolledEvent>(xoffset, yoffset);
		});
	}

	void DisplayWindow::Render() {
		glClear(GL_COLOR_BUFFER_BIT);

		_layerGroup->Render();

		_graphicsContext->SwapBuffers();
		glfwPollEvents();
	}

	void DisplayWindow::Destroy() {
		GLFWwindow* window = (GLFWwindow*)_window;

		delete _graphicsContext;
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}

#endif

