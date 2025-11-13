#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 2) in vec3 a_normal;

out vec3 v_texturecoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;



void main()
{
	//transform position and normal to the world space
	vec3 position = vec3(u_model * vec4(a_position, 1));
	vec3 normal = normalize(mat3(u_model) * a_normal);

	vec3 viewPosition = inverse(u_view)[3].xyz;
	vec3 viewDir = normalize(position - viewPosition);

	//viewdir is reflected ABOUT the normal.
	v_texturecoords = reflect(viewDir, normal);

	//this is so we can still move our model across the plane and isnt stuck at the origin
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);

}
