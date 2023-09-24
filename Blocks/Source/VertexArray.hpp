#pragma once

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray
{
private:
	unsigned int _rendererID;

public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;
	void SetBuffer(VertexBuffer const& vb, VertexBufferLayout const& layout) const;
};

