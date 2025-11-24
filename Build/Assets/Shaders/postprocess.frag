#version 430

#define GRAYSCALE	(1 << 0)
#define COLORTINT	(1 << 1)
#define SCANLINE	(1 <<2)
#define GRAIN		(1 <<3)
#define INVERT		(1 <<4)
#define VIGNETTE	(1 <<5) 

in vec2 v_texcoord;

out vec4 f_color;

// post process controls
uniform uint u_parameters = 0;
uniform float u_time = 0;
uniform float u_intensity = 1.0;
uniform vec3 u_colorTint = vec3(0, 1, 0);
uniform float u_blend = 0;  // blend between original and postprocess color

uniform sampler2D u_baseMap;
uniform vec2 u_resolution;

// There is no random function in GLSL, uses math function to generate psuedo random number
// https://thebookofshaders.com/10/
float random(vec2 st) 
{
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
}


void main()
{
	vec4 color = texture(u_baseMap, v_texcoord);

	vec4 postprocess = color;
	
	if ((u_parameters & GRAYSCALE) != 0u)	postprocess = vec4(color.r + color.y + color.z /3.0, color.r + color.y + color.z /3.0, color.r + color.y + color.z /3.0, 1);
	//<rgb is the average value of the rgb values>
	if ((u_parameters & COLORTINT) != 0u) postprocess = postprocess * vec4(u_colorTint,1);
	//<multiply postprocess with u_colorTint>
	if ((u_parameters & SCANLINE) != 0u)	postprocess = (int(gl_FragCoord.y) % 3 != 0) ? vec4(0, 0, 0, 1) : postprocess;
	if ((u_parameters & GRAIN) != 0u)		postprocess = postprocess * random(gl_FragCoord.xy + u_time);
	if ((u_parameters & INVERT) != 0u)		postprocess = vec4(1-color.r, 1-color.g, 1-color.b, 1);
	//<invert color>

	if ((u_parameters & VIGNETTE) != 0u){
	//calculate distance, and darken pixels the further they are away from the screen
	vec2 centered = v_texcoord -0.5; //get the center of the screen
	centered.x *= u_resolution.x / u_resolution.y; // this adjusts this to the aspect ratio
	float distanceFromCenter = length(centered); 
	float vignette = smoothstep(0.7, 0.5, distanceFromCenter);
	postprocess.rgb *= 1.0 - vignette * u_intensity;
// obtained via copilot. if something IS wrong here, then I will go over it once more.


	}
	
	
	
	f_color = mix(color, postprocess, u_blend); // mix colors using u_blend (0-1)


}