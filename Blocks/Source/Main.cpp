#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Error.cpp"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

static void GLDebugMessageHandle(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* userParam)
{
	std::cout << message << std::endl;
}

static const std::string ParseShader(char const* filePath)
{
	std::ifstream file(filePath);

	if (!file)
		throw Error(OpenFileFail, filePath);

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

static unsigned int CompileShader(unsigned int type, std::string const& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_TRUE)
		return id;

	int length;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

	unsigned int messageByteCount = length * sizeof(char);
	char* message = new char[messageByteCount];

	glGetShaderInfoLog(id, length, &length, message);
	throw Error(CompileShaderFail, message);
}

static unsigned int CreateShader(std::string const& vertexShader, std::string const& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader.c_str());
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader.c_str());

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
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

		unsigned int vao; // vertex array object
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		VertexBuffer vb(positions, 8 * sizeof(float));
		IndexBuffer ib(indices, 6);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

		glBindVertexArray(GL_ZERO);
		glBindBuffer(GL_ARRAY_BUFFER, GL_ZERO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_ZERO);

		unsigned int shader = CreateShader(
			ParseShader("Resources/Shaders/Basic.vert"),
			ParseShader("Resources/Shaders/Basic.frag"));

		glUseProgram(shader);

		glUniform4f(1, 0.0f, 0.0f, 1.0f, 1.0f);

		glBindVertexArray(vao);

		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);

			ib.Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			glfwSwapBuffers(window);

			glfwPollEvents();
		}

		glDeleteProgram(shader);

		glfwTerminate();
	}
	catch (Error& e)
	{
		std::cout << e.GetMessage() << std::endl;
		return e.GetErrorType();
	}

	return 0;
}