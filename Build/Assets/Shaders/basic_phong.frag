#version 460 core

in vec3 v_color;
in vec2 v_texturecoords;
in vec3 v_position;
in vec3 v_normal;

out vec4 f_color;

uniform struct material{
//sampler2D texture;
	sampler2D baseMap;
	vec3 baseColor;

	float shininess;
	vec2 tiling;
	vec2 offset;
}u_material;

//uniform sampler2D u_texture;

void main(){
	f_color = texture(u_material.baseMap, v_texturecoords) * vec4(v_color, 1);
	//f_color = vec4(v_color, 1);
}