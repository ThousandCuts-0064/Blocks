#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.hpp"
#include "Error.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

static void GLDebugMessageHandle(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* userParam)
{
	std::cout << message << std::endl;
}

int main(void)
{
	try
	{
		if (glfwInit() == GLFW_FALSE)
			throw Error(GlfwInitFail);

		GLFWvidmode const* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int const width = mode->width;
		int const height = mode->height;
		float const aspectRatio = static_cast<float>(width) /height;

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		GLFWwindow* const window = glfwCreateWindow(width, height, "Blocks", nullptr, nullptr);
		if (!window)
		{
			glfwTerminate();
			throw Error(GlfwCreateWindowFail);
		}

		glfwMakeContextCurrent(window);
		glfwMaximizeWindow(window);
		glfwSwapInterval(1);

		if (glewInit() != GLEW_OK)
			throw Error(GlewInitFail);

		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugMessageHandle, nullptr);
		GLuint const unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		std::cout << "Version:" << glGetString(GL_VERSION) << std::endl << std::endl;

		float const positions[] =
		{
			 000.0f,  000.0f,   0.0f, 0.0f,
			 100.0f,  000.0f,   1.0f, 0.0f,
			 100.0f,  100.0f,   1.0f, 1.0f,
			 000.0f,  100.0f,   0.0f, 1.0f,
		};

		unsigned const int indices[] =
		{
			0, 1, 2,
			2, 3, 0,
		};

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.SetBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(1000.0f, 0.0f, 0.0f));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 500.0f, 0.0f));
		glm::mat4 mvp = projection * view * model;

		Shader shader("Resources/Shaders/Basic.vert", "Resources/Shaders/Basic.frag");
		shader.Bind();

		Texture texture("Resources/Textures/Grass_Side_16.png");
		texture.Bind();
		shader.SetUniform1i(2, 0);
		shader.SetUniformMat4f(3, mvp);

		std::cout << glGetUniformLocation(1, "u_mvp") << std::endl;

		Renderer renderer;

		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();
			shader.SetUniform4f(1, 0.0f, 0.0f, 1.0f, 1.0f);
			renderer.Draw(va, ib, shader);

			glfwSwapBuffers(window);

			glfwPollEvents();
		}

		glfwTerminate();
	}
	catch (Error const& e)
	{
		std::cout << e.GetMessage() << std::endl;
		return e.GetErrorType();
	}

	return 0;
}