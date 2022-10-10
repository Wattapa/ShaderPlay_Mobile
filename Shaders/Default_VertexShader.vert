#version 330

in vec4 posAttr;
in vec4 colAttr;

uniform mat4 matrix;

out vec4 col;

void main() 
{
	col = colAttr;
    gl_Position = matrix * posAttr;
}