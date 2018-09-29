#ifndef MKMODEL_H
#define MKMODEL_H


#include "MKMesh.h"
#include <vector>
#include <string>
#include <sstream>
#include <deque>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tinyxml2/tinyxml2.h>
#include "MKGlobal.h"
#include <float.h>


using namespace std;

class MKSceneNode
{
public:
	string m_meshName;
	string m_materialName;
	MKMesh *m_mesh;
	MKMesh *m_bboxMesh;
	MKSceneNode *m_parentNode;
	// 调试用
	uint32 m_id;
	
	void ComputeBoundingBox();
	void CreateBoundingBoxMesh();
	void CreateBoundingBoxMeshSubTree();


	void DrawBoundingBox(bool isLeafBBox);

	// 相对父节点的model变换
	glm::mat4 m_modelToParent;
	// 子节点
	vector<MKSceneNode *> m_childNodes;

	//包围盒
	MKSphereBoundingBox m_sphere_BBox;
	MKAABBBoundingBox m_AABB_BBox;

	MKSceneNode() {}
	MKSceneNode(MKMesh *mesh, glm::mat4 model) :m_mesh(mesh), m_modelToParent(model)
	{

	}

	void DrawNodeRecursive(glm::mat4 &parentLevelModel);
	
	

	glm::mat4 getModelToWorld();
	
};


class MKScene
{
public:

	MKSceneNode m_rootNode;
	
	
	MKScene(){}
	
	void Draw()
	{
		drawCallPerLoop = 0;
		m_rootNode.DrawNodeRecursive(m_rootNode.m_modelToParent);
		//m_rootNode.DrawBoundingBox();
	}

	void LoadFromXML(const char *path);


	uint32 m_bBoxVao;
	uint32 m_bBoxVbo;
	uint32 m_bBoxEbo;
	

};
#endif