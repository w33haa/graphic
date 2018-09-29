#ifndef MKFRUSTUM_H
#define MKFRUSTUM_H

#include <glm/glm.hpp>
#include "MKCommon.h"
#include "MKGlobal.h"

class MKMesh;
struct MKPlane
{
	float a, b, c, d;
	glm::vec3 Normal()
	{
		return glm::vec3(a, b, c);
	}
	float SignedDistance(glm::vec3 p)
	{
		return a * p.x + b * p.y + c * p.z + d;
	}
	void Normalize()
	{
		float l = sqrt(a*a + b * b + c * c);
		a /= l;
		b /= l;
		c /= l;
		d /= l;
	}
};

struct MKFrustum
{
	float m_fovy;
	float m_aspect;
	float m_near;
	float m_far;
	MKPlane m_planes[6];//0~5 near,far,left,right,top,bottom

	glm::vec3 m_corners[8];
	MKSphereBoundingBox m_sphereBBox;

	MKMesh *m_mesh;
	MKFrustum()
	{
		/*m_fovy = 45;
		m_aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
		m_near = 0.1f;
		m_far = 100.0f;*/
		m_fovy = 45;
		m_aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
		m_near = 1.0f;
		m_far = 3.0f;

	}
	glm::mat4 GetPersp();
	void ComputePlanes(const glm::mat4 &comboMatrix);
	void ComputeCorners();
	void ComputeFrustumSphere();
	
	bool IntersectWithBBoxSphere(MKSphereBoundingBox &sbbox);
	bool IntersectWithBBoxAABB(MKAABBBoundingBox &abbbox);
	void CreateMesh();
	void Draw();
};




#endif