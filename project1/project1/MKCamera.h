#ifndef MK_CAMERA
#define MK_CAMERA

#include <glm/glm.hpp>

#include <glm/common.hpp> 

#include <vector>
#include "MKType.h"
#include "MKCommon.h"
#include "MKFrustum.h"

class MKFrustum;



static void printMatrix(glm::mat4 &m)
{
	for (int r = 0; r < 4; r++)
	{
		printf("%f %f %f %f\n", m[0][r], m[1][r], m[2][r], m[3][r]);
	}
	printf("--------------\n");
}



//用来计算view matrix
class MKCamera
{
public:
	// 相机位置
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;
	glm::vec3 m_viewPoint;

	// MVP matrix
	glm::mat4 m_model;
	glm::mat4 m_view;

	float m_scaleNum;
	float m_totalScale;
	glm::mat4 m_scale;
	glm::mat4 m_rotate;
	glm::mat4 m_translate;
	// Euler Angles
	float Yaw;
	float Pitch;
	// 视锥
	MKFrustum *m_frustum;

	MKPlane m_frustumPlanes[6];//0~5 near,far,left,right,top,bottom
	
	//
	float Zoom;
	MKCamera();
	// Constructor with vectors
	MKCamera(glm::vec3 position, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	// Constructor with scalar values
	MKCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	void CreateFrusdumMesh();
	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();
	glm::mat4 GetModelMatrix();
	glm::mat4 GetPerspectiveMatrix();

	void Move(glm::vec3 dir);
	// 处理外部键鼠操作
	void ProcessLBMouseMovement(float xoffset, float yoffset);
	
	void ProcessMouseScroll(float xoffset, float yoffset);

	void ProcessRBMouseMovement(float xoffset, float yoffset);
    
};

extern MKCamera camera;
#endif

