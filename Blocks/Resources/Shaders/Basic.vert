#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;

layout(location = 3) uniform mat4 u_mvp;

void main()
{
	gl_Position = u_mvp * position;
	v_texCoord = texCoord;
}