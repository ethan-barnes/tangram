#version 330 core
int vec3 My_color;
out vec4 color;

void main()
{
color = vec4(My_color, 1.0f);
}