#shader vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormals;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 u_MVP;
uniform mat4 u_MV;
uniform mat4 u_NormalMatrix;

// Sorties du VertexShader
out vec3 vNormals_vs;
out vec3 vPosition_vs;

void main()
{
    // Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aPosition, 1);
    vec4 vertexNormal = vec4(aNormals, 0);

    // Calcul des valeurs de sortie
    vPosition_vs = vec3(u_MV * vertexPosition);
    vNormals_vs = vec3(u_NormalMatrix * vertexNormal);
    // vTexCoords = aTexCoords;

    gl_Position = u_MVP * vertexPosition;
};

#shader fragment
#version 330 core

out vec3 fFragColor;

in vec3 vNormals_vs;
in vec3 vPosition_vs;

uniform vec3 u_Kd;
uniform vec3 u_Ks;
uniform float u_Shininess;

uniform vec3 u_LightDir_vs;
uniform vec3 u_LightIntensity;

vec3 BlinnPhong()
{
    // Vecteur Pointant Vers la Camera
    vec3 viewDir = normalize(-vPosition_vs);
    vec3 Normal = normalize(vNormals_vs);
    vec3 LightDir = normalize(u_LightDir_vs);

    vec3 halfwayDir = (viewDir + LightDir) * 0.5;

    vec3 Color = u_LightIntensity
                * (u_Kd * dot(LightDir, Normal)
                + u_Ks * pow(dot(halfwayDir, Normal), u_Shininess));

    return Color;
}

void main()
{
    // Blinn Phong
    fFragColor = BlinnPhong();
};