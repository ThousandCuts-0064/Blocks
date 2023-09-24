#include <glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Shader.hpp"
#include "Error.hpp"

Shader::Shader(std::string const& vertexFilePath, std::string const& fragmentFilePath) :
	_vertexFilePath(vertexFilePath),
	_fragmentFilePath(fragmentFilePath),
	_rendererID(CreateShader(
		ParseShader(_vertexFilePath),
		ParseShader(_fragmentFilePath)))
{}

Shader::~Shader()
{
	glDeleteProgram(_rendererID);
}

void Shader::Bind() const
{
	glUseProgram(_rendererID);
}

void Shader::Unbind() const
{
	glUseProgram(GL_ZERO);
}

void Shader::SetUniform1i(int location, int value) const
{
	glUniform1i(location, value);
}

void Shader::SetUniform4f(int location, float v0, float v1, float v2, float v3) const
{
	glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(int location, glm::mat4 const& matrix) const
{
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

std::string const Shader::ParseShader(std::string const& filePath) const
{
	std::ifstream file(filePath);

	if (!file)
		throw Error(OpenFileFail, filePath);

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

unsigned int Shader::CreateShader(std::string const& vertexShader, std::string const& fragmentShader) const
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

unsigned int Shader::CompileShader(unsigned int type, std::string const& source) const
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
