#pragma once

class IndexBuffer
{
private:
	unsigned int _rendererID;
	unsigned int _count;

public:
	IndexBuffer(unsigned int const* data, unsigned int count);
	~IndexBuffer();

	inline unsigned int GetCount() const { return _count; }

	void Bind() const;
	void Unbind() const;
};

