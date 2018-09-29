#version 420 core
layout (location = 0) in vec3 aPos;


out vec4 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform  int drawId;

void main()
{ 
	int did =  gl_InstanceID + drawId;
	float x = did/32;
	float z = did%32;
	mat4 tran = mat4(1.0);
	tran[3][0] = 0.5 * x;
	tran[3][2] = 0.5 * z;
	color.r = did/256.0/8.0;
	color.g = (did/64%64)/16.0;
	color.b = (did%64)/16.0;
	//color.r=1.0;
    gl_Position = projection * view * model * tran * vec4(aPos, 1.0);
}