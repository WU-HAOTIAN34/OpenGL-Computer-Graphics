#version 430

in vec3 v2fColor;
in vec3 v2fNormal;
in vec3 v2fPosition;

layout( location = 2 ) uniform vec3 lightPos;      
layout( location = 3 ) uniform vec3 ambientColor;  
layout( location = 4 ) uniform vec3 lightColor;    
layout( location = 5 ) uniform vec3 viewPos;       
layout( location = 6 ) uniform float shininess;
layout( location = 7 ) uniform vec3 lightPos2;
layout( location = 8 ) uniform vec3 lightColor2;  
layout( location = 9 ) uniform vec3 lightPos3;
layout( location = 10 ) uniform vec3 lightColor3;  

layout( location = 0 ) out vec4 oColor;

void main() {
    
    vec3 norm = normalize(v2fNormal);
    vec3 lightDir = normalize(lightPos - v2fPosition);
    vec3 viewDir = normalize(viewPos - v2fPosition);

    vec3 ambient = ambientColor * lightColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = spec * lightColor;

    float distance = length(lightPos - v2fPosition);
    float distanceSquared = distance * distance;

    diffuse /= distanceSquared;
    specular /= distanceSquared;

    vec3 lightDir2 = normalize(lightPos2 - v2fPosition);
    vec3 viewDir2 = normalize(viewPos - v2fPosition);

    vec3 ambient2 = ambientColor * lightColor2;

    float diff2 = max(dot(norm, lightDir2), 0.0);
    vec3 diffuse2 = diff2 * lightColor2;

    vec3 halfwayDir2 = normalize(lightDir2 + viewDir);
    float spec2 = pow(max(dot(norm, halfwayDir2), 0.0), shininess);
    vec3 specular2 = spec2 * lightColor2;

    float distance2 = length(lightPos2 - v2fPosition);
    float distanceSquared2 = distance2 * distance2;

    diffuse2 /= distanceSquared2;
    specular2 /= distanceSquared2;


    vec3 lightDir3 = normalize(lightPos3 - v2fPosition);
    vec3 viewDir3 = normalize(viewPos - v2fPosition);

    vec3 ambient3 = ambientColor * lightColor3;

    float diff3 = max(dot(norm, lightDir3), 0.0);
    vec3 diffuse3 = diff3 * lightColor3;

    vec3 halfwayDir3 = normalize(lightDir3 + viewDir);
    float spec3 = pow(max(dot(norm, halfwayDir3), 0.0), shininess);
    vec3 specular3 = spec3 * lightColor3;

    float distance3 = length(lightPos3 - v2fPosition);
    float distanceSquared3 = distance3 * distance3;

    diffuse3 /= distanceSquared3;
    specular3 /= distanceSquared3;

    oColor = vec4((ambient + diffuse + specular + ambient2 + diffuse2 + specular2 + ambient3 + diffuse3 + specular3) * v2fColor, 1.0);

}