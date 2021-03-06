
#include <iostream>
#include "MKApp.h"


#define TEST1
#ifndef TEST
int main()
{
	MKApp *myMKApp = new MKApp();
	if (myMKApp->init())
	{
		myMKApp->run();
	}

	delete myMKApp;
	return 0;
}
#endif

#ifdef TEST

#include "MKCommon.h"
#include "MKCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;
int main()
{
	MKFrustum * frustum = new MKFrustum();
	// test frustum compute
	frustum->ComputeCorners();
	cout << "corners: " << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << frustum->m_corners[i].x << " " << frustum->m_corners[i].y << " " << frustum->m_corners[i].z << endl;
	}

	glm::mat4 view = glm::lookAt(vec3(0,0,3), vec3(0,0,0), vec3(0,1,0));
	glm::mat4 persp = frustum->GetPersp();
	frustum->ComputePlanes( persp * view);
	frustum->ComputeFrustumSphere();

	MKAABBBoundingBox aabb;
	aabb.m_DownLeftPoint = vec3(-71.0, -8.0, -35.0);
	aabb.m_UpRightPoint = vec3(11.0, 8.0, -30.0);

	// test frustum intersect aabb
	
	//cout << frustum->IntersectWithBBoxAABB(aabb);
	// test frustum intersect sphere

	MKSphereBoundingBox sbbox;
	sbbox.m_center = glm::vec3(0, 0, -100);
	sbbox.m_radius = 4.0f;
	//cout << frustum->m_sphereBBox.Intersect(sbbox);
	cout << frustum->IntersectWithBBoxSphere(sbbox);
	MKPlane plane;
	
	return 0;
}


#endif

