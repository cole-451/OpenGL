#version 460 core

#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2


layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texturecoords;
layout (location = 2) in vec3 a_normal;

out VS_OUT
{
	out vec3 color;
	out vec2 texturecoords;
}vs_out;

uniform float u_time;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform vec3 u_ambient_light;
uniform vec3 u_light_pos;

 struct Light
{
	int type;
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float range;
	float outerCutoff;
	float innerCutoff;
};

uniform struct material{
	sampler2D baseMap;
	vec3 baseColor;

	float shininess;
	vec2 tiling;
	vec2 offset;
} u_material;

uniform int u_num_lights = 1;
uniform Light u_lights[5];

float calculateAttenuation(in float light_distance, in float range){
	float attenuation = max(0.0 ,(1.0 - (light_distance / range)));
	return attenuation * attenuation;
}


vec3 calculateLight(in Light light, in vec3 position, in vec3 normal){
	float attenuation = 1.0;
	vec3 light_dir;
	float light_distance;
	switch(light.type){
	case POINT:
		light_dir = normalize( light.position - position);
		light_distance = length(light.position - position);
		attenuation = calculateAttenuation(light_distance, light.range);
	break;
	case DIRECTIONAL:
	light_dir = light.direction;
	attenuation = 1.0;
	break;

	case SPOT:
		light_dir = normalize(light.position - position);
		light_distance = length(light.position - position);
		float angle = dot(light_dir, normalize(light.direction));
		attenuation = calculateAttenuation(light_distance, light.range);
	float spotAttenuation = smoothstep(light.outerCutoff + 0.001, light.innerCutoff, angle);
	break;
	}

	//light_dir = normalize( light.position - position);

	//diffuse
	float intensity = max(dot(light_dir, normal), 0);

	vec3 diffuse = light.color * u_material.baseColor * intensity;

	//specular
	vec3 reflection = reflect(-light_dir, normal);
	vec3 view_dir = normalize(-position);
	intensity = max(dot(reflection, view_dir), 0);
	intensity = pow(intensity, u_material.shininess);
	vec3 specular = vec3(intensity);

	//blinn-phong
	vec3 halfway_dir = normalize(light_dir + view_dir);
	float NdotH = max(dot(normal, halfway_dir), 0);
	NdotH = pow(NdotH, u_material.shininess);
	specular = vec3(NdotH);


	//float light_distance = length(light.position - position);
	//float attenuation = calculateAttenuation(light_distance, light.range);

	return (diffuse + specular) * light.intensity * attenuation;
}

void main()
{
	vs_out.texturecoords = a_texturecoords * u_material.tiling + u_material.offset;

	mat4 model_view = u_view * u_model;
	vec3 position = vec3(model_view * vec4(a_position, 1));

	vec3 normal = normalize(mat3(model_view) * a_normal);

	vs_out.color = u_ambient_light;
	for(int i = 0; i <u_num_lights; i++){
		vs_out.color += calculateLight(u_lights[i],position, normal);
	}

	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);

}
