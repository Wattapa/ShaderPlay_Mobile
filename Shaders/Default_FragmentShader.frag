#version 330

in vec4 col;

layout(location = 0, index = 0) out vec4 color;

void main() 
{
    color = col;
}