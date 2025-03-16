#version 330 compatibility

in		vec2 fragPosition;
out		vec4 fragColor;
uniform bool colorSwap;

void main(){
	float distance = length(fraPosition);
	vec3 center = vec3(1.0,0.0,0.0);
	vec3 border = vec3(0.0,1.0,0.0);
	if(colorSwap){
		center = vec3(0.0,1.0,0.0);
		border = vec3(1.0,0.0,0.0);
	}
	fragColor = vec4(mix(center, border, distance / 50.0), 1.0);

}