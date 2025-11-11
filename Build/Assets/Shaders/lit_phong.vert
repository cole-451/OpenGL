#version 460 core
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texturecoords;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in vec3 a_tangent;

out VS_OUT
{
	 vec3 color;
	 vec3 position;
	 vec2 texturecoords;
	 vec3 normal;
	 mat3 tbn;
}vs_out;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform struct material{
	vec3 baseColor;
	vec3 emissiveColor;
	float shininess;
	vec2 tiling;
	vec2 offset;
	uint parameters;
} u_material;

void main()
{

	mat4 model_view = u_view * u_model;
	mat3 normal_matrix = transpose(inverse(mat3(model_view)));

	vs_out.texturecoords = a_texturecoords * u_material.tiling + u_material.offset;
	vs_out.position = vec3(model_view * vec4(a_position, 1));
	vs_out.normal = normalize(normal_matrix * a_normal);

	vec3 N = normalize(normal_matrix * a_normal);
	vec3 T = normalize(normal_matrix * a_tangent);
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	vec3 B = normalize(cross(N, T));
	vs_out.tbn = mat3(T, B, N);

	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}
