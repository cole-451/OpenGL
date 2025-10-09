#version 460 core

in vec3 v_color;
out vec4 f_color;
void main(){
//f_color = vec4(0.5,0,1,1);
f_color = vec4(v_color, 1);

}