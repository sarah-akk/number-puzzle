#version 330 core

in vec2 texture_pos;
in vec3 tex_color;

out vec4 color;

uniform sampler2D texture_sample;

void main()
{
    color = vec4(tex_color, 1.0) * texture(texture_sample, texture_pos);
}