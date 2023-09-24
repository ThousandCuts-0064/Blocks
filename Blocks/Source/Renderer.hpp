#pragma once

#include <glew.h>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Renderer
{
public:
	void Clear() const;
	void Draw(VertexArray const& va, IndexBuffer const& ib, Shader const& shader) const;
};