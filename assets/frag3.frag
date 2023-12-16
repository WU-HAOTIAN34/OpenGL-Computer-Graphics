#version 430

in vec3 v2fColor; 
in float time;
layout( location = 0 ) out vec4 oColor;



void main()
{
	oColor = vec4( v2fColor, time / 25.0);
}