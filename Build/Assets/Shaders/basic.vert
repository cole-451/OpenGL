#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_texturecoords;


out vec3 v_color;
uniform float u_time;
out vec2 v_texturecoords;


void main()
{
v_color = a_color * sin(u_time);
v_texturecoords = a_texturecoords;
gl_Position = vec4(a_position * sin(u_time), 1.0);



}
