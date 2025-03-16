#version 330 compatibility
layout(location = 0)	in vec		position;
out vec2    fragPosition;

void main(){
	gl_Position = vec4(position / 350.0, 1.0);
	fragPosition = position;
}