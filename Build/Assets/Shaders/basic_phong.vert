#version 460 core

layout (location = 0) in vec3 a_position;
//layout (location = 1) in vec3 a_color;
layout (location = 1) in vec2 a_texturecoords;
layout (location = 2) in vec3 a_normal;

 out vec3 v_color;
out vec2 v_texturecoords;
out vec3 v_position;
out vec3 v_normal;


uniform float u_time;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform vec3 u_ambient_light;
uniform vec3 u_light_pos;

uniform struct light
{
vec3 position;
vec3 color;
}u_light;

uniform struct material{
//sampler2D texture;
	sampler2D baseMap;
	vec3 baseColor;

	float shininess;
	vec2 tiling;
	vec2 offset;
}u_material;



void main()
{
	v_texturecoords = a_texturecoords * u_material.tiling + u_material.offset;

	mat4 model_view = u_view * u_model;
	vec3 position = vec3(model_view * vec4(a_position, 1));

	vec3 normal = normalize(mat3(model_view) * a_normal);

	
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);

}
