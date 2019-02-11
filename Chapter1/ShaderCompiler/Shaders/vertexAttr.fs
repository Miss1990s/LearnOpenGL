#version 330 core
out vec4 FragColor;
in vec3 outColor;
uniform float offset;

void main()
{
	FragColor = vec4(outColor.xy, offset,1.0);
}