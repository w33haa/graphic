#version 420 core
layout (location = 0) in vec3 iPositon;
layout (location = 2) in vec2 iTexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float frameTime;



out VS_OUT{
    vec4 position;
    vec2 texCoords0;
    vec2 texCoords1;
    vec2 texCoords2;
    vec2 texCoords3;
}vs_out;

void main()
{
	// ³£Êý
	vec3 scrollSpeeds = vec3(1.3f, 2.1f, 2.3f);
	vec3 scales = vec3(1.0f, 2.0f, 3.0f);

    vs_out.position = projection * view * model * vec4(iPositon, 1.0);

	//debug
	/*
	vec2 t = iTexCoords;
	t.x += frameTime * scrollSpeeds.x;
    vs_out.texCoords0 = t;
	*/
	vs_out.texCoords0 = iTexCoords;

    // Compute texture coordinates for first noise texture using the first scale and upward scrolling speed values.
    vs_out.texCoords1 = (iTexCoords * scales.x);
    vs_out.texCoords1.y = vs_out.texCoords1.y + (frameTime * scrollSpeeds.x);

    // Compute texture coordinates for second noise texture using the second scale and upward scrolling speed values.
    vs_out.texCoords2 = (iTexCoords * scales.y);
    vs_out.texCoords2.y = vs_out.texCoords2.y + (frameTime * scrollSpeeds.y);

    // Compute texture coordinates for third noise texture using the third scale and upward scrolling speed values.
    vs_out.texCoords3 = (iTexCoords * scales.z);
    vs_out.texCoords3.y = vs_out.texCoords3.y + (frameTime * scrollSpeeds.z);
     
    gl_Position = projection * view * model * vec4(iPositon, 1.0);
}
