#version 460 core
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

#define	BASE_MAP		(1 << 0)
#define	SPECULAR_MAP	(1 << 1)
#define	EMISSIVE_MAP	(1 << 2)
#define	NORMAL_MAP		(1 << 3)
#define	CUBE_MAP		(1 << 4)
#define	SHADOW_MAP		(1 << 5)

out vec4 f_color;

in VS_OUT
{
	 vec3 color;
	 vec3 position;
	 vec2 texturecoords;
	 vec3 normal;
	 vec4 shadowcoord;
	 mat3 tbn;
}fs_in;

uniform struct material{
	vec3 baseColor;
	vec3 emissiveColor;
	float shininess;
	vec2 tiling;
	vec2 offset;
	uint parameters;
} u_material;

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
	bool shadowCaster;
};

uniform vec3 u_ambient_light;
uniform int u_num_lights = 1;
uniform Light u_lights[5];

uniform sampler2D u_baseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_emissiveMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_shadowMap;

float calculateAttenuation( in float light_distance, in float range){
	float attenuation = max(0.0 ,(1.0 - (light_distance / range)));
	return attenuation * attenuation;
}

float calculateShadow(in vec4 shadowcoord, in float bias){
return texture(u_shadowMap, shadowcoord.xy).z < shadowcoord.z - bias ? 0.0 : 1.0;
}

vec3 calculateLight(in Light light, vec3 position, vec3 normal, in float specularMask) {
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
		light_dir = -light.direction;
		attenuation = 1.0;
	break;

	case SPOT:
		light_dir = normalize(light.position - position); // the distance between your light and the object
		light_distance = length(light.position - position);
		attenuation = calculateAttenuation(light_distance, light.range);
		float angle = dot(light_dir, normalize(-light.direction));
		if (angle > light.outerCutoff){
		attenuation = 0;
		}
		else{
		float spotAttenuation = smoothstep(light.outerCutoff + 0.001, light.innerCutoff, angle);
		attenuation *= spotAttenuation;
		}
	break;
	}

	// diffuse
	float NdotL = max(dot(normal, light_dir), 0);
	vec3 diffuse = light.color * u_material.baseColor * NdotL;

	//specular
	//vec3 reflection = reflect(-light_dir, normal);
	//intensity = pow(intensity, u_material.shininess);

	//blinn phong
	vec3 view_dir = normalize(-position);
	vec3 halfway_dir = normalize(light_dir + view_dir);
	float NdotH = max(dot(normal, halfway_dir), 0);
	NdotH = pow(NdotH, u_material.shininess);
	vec3 specular = vec3(NdotH) * specularMask;

	return diffuse + specular;
}

vec3 calculateNormal()
{
	// generate the normals from the normal map
	vec3 normal = texture(u_normalMap, fs_in.texturecoords).rgb;
	// convert rgb normal (0 <-> 1) to xyx (-1 <-> 1)
	normal = normalize((normal *2) -1);
	// transform normals to model view space
	normal = normalize(fs_in.tbn * normal);

	return normal;
}

void main(){

	// specular mask (0 - 1)
	float specularMask = ((u_material.parameters & SPECULAR_MAP) != 0u)
	? texture(u_specularMap, fs_in.texturecoords).r
	: 1;

	// normal (normal mapping)
	vec3 normal = ((u_material.parameters & NORMAL_MAP) != 0u)
	? calculateNormal()
	: fs_in.normal;

	vec3 color = u_ambient_light;

	// multi-lights
	for(int i = 0; i <u_num_lights;i++){
  float shadow = (u_lights[i].shadowCaster && ((u_material.parameters & SHADOW_MAP) != 0u))
   ? calculateShadow(fs_in.shadowcoord, 0.001)
   : 1.0;
    color += calculateLight(u_lights[i], fs_in.position, normal, specularMask) * shadow;
  }

	// emissive
	vec4 emissive = ((u_material.parameters & EMISSIVE_MAP) != 0u)
	? texture(u_emissiveMap, fs_in.texturecoords) * vec4(u_material.emissiveColor, 1)
	: vec4(u_material.emissiveColor, 1);

	f_color = texture(u_baseMap, fs_in.texturecoords) * vec4(color, 1) + emissive;

}