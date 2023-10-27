#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
	std::cout << message << std::endl << std::endl;
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
		float const aspectRatio = static_cast<float>(width) / height;

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

		float const vertices[] =
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
		VertexBuffer vb(vertices, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.SetBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 mvp = projection * view * model;

		Shader shader("Resources/Shaders/Basic.vert", "Resources/Shaders/Basic.frag");
		shader.Bind();

		Texture texture("Resources/Textures/Grass_Side_16.png");
		texture.Bind();
		shader.SetUniform1i(2, 0);
		shader.SetUniformMat4f(3, mvp);

		Renderer renderer;

		ImGui::CreateContext();
		ImGuiIO& imGuiIO = ImGui::GetIO();
		imGuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
		ImGui::StyleColorsDark();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(window, true);

		float x = 0.0f;
		float y = 0.0f;

		while (!glfwWindowShouldClose(window))
		{
			view = glm::translate(glm::mat4(1.0f), glm::vec3(x * (width - 100), y * (height - 100), 0.0f));
			glm::mat4 mvp = projection * view * model;
			shader.SetUniformMat4f(3, mvp);

			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//shader.SetUniform4f(1, 0.0f, 0.0f, 1.0f, 1.0f);
			renderer.Draw(va, ib, shader);

			{
				ImGui::Begin("Hello, world!");
				ImGui::SliderFloat("X", &x, 0.0f, 1.0f);
				ImGui::SliderFloat("Y", &y, 0.0f, 1.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / imGuiIO.Framerate, imGuiIO.Framerate);
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);

			glfwPollEvents();
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwTerminate();
	}
	catch (Error const& e)
	{
		std::cout << e.GetMessage() << std::endl;
		return e.GetErrorType();
	}

	return 0;
}