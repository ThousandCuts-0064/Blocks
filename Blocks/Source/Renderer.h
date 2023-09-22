#pragma once

#include <glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	void Draw(VertexArray const& va, IndexBuffer const& ib, Shader const& shader) const;
};