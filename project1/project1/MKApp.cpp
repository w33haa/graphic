#include "MKApp.h"
#include <GLFW/glfw3.h>



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <windows.h>
#include "MKCamera.h"
//#include "MKMesh.h"
//#include "MKTools.h"
#include "MKType.h"
#include <iostream>
#include "MKMeshData.h"
#include "MKGlobal.h"
#include "MKPartices.h"
#include "MKBillbord.h"

bool IsLeftPress = false;
bool IsRightPress = false;
bool IsUpPress = false;
bool IsDownPress = false;
bool IsPageUpPress = false;
bool IsPageDownPress = false;

uint32 nbFrames = 0;
double lastTime = 0.0;
void setWindowFPS(GLFWwindow* win)
{
	// Measure speed
	double currentTime = glfwGetTime();
	nbFrames++;

	if (currentTime - lastTime >= 1.0) { // If last cout was more than 1 sec ago
		char title[256];
		title[255] = '\0';

		snprintf(title, 255,
			"[FPS: %3.2f]",
			  (float)nbFrames/(currentTime - lastTime));

		glfwSetWindowTitle(win, title);

		nbFrames = 0;
		lastTime = currentTime;
	}
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// screen
extern const uint32 SCR_WIDTH;
extern const uint32 SCR_HEIGHT;

//mouse
bool isMouseLeftPress = false;
bool isMouseRightPress = false;
float lastLX = SCR_WIDTH / 2.0f;
float lastLY = SCR_HEIGHT / 2.0f;
float lastRX = SCR_WIDTH / 2.0f;
float lastRY = SCR_HEIGHT / 2.0f;

//callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

MKApp::MKApp()
{

}

bool MKApp::init()
{
	return windowInit() && glInit();
}

bool MKApp::windowInit()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_window);

	// 输入回调函数
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetCursorPosCallback(m_window, cursor_pos_callback);
	glfwSetScrollCallback(m_window, scroll_callback);
	glfwSetKeyCallback(m_window, key_callback);
	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	return 1;
}

bool MKApp::glInit()
{
	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 加载shader
	cubeShader = new MKShader("shader/cube.vert", "shader/cube.frag");
	defaultShader = new MKShader("shader/bbox.vert", "shader/bbox.frag");
	instancingShader = new MKShader("shader/instancing.vert", "shader/instancing.frag");
	particleShader = new MKShader("shader/particle.vert", "shader/particle.frag");
	fireShader = new MKShader("shader/fire.vert", "shader/fire.frag");
	
	// cube mesh
	vector<glm::vec3> positions;
	vector<glm::vec3> normals;
	vector<glm::vec2> textCoords;
	vector<glm::vec3> tangent;
	vector<glm::vec3> bitangent;
	vector<uint32> indices;

	for (int i = 0,j =0; i < sizeof(cubeVertices)/sizeof(float); i = i + 8, j+= 6)
	{
		positions.push_back(glm::vec3(cubeVertices[i], cubeVertices[i + 1], cubeVertices[i + 2]));
		normals.push_back(glm::vec3(cubeVertices[i+3], cubeVertices[i + 4], cubeVertices[i + 5]));
		textCoords.push_back(glm::vec2(cubeVertices[i+6], cubeVertices[i + 7]));
		tangent.push_back(glm::vec3(cubeTB[j], cubeTB[j + 1], cubeTB[j + 2]));
		bitangent.push_back(glm::vec3(cubeTB[j+3], cubeTB[j + 4], cubeTB[j + 5]));
		indices.push_back(i/8);
	}

	/*positions.push_back(glm::vec3(0.5, 0, 0));
	positions.push_back(glm::vec3(0, 0.5, 0));
	positions.push_back(glm::vec3(0, 0, 0.5));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);*/
	/*for (int i = 0; i < sizeof(cubeVertices) / sizeof(float); i++)
	{
		indices.push_back(i);
	}*/

	MKMesh *meshCube = new MKMesh(positions, normals, textCoords, tangent, bitangent, indices);
	//MKMesh *meshCube = new MKMesh(positions, indices);
	//MKMesh *meshWall = new MKMesh(vertexArray, indices);

	// tet mesh
	vector<glm::vec3> tetPositions;
	vector<glm::vec3> tetNormals;
	vector<glm::vec2> tetTextCoords;
	vector<glm::vec3> tetTangent;
	vector<glm::vec3> tetBitangent;

	for (int i = 0, j = 0; i < sizeof(tetVertices) / sizeof(float); i = i + 8, j += 6)
	{
		tetPositions.push_back(glm::vec3(tetVertices[i], tetVertices[i + 1], tetVertices[i + 2]));
		tetNormals.push_back(glm::vec3(tetVertices[i + 3], tetVertices[i + 4], tetVertices[i + 5]));
		tetTextCoords.push_back(glm::vec2(tetVertices[i + 6], tetVertices[i + 7]));
		tetTangent.push_back(glm::vec3(tetTB[j], tetTB[j + 1], tetTB[j + 2]));
		tetBitangent.push_back(glm::vec3(tetTB[j + 3], tetTB[j + 4], tetTB[j + 5]));
	}

	vector<uint32> indicesTET;
	for (int i = 0; i < 18; i++)
	{
		indicesTET.push_back(i);
	}
	MKMesh *meshTET = new MKMesh(tetPositions, tetNormals, tetTextCoords, tetTangent, tetBitangent, indicesTET);
	
	meshMap["cube"] = meshCube;
	meshMap["tet"] = meshTET;
	
	m_scene = new MKScene();
	m_scene->LoadFromXML((xmlDir +string("Scene.xml")).c_str());
	m_scene->m_rootNode.m_childNodes[0]->m_modelToParent = glm::rotate(glm::mat4(1.0), glm::radians(50.0f), glm::vec3(1.0, 0, 0));
	m_scene->m_rootNode.ComputeBoundingBox();
	//m_scene->m_rootNode.CreateBoundingBoxMeshSubTree();

	camera.CreateFrusdumMesh();

	//sphere mesh test
	vector<glm::vec3> sphereInputV, sphereOutputV;
	vector<uint32> sphereInputI, sphereOutputI, sphereWireFrameI;
	for (int i = 0; i < sizeof(octVertices) / sizeof(float); i += 3)
	{
		glm::vec3 v(octVertices[i], octVertices[i + 1], octVertices[i + 2]);
		sphereInputV.push_back(v);
	}
	sphereInputI.assign(begin(octIndices), end(octIndices));
	sphereOutputV = sphereInputV;
	splitTriangle(sphereInputV, sphereInputI, sphereOutputV, sphereOutputI, 1);
	
	for (int i = 0; i < sphereOutputV.size(); i++)
	{
		sphereOutputV[i] /= 10;
	}
	for (int i = 0; i < sphereOutputI.size(); i+=3)
	{
		sphereWireFrameI.push_back(sphereOutputI[i]);
		sphereWireFrameI.push_back(sphereOutputI[i+1]);

		sphereWireFrameI.push_back(sphereOutputI[i+1]);
		sphereWireFrameI.push_back(sphereOutputI[i+2]);

		sphereWireFrameI.push_back(sphereOutputI[i]);
		sphereWireFrameI.push_back(sphereOutputI[i+2]);
	}
	//MKMesh *sphereMesh = new MKMesh(sphereOutputV, sphereOutputI);
	MKMesh *sphereMesh = new MKMesh(sphereOutputV, sphereOutputI);
	MKMaterial *simpleMaterial = new MKMaterial(defaultShader);
	simpleMaterial->m_primitive = GL_TRIANGLES;// GL_TRIANGLES;// GL_LINES;
	sphereMesh->UseMaterial(simpleMaterial);
	meshMap["sphere"] = sphereMesh;

	vector<vec3> faceVertices;
	faceVertices.push_back(vec3(-1.0, -1.0, 0));
	faceVertices.push_back(vec3(-1.0, 1.0, 0));
	faceVertices.push_back(vec3(1.0, 1.0, 0));
	faceVertices.push_back(vec3(1.0, -1.0, 0));

	vector<vec2> faceTextCoord;
	faceTextCoord.push_back(vec2(0.0, 1.0));
	faceTextCoord.push_back(vec2(0.0, 0.0));
	faceTextCoord.push_back(vec2(1.0, 0.0));
	faceTextCoord.push_back(vec2(1.0, 1.0));

	vector<uint32> faceIndices;
	uint32 index[] = { 0,3,1,3,2,1 };
	faceIndices.assign(begin(index), end(index));
	// face
	MKMesh *faceMesh = new MKMesh(faceVertices, faceIndices, faceTextCoord);


	MKMaterial *goldMaterial = new MKMaterial(defaultShader);
	goldMaterial->loadFromXML((xmlDir+"Material-gold.xml").c_str());
	simpleMaterial->m_primitive = GL_TRIANGLES;// GL_TRIANGLES;// GL_LINES;
	faceMesh->UseMaterial(goldMaterial);
	meshMap["faceMesh"] = faceMesh;


	instest = new InstancingTest(sphereOutputV, sphereOutputI);
	
	psystem = new MKParticleSystem(sphereOutputV, sphereOutputI);
	bh = new MKBlackHole(sphereOutputV, sphereOutputI);
	
	
	// 火焰材质和mesh
	MKMaterial *fireMaterial = new MKMaterial(fireShader);
	fireMaterial->loadFromXML((xmlDir + "Material-fire.xml").c_str());
	fireMaterial->m_primitive = GL_TRIANGLES;// GL_TRIANGLES;// GL_LINES;
	// 设置noiseSampler的wrapping mode

	


	MKMesh *fireMesh = new MKMesh(faceVertices, faceIndices, faceTextCoord);
	fireMesh->UseMaterial(fireMaterial);
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glActiveTexture(GL_TEXTURE1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glActiveTexture(GL_TEXTURE0);
	

	meshMap["fire"] = fireMesh;

	bb = new MKBillBord(vec3(2.0, 0, 0), fireMesh);
	
	fire = new MKFire(sphereOutputV, sphereOutputI);

	return true;
}

void MKApp::run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		// input
		processInput(m_window);

		render();
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(m_window);
		setWindowFPS(m_window);
		glfwPollEvents();
		//Sleep(100);
	}

	// 删除VAO,VBO
	glDeleteVertexArrays(1, &m_cubeVAO);
	glDeleteBuffers(1, &m_cubeVBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}

void MKApp::render()
{
	//static float frameTime = 0.0f;
	// Increment the frame time counter.
	frameTime += 0.01f;
	if (frameTime > 1000.0f)
	{
		frameTime = 0.0f;
	}

	// 重置颜色缓存、深度缓存
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//camera.Move(glm::vec3(0.01, 0, 0));
	
	//m_scene->Draw();
	//cout << "drawCallPerLoop :" << drawCallPerLoop << endl;;
	//camera.m_frustum->Draw();
	glm::mat4 a(1.0);
	//instest->Draw();
	//instest->Draw2();
	//psystem->Update();
	//bh->Update();
	fire->Update();
	//bb->Render();
	//meshMap["fire"]->Draw(a, frameTime);

	//meshMap["faceMesh"]->Draw(a);
	//meshMap["faceMesh"]->Draw(camera.GetModelMatrix());
	//cout << "particle num:" << psystem->partices.size() << endl;
	/*meshMap["sphere"]->m_material->m_shader->use();
	meshMap["sphere"]->m_material->m_shader->setVec4("color", glm::vec4(1.0, 0, 0, 1.0));
	meshMap["sphere"]->Draw(camera.GetModelMatrix());*/
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (IsLeftPress)
		camera.Move(glm::vec3(-CAMERA_MOVE_SPEED, 0, 0));
	if (IsRightPress)
		camera.Move(glm::vec3(CAMERA_MOVE_SPEED, 0, 0));
	if (IsUpPress)
		camera.Move(glm::vec3(0, CAMERA_MOVE_SPEED, 0));
	if (IsDownPress)
		camera.Move(glm::vec3(0, -CAMERA_MOVE_SPEED, 0));
	if (IsPageUpPress)
		camera.Move(glm::vec3(0, 0, CAMERA_MOVE_SPEED));
	if (IsPageDownPress)
		camera.Move(glm::vec3(0, 0, -CAMERA_MOVE_SPEED));

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		isMouseLeftPress = true;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		lastLX = xpos;
		lastLY = ypos;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		isMouseLeftPress = false;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		isMouseRightPress = true;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		lastRX = xpos;
		lastRY = ypos;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		isMouseRightPress = false;
	}

	
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	// 旋转
	if (isMouseLeftPress)
	{
		float xoffset = xpos - lastLX;
		float yoffset = lastLY - ypos; // reversed since y-coordinates go from bottom to top
		lastLX = xpos;
		lastLY = ypos;
		//printf("xoffset: %f, yoffset: %f", xoffset, yoffset);
		if (glm::abs(xoffset) > 1e-6 || glm::abs(yoffset) > 1e-6)
		{
			camera.ProcessLBMouseMovement(xoffset, yoffset);
		}
	}
	// 平移
	else if (isMouseRightPress)
	{
		float xoffset = xpos - lastRX;
		float yoffset = lastRY - ypos; 
		lastRX = xpos;
		lastRY = ypos;// reversed since y-coordinates go from bottom to top
		camera.ProcessRBMouseMovement(xoffset, yoffset);
	}
	
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(xoffset, yoffset);
	//printf("xoffset: %f, yoffset: %f\n", xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		IsLeftPress = true;
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		IsRightPress = true;
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		IsUpPress = true;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		IsDownPress = true;
	if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
		IsPageUpPress = true;
	if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
		IsPageDownPress = true;


	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
		IsLeftPress = false;
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
		IsRightPress = false;
	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		IsUpPress = false;
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		IsDownPress = false;
	if (key == GLFW_KEY_PAGE_UP && action == GLFW_RELEASE)
		IsPageUpPress = false;
	if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_RELEASE)
		IsPageDownPress = false;

}