#include "MKScene.h"
#include <list>
#include "MKCamera.h"

extern MKCamera camera;
void MKSceneNode::DrawNodeRecursive(glm::mat4 &parentLevelModel)
{
	glm::mat4 currentModel = parentLevelModel * m_modelToParent;
	if (m_mesh)
	{
		if (camera.m_frustum->IntersectWithBBoxSphere(m_mesh->m_sphere_BBox))
		{
			if (camera.m_frustum->IntersectWithBBoxAABB(m_mesh->m_AABB_BBox))
			{
				m_mesh->Draw(camera.GetModelMatrix() * currentModel);
				drawCallPerLoop++;
			}
		}
		
	}
	bool isLeaf = false;
	if (m_childNodes.size() == 0)
	{
		isLeaf = true;
	}
	DrawBoundingBox(isLeaf);
	drawCallPerLoop++;

	for (int i = 0; i < m_childNodes.size(); i++)
	{
		if (camera.m_frustum->IntersectWithBBoxSphere(m_childNodes[i]->m_sphere_BBox))
		{
			if (camera.m_frustum->IntersectWithBBoxAABB(m_childNodes[i]->m_AABB_BBox))
			{
				m_childNodes[i]->DrawNodeRecursive(currentModel);
			}
		}
	}
}

void MKSceneNode::ComputeBoundingBox()
{
	if (m_mesh != NULL)
	{
		glm::mat4 modelToWorld = getModelToWorld();
		m_mesh->ComputeBoundingBox(modelToWorld);
		m_AABB_BBox = m_mesh->m_AABB_BBox;
		m_sphere_BBox = m_mesh->m_sphere_BBox;
	}

	for (int i = 0; i < m_childNodes.size(); i++)
	{
		m_childNodes[i]->ComputeBoundingBox();
		glm::vec3 childDownLeftPoint = m_childNodes[i]->m_AABB_BBox.m_DownLeftPoint;
		m_AABB_BBox.m_DownLeftPoint.x = childDownLeftPoint.x < m_AABB_BBox.m_DownLeftPoint.x ? childDownLeftPoint.x : m_AABB_BBox.m_DownLeftPoint.x;
		m_AABB_BBox.m_DownLeftPoint.y = childDownLeftPoint.y < m_AABB_BBox.m_DownLeftPoint.y ? childDownLeftPoint.y : m_AABB_BBox.m_DownLeftPoint.y;
		m_AABB_BBox.m_DownLeftPoint.z = childDownLeftPoint.z < m_AABB_BBox.m_DownLeftPoint.z ? childDownLeftPoint.z : m_AABB_BBox.m_DownLeftPoint.z;

		glm::vec3 childUpRightPoint = m_childNodes[i]->m_AABB_BBox.m_UpRightPoint;
		m_AABB_BBox.m_UpRightPoint.x = childUpRightPoint.x > m_AABB_BBox.m_UpRightPoint.x ? childUpRightPoint.x : m_AABB_BBox.m_UpRightPoint.x;
		m_AABB_BBox.m_UpRightPoint.y = childUpRightPoint.y > m_AABB_BBox.m_UpRightPoint.y ? childUpRightPoint.y : m_AABB_BBox.m_UpRightPoint.y;
		m_AABB_BBox.m_UpRightPoint.z = childUpRightPoint.z > m_AABB_BBox.m_UpRightPoint.z ? childUpRightPoint.z : m_AABB_BBox.m_UpRightPoint.z;
	}
	m_sphere_BBox.m_center = m_AABB_BBox.m_DownLeftPoint + m_AABB_BBox.m_UpRightPoint;
	m_sphere_BBox.m_center /= 2;
	m_sphere_BBox.m_radius = (m_AABB_BBox.m_DownLeftPoint - m_AABB_BBox.m_UpRightPoint).length() / 2;
	this->CreateBoundingBoxMesh();
}

glm::mat4 MKSceneNode::getModelToWorld()
{
	glm::mat4 r(1.0);
	MKSceneNode *p = this;
	std::list<MKSceneNode *> plist;
	while (p)
	{
		plist.push_front(p);
		p = p->m_parentNode;
	}
	for (std::list<MKSceneNode *>::iterator it = plist.begin(); it != plist.end(); it++)
	{
		r *= (*it)->m_modelToParent;
	}
	return r;
}

void MKSceneNode::CreateBoundingBoxMesh()
{
	vector<glm::vec3> vertices;
	vector<uint32> indices;

	float lx = m_AABB_BBox.m_DownLeftPoint.x;
	float ly = m_AABB_BBox.m_DownLeftPoint.y;
	float lz = m_AABB_BBox.m_DownLeftPoint.z;
	float hx = m_AABB_BBox.m_UpRightPoint.x;
	float hy = m_AABB_BBox.m_UpRightPoint.y;
	float hz = m_AABB_BBox.m_UpRightPoint.z;
	vertices.push_back(glm::vec3(lx, ly, lz));
	vertices.push_back(glm::vec3(lx, ly, hz));
	vertices.push_back(glm::vec3(hx, ly, hz));
	vertices.push_back(glm::vec3(hx, ly, lz));
	vertices.push_back(glm::vec3(lx, hy, lz));
	vertices.push_back(glm::vec3(lx, hy, hz));
	vertices.push_back(glm::vec3(hx, hy, hz));
	vertices.push_back(glm::vec3(hx, hy, lz));

	int poffset[] = { 0,1,1,2,2,3,3,0,0,4,1,5,2,6,3,7,4,5,5,6,6,7,7,4 };

	indices.assign(begin(poffset), end(poffset));
	m_bboxMesh = new MKMesh(vertices, indices);
	MKMaterial *simpleMaterial = new MKMaterial(defaultShader);
	simpleMaterial->m_primitive = GL_LINES;
	m_bboxMesh->UseMaterial(simpleMaterial);
}

void MKSceneNode::CreateBoundingBoxMeshSubTree()
{
	deque<MKSceneNode *> sceneNodeQueue;
	MKSceneNode *snode;
	vector<glm::vec3> vertices;
	vector<uint32> indices;


	sceneNodeQueue.push_back(this);
	glm::mat4 model;
	while (!sceneNodeQueue.empty())
	{
		snode = sceneNodeQueue.front();

		sceneNodeQueue.pop_front();
		float lx = snode->m_AABB_BBox.m_DownLeftPoint.x;
		float ly = snode->m_AABB_BBox.m_DownLeftPoint.y;
		float lz = snode->m_AABB_BBox.m_DownLeftPoint.z;
		float hx = snode->m_AABB_BBox.m_UpRightPoint.x;
		float hy = snode->m_AABB_BBox.m_UpRightPoint.y;
		float hz = snode->m_AABB_BBox.m_UpRightPoint.z;

		int base = vertices.size();
		int poffset[] = { 0,1,1,2,2,3,3,0,0,4,1,5,2,6,3,7,4,5,5,6,6,7,7,4 };
		for (int i = 0; i < sizeof(poffset)/sizeof(int); i++)
		{
			poffset[i] += base;
		}
		indices.insert(indices.end(), begin(poffset), end(poffset));

		vertices.push_back(glm::vec3(lx, ly, lz));
		vertices.push_back(glm::vec3(lx, ly, hz));
		vertices.push_back(glm::vec3(hx, ly, hz));
		vertices.push_back(glm::vec3(hx, ly, lz));
		vertices.push_back(glm::vec3(lx, hy, lz));
		vertices.push_back(glm::vec3(lx, hy, hz));
		vertices.push_back(glm::vec3(hx, hy, hz));
		vertices.push_back(glm::vec3(hx, hy, lz));

		for (int i = 0; i < snode->m_childNodes.size(); i++)
		{
			sceneNodeQueue.push_back(snode->m_childNodes[i]);
			model *= snode->m_childNodes[i]->m_modelToParent;
		}
	}
	m_bboxMesh = new MKMesh(vertices, indices);
	MKMaterial *simpleMaterial = new MKMaterial(defaultShader);
	simpleMaterial->m_primitive = GL_LINES;
	m_bboxMesh->UseMaterial(simpleMaterial);
}
#if 0
void MKSceneNode::CreateBoundingBoxMesh()
{
	deque<MKSceneNode *> sceneNodeQueue;
	deque<glm::mat4> sceneModelQueue;
	MKSceneNode *snode;
	vector<glm::vec3> vertices;
	vector<uint32> indices;


	sceneNodeQueue.push_back(this);
	sceneModelQueue.push_back(this->m_modelToParent);
	glm::mat4 model;
	while (!sceneNodeQueue.empty())
	{
		snode = sceneNodeQueue.front();
		model = sceneModelQueue.front();

		sceneNodeQueue.pop_front();
		sceneModelQueue.pop_front();
		float lx = snode->m_AABB_BBox.m_DownLeftPoint.x;
		float ly = snode->m_AABB_BBox.m_DownLeftPoint.y;
		float lz = snode->m_AABB_BBox.m_DownLeftPoint.z;
		float hx = snode->m_AABB_BBox.m_UpRightPoint.x;
		float hy = snode->m_AABB_BBox.m_UpRightPoint.y;
		float hz = snode->m_AABB_BBox.m_UpRightPoint.z;

		vertices.push_back(glm::vec3(model*glm::vec4(lx, ly, lz, 1)));
		vertices.push_back(glm::vec3(model*glm::vec4(lx, ly, hz, 1)));
		vertices.push_back(glm::vec3(model*glm::vec4(hx, ly, hz, 1)));
		vertices.push_back(glm::vec3(model*glm::vec4(hx, ly, lz, 1)));
		vertices.push_back(glm::vec3(model*glm::vec4(lx, hy, lz, 1)));
		vertices.push_back(glm::vec3(model*glm::vec4(lx, hy, hz, 1)));
		vertices.push_back(glm::vec3(model*glm::vec4(hx, hy, hz, 1)));
		vertices.push_back(glm::vec3(model*glm::vec4(lx, hy, lz, 1)));

		int base = vertices.size();
		int poffset[] = { 0,1,1,2,2,3,3,0,0,4,1,5,2,6,3,7,4,5,5,6,6,7,7,4 };
		for (int i = 0; i < indices.size(); i++)
		{
			indices[i] += base;
		}
		indices.insert(indices.end(), begin(poffset), end(poffset));

		for (int i = 0; i < snode->m_childNodes.size(); i++)
		{
			sceneNodeQueue.push_back(snode->m_childNodes[i]);
			model *= snode->m_childNodes[i]->m_modelToParent;
			sceneModelQueue.push_back(model);
		}
	}
	m_bboxMesh = new MKMesh(vertices, indices);
	MKMaterial *simpleMaterial = new MKMaterial(defaultShader);
	simpleMaterial->m_primitive = GL_LINES;
	m_bboxMesh->UseMaterial(simpleMaterial);
}
#endif



void MKSceneNode::DrawBoundingBox(bool isLeafBBox)
{
	m_bboxMesh->m_material->m_shader->use();
	if (isLeafBBox)
	{
		m_bboxMesh->m_material->m_shader->setVec4("color", glm::vec4(1.0, 0.0, 1.0, 1.0));
	}
	else
	{
		m_bboxMesh->m_material->m_shader->setVec4("color", glm::vec4(1.0, 1.0, 0, 1.0));
	}
	
	
	m_bboxMesh->Draw(camera.GetModelMatrix());
}

void MKScene::LoadFromXML(const char *path)
{
	tinyxml2::XMLDocument doc;

	doc.LoadFile(path);

	deque<tinyxml2::XMLNode *> xmlNodeQueue;
	deque<MKSceneNode *> modelNodeQueue;
	tinyxml2::XMLNode * elem = doc.FirstChildElement("Scene"); //->FirstChildElement("Model") 
	xmlNodeQueue.push_back(elem);
	modelNodeQueue.push_back(&m_rootNode);

	m_rootNode.m_modelToParent = glm::mat4(1.0);
	m_rootNode.m_parentNode = NULL;

	// 层次遍历xml树，同时创建MKScene结构
	while (!xmlNodeQueue.empty())
	{
		tinyxml2::XMLNode *xNode = xmlNodeQueue.front();
		xmlNodeQueue.pop_front();
		tinyxml2::XMLElement  *xNodeChild = xNode->FirstChildElement("Model");

		MKSceneNode * parentTreeNode = modelNodeQueue.front();
		modelNodeQueue.pop_front();

		for (; xNodeChild; xNodeChild = xNodeChild->NextSiblingElement("Model"))
		{
			//cout << xNodeChild->Value() << endl;
			MKSceneNode *childTreeNode = new MKSceneNode();
			childTreeNode->m_meshName = xNodeChild->Attribute("mesh");
			childTreeNode->m_materialName = xNodeChild->Attribute("material");
			childTreeNode->m_parentNode = parentTreeNode;
			childTreeNode->m_id = xNodeChild->UnsignedAttribute("id",0);
			string modelText = xNodeChild->Attribute("model");
			istringstream is(modelText);
			glm::mat4 model;
			is	>> model[0][0] >> model[1][0] >> model[2][0] >> model[3][0]
				>> model[0][1] >> model[1][1] >> model[2][1] >> model[3][1]
				>> model[0][2] >> model[1][2] >> model[2][2] >> model[3][2]
				>> model[0][3] >> model[1][3] >> model[2][3] >> model[3][3];
			childTreeNode->m_modelToParent = model;
			// 查找配置的mesh和material
			if (meshMap.find(childTreeNode->m_meshName) != meshMap.end())
			{
				childTreeNode->m_mesh = new MKMesh(*meshMap[childTreeNode->m_meshName]);


				if (materialMap.find(childTreeNode->m_materialName) != materialMap.end())
				{
					childTreeNode->m_mesh->UseMaterial(materialMap[childTreeNode->m_materialName]);
				}
				// 如果material未加载则加载至内存中
				else
				{
					MKMaterial *material = new MKMaterial(cubeShader);
					material->loadFromXML((xmlDir + childTreeNode->m_materialName).c_str());
					material->LoadResouces();
					childTreeNode->m_mesh->UseMaterial(material);
					materialMap[childTreeNode->m_materialName] = material;
				}
			}
			xmlNodeQueue.push_back(xNodeChild);
			modelNodeQueue.push_back(childTreeNode);
			parentTreeNode->m_childNodes.push_back(childTreeNode);

		}
	}
}

