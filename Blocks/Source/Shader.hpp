#pragma once

#include <glm.hpp>

#include <string>

class Shader
{
private:
	std::string _vertexFilePath;
	std::string _fragmentFilePath;
	unsigned int _rendererID;

public:
	Shader(std::string const& vertexFilePath, std::string const& fragmentFilePath);
	~Shader();

	void Bind() const;
	void Unbind() const;
	void SetUniform1i(int location, int value) const;
	void SetUniform4f(int location, float v0, float v1, float v2, float v3) const;
	void SetUniformMat4f(int location, glm::mat4 const& matrix) const;

private:
	std::string const ParseShader(std::string const& filePath) const;
	unsigned int CreateShader(std::string const& vertexShader, std::string const& fragmentShader) const;
	unsigned int CompileShader(unsigned int type, std::string const& source) const;
};

