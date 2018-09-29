#ifndef MKCOMMON_H
#define MKCOMMON_H
#include <float.h>
#include <glm/glm.hpp>

struct MKSphereBoundingBox
{
	glm::vec3 m_center;
	float m_radius;
	bool Intersect(MKSphereBoundingBox &ref)
	{
		glm::vec3 diff = m_center - ref.m_center;
		float centerDistanceSquare = diff.x * diff.x * +diff.y * diff.y * +diff.z * diff.z;
		float raidusSumSquare = (m_radius + ref.m_radius) * (m_radius + ref.m_radius);
		return centerDistanceSquare < raidusSumSquare;
	}
};

struct MKAABBBoundingBox
{
	MKAABBBoundingBox()
		:m_DownLeftPoint(FLT_MAX, FLT_MAX, FLT_MAX), m_UpRightPoint(-FLT_MAX, -FLT_MAX, -FLT_MAX)
	{
	}
	glm::vec3 m_DownLeftPoint;
	glm::vec3 m_UpRightPoint;
};


#endif