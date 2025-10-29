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

uniform struct light
{
vec3 position;
vec3 color;
}u_light;

uniform vec3 u_ambient_light;
//uniform sampler2D u_texture;

vec3 calculateLight(vec3 position, vec3 normal){
vec3 light_dir = normalize( u_light.position - position);
float intensity = max(dot(light_dir, normal), 0);

vec3 diffuse = u_light.color * u_material.baseColor * intensity;

//specular
vec3 reflection = reflect(-light_dir, normal);
vec3 view_dir = normalize(-position);
intensity = max(dot(reflection, view_dir), 0);
intensity = pow(intensity, u_material.shininess);
vec3 specular = vec3(intensity);

//bimm-phong

 light_dir = normalize( u_light.position - position);
 view_dir  = normalize(u_light.position - position);
 vec3 halfway_dir = normalize(light_dir + view_dir);

 float spec = pow(max(dot(v_normal, halfway_dir), 0.0), u_material.shininess);
specular = u_light.color * spec;


return u_ambient_light + diffuse + specular;
}

void main(){
	f_color = texture(u_material.baseMap, v_texturecoords) * vec4(v_color, 1);
	//f_color = vec4(v_color, 1);
}