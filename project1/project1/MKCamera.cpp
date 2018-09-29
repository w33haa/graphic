#include "MKCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MKFrustum.h"
// camera
//MKCamera camera(glm::vec3(0.0f, 0.0f, 5.0f));
MKCamera camera;

MKCamera::MKCamera()
{
	// 相机位置
	m_position = glm::vec3(0.0f, 0.0f, 8.0f);


	m_viewPoint = glm::vec3(0.0, 0.0, 0.0);

	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_front = glm::normalize(m_viewPoint - m_position);
	m_right = glm::normalize(glm::cross(m_front, m_up));

	// 视锥参数
	m_frustum = new MKFrustum();
	m_frustum->ComputeCorners();
	m_frustum->ComputeFrustumSphere();

	glm::mat4 view = glm::lookAt(m_position, m_viewPoint, m_up);
	glm::mat4 persp = m_frustum->GetPersp();
	m_frustum->ComputePlanes(persp * view);
	//MVP
	m_model = glm::mat4(1.0f);


	// 模型的缩放旋转平移
	m_scale = glm::mat4(1.0f);
	m_totalScale = 1.0f;
	m_rotate = glm::mat4(1.0f);
	m_translate = glm::mat4(1.0f);

	Zoom = ZOOM;
	m_scaleNum = 1.0f;
	
}

MKCamera::MKCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: m_front(glm::vec3(0.0f, 0.0f, -1.0f)), Zoom(ZOOM), m_viewPoint(0.0, 0.0, 0.0), m_model(1.0f), m_scaleNum(1.0f), m_scale(1.0f), m_totalScale(1.0f),
	m_rotate(1.0f), m_translate(1.0)
{
	m_position = position;
	Yaw = yaw;
	Pitch = pitch;
	m_up = m_worldUp = up;
	m_front = glm::normalize(m_viewPoint - m_position);
	m_right = glm::normalize(glm::cross(m_front, m_up));

}



MKCamera::MKCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: m_front(glm::vec3(0.0f, 0.0f, -1.0f)), Zoom(ZOOM)
{
	m_position = glm::vec3(posX, posY, posZ);
	m_worldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	m_up = m_worldUp = glm::vec3(0.0, 1.0, 0.0);
	m_front = glm::normalize(m_viewPoint - m_position);
	m_right = glm::normalize(glm::cross(m_front, m_up));
}

void MKCamera::CreateFrusdumMesh()
{
	m_frustum->CreateMesh();
}
// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 MKCamera::GetViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}
glm::mat4 MKCamera::GetModelMatrix()
{
	return m_model;
}


glm::mat4 MKCamera::GetPerspectiveMatrix()
{
	return m_frustum->GetPersp();
}

void MKCamera::Move(glm::vec3 offset)
{
	m_position += offset.x * m_right + offset.y * m_up + offset.z * m_front;
	m_viewPoint += offset.x * m_right + offset.y * m_up + offset.z * m_front;

	// 重新计算视锥
	m_frustum->ComputeCorners();
	m_frustum->ComputeFrustumSphere();
	m_frustum->CreateMesh();

	glm::mat4 view = glm::lookAt(m_position, m_viewPoint, m_up);
	glm::mat4 persp = m_frustum->GetPersp();
	m_frustum->ComputePlanes(persp * view);
	
}
// 处理外部键鼠操作
void MKCamera::ProcessLBMouseMovement(float xoffset, float yoffset)
{
	//改变相机位置
	//鼠标左键拖动旋转相机
	//旋转方向
	glm::vec3 mouseTrace = -m_up * yoffset - xoffset * m_right;
	//旋转轴
	glm::vec3 rotateAsix = glm::normalize(glm::cross(m_front, mouseTrace));
	float angle = -0.01 * glm::length(mouseTrace);
	//改变相机位置
	glm::mat4 rotatMatrix = glm::rotate(glm::mat4(1.0), angle, rotateAsix);
	
	glm::vec4 t = rotatMatrix * glm::vec4(m_position, 1);
	m_position = glm::vec3(t);

	t = rotatMatrix * glm::vec4(m_right, 1);
	m_right = glm::vec3(t);

	
	m_front = glm::normalize(m_viewPoint - m_position);

	// 使用worldUp计算m_right，当m_front和m_worldUp共线时，会导致bug
	//m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
	//printf("m_front: %f, %f, %f\n", m_front.x, m_front.y, m_front.z);
	//printf("m_right: %f, %f, %f\n", m_right.x, m_right.y, m_right.z);
	//printf("m_up: %f, %f, %f\n", m_up.x, m_up.y, m_up.z);
	//printf("rotatMatrix: \n");



	////或者改变模型位置
	//glm::vec3 mouseTrace = m_up * yoffset + m_right * xoffset;
	////glm::vec3 mouseTrace = m_right * xoffset;
	////旋转轴
	//glm::vec3 rotateAsix = glm::cross(mouseTrace, m_front);
	//rotateAsix = normalize(rotateAsix);
	////旋转角度
	//float angle = 0.01 * glm::length(mouseTrace);
	//m_model = glm::rotate(glm::mat4(1.0), angle, rotateAsix) * m_model;
	//m_rotate = glm::rotate(glm::mat4(1.0), angle, rotateAsix) * m_rotate;
}

void MKCamera::ProcessMouseScroll(float xoffset, float yoffset)
{
	//glm::mat4 scale = glm::mat4(0.01) * yoffset;
	m_scaleNum = 1.0 + 0.1 * yoffset;
	if (yoffset < 0.0f && (m_totalScale + 0.1 * yoffset) <= 0.2)
	{
		m_scaleNum = 1.0;
	}
	else if (yoffset > 0.0f && (m_totalScale + 0.1 * yoffset) >= 5.0)
	{
		m_scaleNum = 1.0;
	}
	else
	{
		m_totalScale += 0.1 * yoffset;
	}
	m_scale = glm::scale(glm::mat4(1.0), glm::vec3(m_scaleNum));
	m_scale[3][3] = 1;
	//m_scale *= m_scale
	m_model = m_scale * m_model;
	//printMatrix(m_scale);
}

void MKCamera::ProcessRBMouseMovement(float xoffset, float yoffset)
{
	m_translate = glm::translate(glm::mat4(1), glm::vec3(0.01 * xoffset, 0.01 *  yoffset, 0.0));
	m_model = m_translate * m_model;
	//printMatrix(m_translate);
}