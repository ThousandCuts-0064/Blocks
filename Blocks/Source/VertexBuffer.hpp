#pragma once

class VertexBuffer
{
private:
	unsigned int _rendererID;

public:
	VertexBuffer(void const* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};

