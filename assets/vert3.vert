#version 430

layout( location = 0 ) in vec3 iPosition; 
layout( location = 0 ) uniform mat4 uProjCameraWorld;
layout( location = 1 ) in vec3 iColor;
layout( location = 1 ) uniform mat3 uNormalMatrix;
layout( location = 2 ) in float lifeTime;

out vec3 v2fColor;
out float time;

void main()
{
	time = lifeTime;
	v2fColor = iColor;
	gl_Position = uProjCameraWorld * vec4( iPosition, 1.0 );
}
