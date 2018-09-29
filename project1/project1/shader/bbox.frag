

#version 420 core
out vec4 FragColor;

in  vec2 TexCoords;


uniform sampler2D diffuseMapSampler;


void main()
{

	//FragColor = texture(diffuseMapSampler, TexCoords);

	vec4 texColor = texture(diffuseMapSampler, TexCoords);
    //if(texColor.a < 0.1)
    //    discard;
    FragColor = texColor;
} 

