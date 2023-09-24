#include "VertexArray.hpp"

VertexArray::VertexArray() :
	_rendererID(0)
{
	glGenVertexArrays(1, &_rendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &_rendererID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(_rendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(GL_ZERO);
}

void VertexArray::SetBuffer(VertexBuffer const& vb, VertexBufferLayout const& layout) const
{
	Bind();
	vb.Bind();
	auto const& elements = layout.GetElements();
	unsigned long long offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		auto const& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.isNormalized, layout.GetStride(), (void const*)offset);
		offset += static_cast<unsigned long long>(element.count) * VertextBufferElement::GetSizeOfType(element.type);
	}
}
