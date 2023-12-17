#version 430

in vec3 v2fColor; 
layout( location = 0 ) uniform float alpha;
layout( location = 1 ) uniform vec3 uBaseColor;
layout( location = 0 ) out vec4 oColor;


void main()
{
	oColor = vec4( v2fColor * uBaseColor, alpha);
}