#include "MKFrustum.h"

#include <glm/gtc/matrix_transform.hpp>
#include "MKCamera.h"
#include "MKMesh.h"
extern MKCamera camera;
glm::mat4 MKFrustum::GetPersp()
{
	return glm::perspective(glm::radians(m_fovy), m_aspect, m_near, m_far);
}


void MKFrustum::ComputePlanes(const glm::mat4 &comboMatrix)
{
	// Left clipping plane    
	m_planes[0].a = comboMatrix[0][3] + comboMatrix[0][0];
	m_planes[0].b = comboMatrix[1][3] + comboMatrix[1][0];
	m_planes[0].c = comboMatrix[2][3] + comboMatrix[2][0];
	m_planes[0].d = comboMatrix[3][3] + comboMatrix[3][0];
	// Right clipping plane
	m_planes[1].a = comboMatrix[0][3] - comboMatrix[0][0];
	m_planes[1].b = comboMatrix[1][3] - comboMatrix[1][0];
	m_planes[1].c = comboMatrix[2][3] - comboMatrix[2][0];
	m_planes[1].d = comboMatrix[3][3] - comboMatrix[3][0];
	// Top clipping plane
	m_planes[2].a = comboMatrix[0][3] - comboMatrix[0][1];
	m_planes[2].b = comboMatrix[1][3] - comboMatrix[1][1];
	m_planes[2].c = comboMatrix[2][3] - comboMatrix[2][1];
	m_planes[2].d = comboMatrix[3][3] - comboMatrix[3][1];
	// Bottom clipping plane
	m_planes[3].a = comboMatrix[0][3] + comboMatrix[0][1];
	m_planes[3].b = comboMatrix[1][3] + comboMatrix[1][1];
	m_planes[3].c = comboMatrix[2][3] + comboMatrix[2][1];
	m_planes[3].d = comboMatrix[3][3] + comboMatrix[3][1];
	// Near clipping plane
	m_planes[4].a = comboMatrix[0][3] + comboMatrix[0][2];
	m_planes[4].b = comboMatrix[1][3] + comboMatrix[1][2];
	m_planes[4].c = comboMatrix[2][3] + comboMatrix[2][2];
	m_planes[4].d = comboMatrix[3][3] + comboMatrix[3][2];
	// Far clipping plane
	m_planes[5].a = comboMatrix[0][3] - comboMatrix[0][2];
	float c13 = comboMatrix[1][3];
	float c12 = comboMatrix[1][2];
	m_planes[5].b = comboMatrix[1][3] - comboMatrix[1][2];
	m_planes[5].c = comboMatrix[2][3] - comboMatrix[2][2];
	m_planes[5].d = comboMatrix[3][3] - comboMatrix[3][2];
	// Normalize the plane equations, if requested

	for (int i = 0; i < 6; i++)
	{
		m_planes[i].Normalize();
	}
	
}

void MKFrustum::ComputeCorners()
{
	// 计算corner的world坐标
	// 计算近远截面的宽和高
	float nearPlaneHeight = m_near * tan(glm::radians(m_fovy * 0.5));
	float nearPlaneWidth = nearPlaneHeight * m_aspect;

	float farPlaneHeight = m_far * tan(glm::radians(m_fovy * 0.5));
	float farPlaneWidth = farPlaneHeight * m_aspect;

	m_corners[0] = camera.m_position + nearPlaneWidth * camera.m_right + nearPlaneHeight * camera.m_up + m_near * camera.m_front;
	m_corners[1] = camera.m_position + -nearPlaneWidth * camera.m_right + nearPlaneHeight * camera.m_up + m_near * camera.m_front;
	m_corners[2] = camera.m_position + -nearPlaneWidth * camera.m_right - nearPlaneHeight * camera.m_up + m_near * camera.m_front;
	m_corners[3] = camera.m_position + nearPlaneWidth * camera.m_right - nearPlaneHeight * camera.m_up + m_near * camera.m_front;

	m_corners[4] = camera.m_position + farPlaneWidth * camera.m_right + farPlaneHeight * camera.m_up + m_far * camera.m_front;
	m_corners[5] = camera.m_position + -farPlaneWidth * camera.m_right + farPlaneHeight * camera.m_up + m_far * camera.m_front;
	m_corners[6] = camera.m_position + -farPlaneWidth * camera.m_right - farPlaneHeight * camera.m_up + m_far * camera.m_front;
	m_corners[7] = camera.m_position + farPlaneWidth * camera.m_right - farPlaneHeight * camera.m_up + m_far * camera.m_front;
}

void MKFrustum::ComputeFrustumSphere()
{
	// 计算视锥包围球半径
	// 先计算远裁面与近裁面的距离
	float viewLen = m_far - m_near;
	// 计算远裁面的半高
	float fHeight = m_far * tan(glm::radians(m_fovy * 0.5f));
	// 计算远平面的半宽
	float fWidth = fHeight * m_aspect;

	// 确定远裁面顶点P，在view空间下的坐标
	glm::vec3 P(0.0f, 0.0f, m_near + viewLen * 0.5f);
	// 确定Q点
	glm::vec3 Q(fWidth, fHeight, viewLen);
	// 获得一个半径向量
	glm::vec3 vDiff(P - Q);
	// 获取半径
	m_sphereBBox.m_radius = glm::length(vDiff);
	// 计算球心
	m_sphereBBox.m_center = camera.m_position + camera.m_front * (m_near + viewLen * 0.5f);
}

#if 1
bool MKFrustum::IntersectWithBBoxSphere(MKSphereBoundingBox &sbbox)
{
	float sd;
	//遍历所有裁面并计算
	for (int i = 0; i < 6; ++i)
	{
		// 计算距离
		sd = m_planes[i].SignedDistance(sbbox.m_center);
		// 不相交
		if (-sd >= sbbox.m_radius)
		{
			return false;
		}
		
	}
	return true;
}
#endif
bool MKFrustum::IntersectWithBBoxAABB(MKAABBBoundingBox &abbbox)
{
	float minX, minY, minZ, maxX, maxY, maxZ;
	minX = abbbox.m_DownLeftPoint.x;
	minY = abbbox.m_DownLeftPoint.y;
	minZ = abbbox.m_DownLeftPoint.z;

	maxX = abbbox.m_UpRightPoint.x;
	maxY = abbbox.m_UpRightPoint.y;
	maxZ = abbbox.m_UpRightPoint.z;

	// 包围盒的8个点如果全在Frustum的某个面外，则不相交
	for (int i = 0; i<6; i++)
	{
		int out = 0;
		out += ((m_planes[i].SignedDistance(glm::vec3(minX, minY, minZ)) < 0.0) ? 1 : 0);
		out += ((m_planes[i].SignedDistance(glm::vec3(minX, minY, maxZ)) < 0.0) ? 1 : 0);
		out += ((m_planes[i].SignedDistance(glm::vec3(minX, maxY, minZ)) < 0.0) ? 1 : 0);
		out += ((m_planes[i].SignedDistance(glm::vec3(minX, maxY, maxZ)) < 0.0) ? 1 : 0);
		out += ((m_planes[i].SignedDistance(glm::vec3(maxX, minY, minZ)) < 0.0) ? 1 : 0);
		out += ((m_planes[i].SignedDistance(glm::vec3(maxX, minY, maxZ)) < 0.0) ? 1 : 0);
		out += ((m_planes[i].SignedDistance(glm::vec3(maxX, maxY, minZ)) < 0.0) ? 1 : 0);
		out += ((m_planes[i].SignedDistance(glm::vec3(maxX, maxY, maxZ)) < 0.0) ? 1 : 0);

		if (out == 8) return false;
	}

	// 继续检测，Frustum的8个点如果在包围盒的某个面外，则不相交
	int out;
	out = 0; for (int i = 0; i<8; i++) out += ((m_corners[i].x > maxX) ? 1 : 0); if (out == 8) return false;
	out = 0; for (int i = 0; i<8; i++) out += ((m_corners[i].x < minX) ? 1 : 0); if (out == 8) return false;
	out = 0; for (int i = 0; i<8; i++) out += ((m_corners[i].y > maxY) ? 1 : 0); if (out == 8) return false;
	out = 0; for (int i = 0; i<8; i++) out += ((m_corners[i].y < minY) ? 1 : 0); if (out == 8) return false;
	out = 0; for (int i = 0; i<8; i++) out += ((m_corners[i].z > maxZ) ? 1 : 0); if (out == 8) return false;
	out = 0; for (int i = 0; i<8; i++) out += ((m_corners[i].z < minZ) ? 1 : 0); if (out == 8) return false;


	return true;
}

void MKFrustum::CreateMesh()
{
	// vertices
	//MKMesh(vector<glm::vec3> &position, vector<uint32> &indices)
	uint32 indices[] = { 0,1,1,2,2,3,3,0,4,5,5,6,6,7,7,4,0,4,4,5,5,1,1,0,3,7,7,6,6,2,2,3,7,4,4,0,0,3,3,7,2,1,1,5,5,6,6,2 }; 
	vector<uint32> uintv;
	uintv.assign(begin(indices), end(indices));
	vector<glm::vec3> pv;
	pv.assign(begin(m_corners), end(m_corners));
	m_mesh = new MKMesh(pv, uintv);
	MKMaterial *simpleMaterial = new MKMaterial(defaultShader);
	simpleMaterial->m_primitive = GL_LINES;
	m_mesh->UseMaterial(simpleMaterial);

	
}

void  MKFrustum::Draw()
{
	if (m_mesh)
	{
		m_mesh->m_material->m_shader->use();
		m_mesh->m_material->m_shader->setVec4("color", glm::vec4(1.0, 0, 0, 1.0));
		m_mesh->Draw(camera.GetModelMatrix());
		//m_mesh->Draw(glm::mat4(1.0));
	}
}