#ifndef MK_BILLBORD_H
#define MK_BILLBORD_H

#include <vector>
#include <glm/glm.hpp>
#include "MKType.h"
#include "MKCamera.h"
#include "MKPartices.h"
#include "MKMesh.h"

using namespace std;
using namespace glm;
class MKBillBord
{
public:
	vec3 m_center;
	float m_width, m_height;
	
	vector<vec3> m_localVertices;
	vector<uint32> m_indices;
	MKBlackHole *m_partices;
	MKMesh *m_mesh;
	MKBillBord(vec3 center, MKBlackHole *partices) :m_center(center), m_partices(partices)
	{
	}

	MKBillBord(vec3 center, MKMesh *mesh) :m_center(center), m_mesh(mesh)
	{
		// 设置不同的sampler
	}

	void Render()
	{
		// 计算Billbord朝向
		vec3 bFront = glm::normalize(camera.m_position - m_center);
		vec3 bRight;
		bRight = glm::normalize(glm::cross(bFront, camera.m_up));
		/*if(bFront.z >= 0.0)
		{
			bRight = glm::normalize(glm::cross(bFront, vec3(0,1.0,0)));
		}
		else
		{
			bRight = glm::normalize(glm::cross(vec3(0,1.0,0), bFront));
		}*/
		
		vec3 bUp = glm::normalize(glm::cross(bRight, bFront));

		mat4 mLocalToWorld(vec4(bRight, 0), vec4(bUp, 0), vec4(bFront, 0), vec4(0, 0, 0, 1.0));
		mat4 tran = glm::translate(mat4(1.0), m_center);
		/*printf("front: %f, %f, %f, right: %f, %f, %f, up: %f, %f, %f\n",
			bFront.x, bFront.y, bFront.z, bRight.x, bRight.y, bRight.z, bUp.x, bUp.y, bUp.z);*/
		mLocalToWorld[3] = vec4(m_center, 1.0);
		if (m_partices)
		{
			m_partices->Update(mLocalToWorld);
		}
		if (m_mesh)
		{
			m_mesh->Draw(mLocalToWorld,frameTime);
		}
		//m_mesh->Draw(mLocalToWorld);
		//m_mesh->Draw(mat4(1.0));
		
	}

};


#endif