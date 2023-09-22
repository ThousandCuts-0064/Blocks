#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Error.hpp"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

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

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		GLFWwindow* window = glfwCreateWindow(640, 480, "Blocks", nullptr, nullptr);
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
		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);

		std::cout << "Version:" << glGetString(GL_VERSION) << std::endl << std::endl;

		float positions[] =
		{
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f,
		};

		unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 0,
		};

		VertexArray va;
		VertexBuffer vb(positions, 8 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.SetBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		glBindVertexArray(GL_ZERO);
		glBindBuffer(GL_ARRAY_BUFFER, GL_ZERO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_ZERO);


		Shader shader("Resources/Shaders/Basic.vert", "Resources/Shaders/Basic.frag");

		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);

			shader.SetUniform4f(1, 0.0f, 0.0f, 1.0f, 1.0f);

			glfwSwapBuffers(window);

			glfwPollEvents();
		}

		glfwTerminate();
	}
	catch (Error& e)
	{
		std::cout << e.GetMessage() << std::endl;
		return e.GetErrorType();
	}

	return 0;
}