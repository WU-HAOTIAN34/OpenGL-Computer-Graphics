#version 430

layout( location = 0 ) in vec3 iPosition; 
layout( location = 0 ) uniform mat4 uProjCameraWorld;
layout( location = 1 ) in vec3 iColor;
layout( location = 2 ) in vec3 iNormal;
layout( location = 1 ) uniform mat3 uNormalMatrix;
layout( location = 11 ) uniform mat4 T;

out vec3 v2fColor;
out vec3 v2fNormal;
out vec3 v2fPosition;

void main()
{
	v2fColor = iColor;
	v2fPosition = (T * vec4( iPosition, 1.0 )).xyz;
	gl_Position = uProjCameraWorld * vec4( iPosition, 1.0 );
	v2fNormal = normalize(uNormalMatrix * iNormal); 
}
