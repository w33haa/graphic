#ifndef MK_TOOLS
#define MK_TOOLS


#include <iostream>
#include <glad/glad.h>
#include <string>
#include <windows.h>
#include "MKType.h"


uint32 loadTexture(char const * path);
uint32 getPrimitiveEnum(std::string &primitiveName);
class MKTimer
{
public:
	MKTimer() {}
	uint64 lastTime;
	void Start()
	{
		lastTime = GetTickCount();
	}
	uint64 Period()
	{
		uint64 now = GetTickCount();
		uint64 period = now - lastTime;
		lastTime = now;
		return period;
	}
};
////创建纹理对象并使用图片初始化纹理
//uint32 loadTexture(char const * path)
//{
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//
//	int width, height, nrComponents;
//	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
//	if (data)
//	{
//		GLenum format;
//		if (nrComponents == 1)
//			format = GL_RED;
//		else if (nrComponents == 3)
//			format = GL_RGB;
//		else if (nrComponents == 4)
//			format = GL_RGBA;
//
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		stbi_image_free(data);
//	}
//	else
//	{
//		std::cout << "Texture failed to load at path: " << path << std::endl;
//		stbi_image_free(data);
//	}
//	return textureID;
//}


#endif
