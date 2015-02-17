#version 330

uniform float vec4 position;

void main(void)
{
    gl_Position = position;
    gl_TexCoord[0] = gl_MultiTexCoord0;
}

