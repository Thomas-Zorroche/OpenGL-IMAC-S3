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
out vec2 vTexCoords;

void main()
{
    // Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aPosition, 1);
    vec4 vertexNormal = vec4(aNormals, 0);

    // Calcul des valeurs de sortie
    vPosition_vs = vec3(u_MV * vertexPosition);
    vNormals_vs = vec3(u_NormalMatrix * vertexNormal);
    vTexCoords = aTexCoords;

    gl_Position = u_MVP * vertexPosition;
};

#shader fragment
#version 330 core

out vec3 fFragColor;
in vec3 vNormals_vs;
in vec2 vTexCoords; 

uniform sampler2D Texture;

void main()
{
    fFragColor = vec3(texture(Texture, vTexCoords).r);
};