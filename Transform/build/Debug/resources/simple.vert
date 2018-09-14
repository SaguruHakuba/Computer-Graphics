#version 330

in vec2 pos;
in vec3 color;

//TODO1 What is the default value of this matrix? 0!
uniform mat4 T;

out vec4 smoothColor;

void main()
{
	smoothColor = vec4(color, 1);
	vec4 v = vec4(pos, 0, 1);
	
	//TODO2 Multiply the vertex by the transform matrix
	v = T * v;
	gl_Position = v;
}
