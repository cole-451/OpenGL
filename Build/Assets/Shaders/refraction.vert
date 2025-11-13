#version 460 core

layout (location = 0) in vec3 a_position;

out vec3 v_texturecoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	v_texturecoords = a_position;
	
	mat4 vp = u_projection * mat4(mat3(u_view)); //double cast to kill translation
	gl_Position = vp * vec4(a_position, 1.0);

}
