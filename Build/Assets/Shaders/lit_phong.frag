#version 460 core
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2
#define		BASE_MAP (1 << 0)
#define		SPECULAR_MAP (1 << 1)
#define		EMISSIVE_MAP (1 << 2)

in vec3 v_color;
in vec2 v_texturecoords;
in vec3 v_position;
in vec3 v_normal;

out vec4 f_color;

in VS_OUT
{
	 vec3 color;
	 vec3 position;
	 vec2 texturecoords;
	 vec3 normal;
}fs_in;

uniform struct material{
	vec3 baseColor;
	vec3 emissiveColor;
	float shininess;
	vec2 tiling;
	vec2 offset;
	uint parameters;
}u_material;

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

uniform vec3 u_ambient_light;
uniform int u_num_lights = 1;
uniform Light u_lights[5];

uniform sampler2D u_baseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_emissiveMap;

float calculateAttenuation( in float light_distance, in float range){
	float attenuation = max(0.0 ,(1.0 - (light_distance / range)));
	return attenuation * attenuation;
}

vec3 calculateLight(in Light light, vec3 position, vec3 normal, in float specularMask){

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
		attenuation = calculateAttenuation(light_distance, light.range);
		float angle = dot(light_dir, normalize(light.direction));
	float spotAttenuation = smoothstep(light.outerCutoff + 0.001, light.innerCutoff, angle);
	break;
	}

float intensity = max(dot(light_dir, normal), 0);

vec3 diffuse = light.color * u_material.baseColor * intensity;

//specular
vec3 reflection = reflect(-light_dir, normal);

intensity = pow(intensity, u_material.shininess);


//blinn phong
 vec3 view_dir = normalize(-position);
 vec3 halfway_dir = normalize(light_dir + view_dir);
 intensity = max(dot(reflection, view_dir), 0);
 intensity = pow(intensity, u_material.shininess);
 vec3 specular = vec3(intensity) * specularMask;



return u_ambient_light + diffuse + specular;
}

void main(){

//syntax issue number one
float specularMask = ((u_material.parameters & SPECULAR_MAP) != 0u)
  ? texture(u_specularMap, fs_in.texturecoords).r
  : 1;

	vec3 color = u_ambient_light;

	//for lights
	for(int i = 0; i <u_num_lights;i++){
	color = calculateLight(u_lights[i], fs_in.position, fs_in.normal, specularMask);
	}

	//another syntax issue.
	vec4 emissive = ((u_material.parameters & EMISSIVE_MAP) != 0u)
  ? texture(u_emissiveMap, fs_in.texturecoords) * vec4(u_material.emissiveColor, 1)
  : vec4(u_material.emissiveColor, 1);

	f_color = texture(u_baseMap, fs_in.texturecoords) * vec4(fs_in.color, 1);

}