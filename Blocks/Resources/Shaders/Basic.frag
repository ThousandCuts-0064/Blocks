#version 430 core

in vec2 v_texCoord;

layout(location = 0) out vec4 color;

layout(location = 1) uniform vec4 u_color;
layout(location = 2) uniform sampler2D u_texture;

void main()
{
	vec4 texColor = texture(u_texture, v_texCoord);
	color = texColor;
}