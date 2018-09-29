#ifndef MK_APP
#define MK_APP
#include "MKType.h"
#include "MKShader.h"
#include "MKMesh.h"
#include <GLFW/glfw3.h>
#include <unordered_map>
#include "MKScene.h"


class MKApp
{
public:

	MKApp();
	~MKApp()
	{
	};
	bool init();
	bool windowInit();
	bool glInit();
	void run();
	void render();

	
private:
	GLFWwindow * m_window;
	

	uint32 m_cubeVAO;
	uint32 m_cubeVBO;
	uint32 m_cubeTB;

	// diffuse map
	uint32 m_diffuseMap;

	uint32 m_normMap;

	MKScene *m_scene;
	
};
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
#endif
