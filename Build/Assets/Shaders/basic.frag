#version 460 core

in vec3 v_color;
in vec2 v_texturecoords;

out vec4 f_color;

uniform float u_time;
uniform sampler2D u_texture;

void main(){
	//float offset = sin(u_time + gl_FragCoord.x);
	f_color = texture(u_texture, v_texturecoords) * vec4(v_color, 1);
}