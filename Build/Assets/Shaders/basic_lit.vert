#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_texturecoords;
layout (location = 3) in vec3 a_normal;

 out vec3 v_color;
out vec2 v_texturecoords;

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
sampler2D texture;
	float shininess;
	vec2 tiling;
	vec2 offset;
}u_material;

vec3 calculateLight(vec3 position, vec3 normal){
vec3 light_dir = normalize( u_light.position - position);
float intensity = max(dot(light_dir, normal), 0);

vec3 diffuse = u_light.color * intensity;

//specular
vec3 reflection = reflect(-light_dir, normal);
vec3 view_dir = normalize(-position);
intensity = max(dot(reflection, view_dir), 0);
intensity = pow(intensity, u_material.shininess);
vec3 specular = vec3(intensity);


return u_ambient_light + diffuse + specular;
}

void main()
{
	v_texturecoords = a_texturecoords * u_material.tiling + u_material.offset;

	mat4 model_view = u_view * u_model;
	vec3 position = vec3(model_view * vec4(a_position, 1));

	vec3 normal = normalize(mat3(model_view) * a_normal);



	v_color = calculateLight(position, normal);
	
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);

}
