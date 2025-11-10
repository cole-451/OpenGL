#version 460 core



in VS_OUT
{
 in vec3 color;
in vec2 texturecoords;
}fs_in;

out vec4 f_color;

uniform struct material{
	sampler2D baseMap;
	vec3 baseColor;

	float shininess;
	vec2 tiling;
	vec2 offset;
}u_material;

void main(){
	f_color = texture(u_material.baseMap, fs_in.texturecoords) * vec4(fs_in.color, 1);
	//f_color = vec4(fs_in.color, 1);
}