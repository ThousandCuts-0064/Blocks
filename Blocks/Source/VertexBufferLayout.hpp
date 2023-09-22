#pragma once

#include <vector>

#include "Renderer.h"
#include "Error.hpp"

struct VertextBufferElement
{
	unsigned int type;
	unsigned int count;
	bool isNormalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return sizeof(float);
			case GL_UNSIGNED_INT: return sizeof(unsigned int);
			case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
			default: throw Error(NotSupported, std::string(__func__) + "was called with type that is not supported");
		}
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertextBufferElement> _elements;
	unsigned int _stride;

public:
	VertexBufferLayout() :
		_stride(0)
	{};

	inline unsigned int GetStride() const { return _stride; }
	inline std::vector<VertextBufferElement> const& GetElements() const { return _elements; }

	template<typename T>
	void Push(unsigned int count)
	{
		_ASSERT(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		_stride += sizeof(float) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		_stride += sizeof(unsigned int) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		_stride += sizeof(unsigned char) * count;
	}
};

