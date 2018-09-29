#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 instance_position;
layout (location = 2) in vec4 instance_color;

out vec4 VertColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec4 color;

void main()
{ 
	
    gl_Position = projection * view * model  * vec4(aPos+instance_position, 1.0);
	VertColor = instance_color;
	//VertColor = color;
	//VertColor = mix(vec4(0.0,0.5,0.5,1.0),vec4(1.0,0,0,1.0),instance_position.y/-3);
}