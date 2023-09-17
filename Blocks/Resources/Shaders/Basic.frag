#version 430 core

layout(location = 0) out vec4 color;

layout(location = 1) uniform vec4 u_color;

void main()
{
	color = u_color;
}