#include "Renderer.h"

void Renderer::Draw(VertexArray const& va, IndexBuffer const& ib, Shader const& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}
