#pragma once

#include <string>

#include "Renderer.hpp"

class Texture
{
private:
	std::string _filePath;
	unsigned char* _buffer;
	unsigned int _rendererID; 
	int _width, _height, _bpp;

public:
	Texture(std::string const& filePath);
	~Texture();

	inline unsigned int GetWidth() const { return _width; }
	inline unsigned int GetHeight() const { return _width; }

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
};