#include "MKTools.h"
#include <iostream>
#include <glad/glad.h>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include "MKType.h"
using namespace std;


// 创建纹理对象并使用图片初始化纹理 内部绑定纹理数据到当前上下文中了，
//如需绑定到不同的texture unit需在调用loadTexture之前调用glActiveTexture
uint32 loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
		//if(path)
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

uint32 getPrimitiveEnum(string &primitiveName)
{
	if (primitiveName == "GL_POINTS")
	{
		return GL_POINTS;
	}
	else if(primitiveName == "GL_LINES")
	{
		return GL_LINES;
	}
	else if (primitiveName == "GL_LINE_LOOP")
	{
		return GL_LINE_LOOP;
	}
	else if (primitiveName == "GL_LINE_STRIP")
	{
		return GL_LINE_STRIP;
	}
	else if (primitiveName == "GL_TRIANGLES")
	{
		return GL_TRIANGLES;
	}
	else if (primitiveName == "GL_TRIANGLE_STRIP")
	{
		return GL_TRIANGLE_STRIP;
	}
	else if (primitiveName == "GL_TRIANGLE_FAN")
	{
		return GL_TRIANGLE_FAN;
	}
	return GL_TRIANGLES;
}