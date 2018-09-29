#include "MKMesh.h"
#include "MKTools.h"
#include <tinyxml2/tinyxml2.h>
extern uint32 getPrimitiveEnum(string &primitiveName);

void MKMaterial::loadFromXML(const char *path)
{
	m_shader->use();
	tinyxml2::XMLDocument doc;

	doc.LoadFile(path);
	//read texture
	tinyxml2::XMLNode * elem = doc.FirstChildElement("Material")->FirstChildElement("LightMap");
	if (elem)
	{
		elem = elem->FirstChildElement("Texture");
	}
	for (int i = 0; elem; elem = elem->NextSibling(), i++)
	{
		string textureName = elem->ToElement()->Attribute("name");
		string texturePath = elem->ToElement()->GetText();
		MKTexture textureObj(texturePath);
		m_texturesInfo[textureName] = textureObj;
	}
	
	// read uniform
	elem = doc.FirstChildElement("Material")->FirstChildElement("Uniform");
	if (elem)
	{
		elem = elem->FirstChildElement("Variable");
	}
	for (int i = 0; elem; elem = elem->NextSibling(), i++)
	{
		MKUniformXML uniformInfo;
		string variableName = elem->ToElement()->Attribute("name");
		string variableType = elem->ToElement()->Attribute("type");

		if (variableType == "int")
		{
			uniformInfo.m_type = MKUniformXML::UNIFORM_INT;
			int variableValue;
			elem->ToElement()->QueryIntText(&variableValue);
			uniformInfo.m_value.ival = variableValue;
		}
		else if (variableType == "float")
		{
			uniformInfo.m_type = MKUniformXML::UNIFORM_FLOAT;
			float variableValue;
			elem->ToElement()->QueryFloatText(&variableValue);
			uniformInfo.m_value.fval = variableValue;
		}
	}

	// read rendering mode
	elem = doc.FirstChildElement("Material")->FirstChildElement("RenderOption");
	if (elem)
	{
		elem = elem->FirstChildElement("Primitive");
		if (elem)
		{
			string primitiveName = elem->ToElement()->GetText();
			m_primitive = getPrimitiveEnum(primitiveName);
		}
	}

	elem = doc.FirstChildElement("Material")->FirstChildElement("RenderOption");
	if (elem)
	{
		elem = elem->FirstChildElement("DepthTest");
		if (elem)
		{
			string depthTest = elem->ToElement()->GetText();
			if (_stricmp(depthTest.c_str() ,"on") == 0)
			{
				m_depthTest = true;
			}
			else
			{
				m_depthTest = false;
			}
		}
	}
	elem = doc.FirstChildElement("Material")->FirstChildElement("RenderOption");
	if (elem)
	{
		elem = elem->FirstChildElement("Blend");
		if (elem)
		{
			string blend = elem->ToElement()->GetText();
			if (_stricmp(blend.c_str(), "on") == 0)
			{
				m_blend = true;
			}
			else
			{
				m_blend = false;
			}
		}
	}
	elem = doc.FirstChildElement("Material")->FirstChildElement("RenderOption");
	if (elem)
	{
		elem = elem->FirstChildElement("Culling");
		if (elem)
		{
			string culling = elem->ToElement()->GetText();
			if (_stricmp(culling.c_str(), "on") == 0)
			{
				m_culling = true;
			}
			else
			{
				m_culling = false;
			}
		}
	}
}

MKMesh::MKMesh(vector<MKVertex> &vertices, vector<uint32> &indices):m_model(1.0)
{
	m_vertices = vertices;
	m_indices = indices;

	SetupBuffer();
}

void MKMesh::UseMaterial(MKMaterial &material)
{
	m_material = material;
	int i = 0;
	// load texture and set sampler
	for (unordered_map<string, MKTexture>::iterator it = material.m_texturesInfo.begin(); it != material.m_texturesInfo.end();i++, it++)
	{
		// load texture
		it->second.textureId = loadTexture(it->second.path.c_str());

		// bind sampler with texture unit
		string samplerName = it->first + "Sampler";
		material.m_shader->setInt(samplerName, i);
	}
	glActiveTexture(GL_TEXTURE0);
	// set uniform
	for (unordered_map<string, MKUniformXML>::iterator it = material.m_uniformInfo.begin(); it != material.m_uniformInfo.end(); it++)
	{
		if(it->second.m_type == MKUniformXML::UNIFORM_INT)
		{
			material.m_shader->setInt(it->first, it->second.m_value.ival);
		}
		else if (it->second.m_type == MKUniformXML::UNIFORM_FLOAT)
		{
			material.m_shader->setFloat(it->first, it->second.m_value.fval);
		}
	}
}

// render the mesh
void MKMesh::Draw(glm::mat4 &modelToParent)
{
	if (m_vertices.size() == 0)
	{
		return;
	}
	int i = 0;
	// set sampler
	for (unordered_map<string, MKTexture>::iterator it = m_material.m_texturesInfo.begin(); it != m_material.m_texturesInfo.end(); i++, it++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, it->second.textureId);
	}
	glActiveTexture(GL_TEXTURE0);


	//UseMaterial(m_material);
	m_material.m_shader->use();
	// ¹âÕÕÊôÐÔÉèÖÃ
	m_material.m_shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	//m_shader->setVec3("light.position", lightPos);
	m_material.m_shader->setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
	m_material.m_shader->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
	m_material.m_shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	m_material.m_shader->setVec3("viewPos", camera.m_position);
	m_material.m_shader->setVec3("lightPos", camera.m_position);
	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	m_material.m_shader->setMat4("projection", projection);
	m_material.m_shader->setMat4("view", view);

	// world transformation
	glm::mat4 model = camera.GetModelMatrix() * modelToParent;
	glm::mat4 stran = glm::translate(glm::mat4(1), glm::vec3(1.0, 1.0, -1.0));

	m_material.m_shader->setMat4("model", model);
	//// draw mesh
	if (m_material.m_depthTest)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
	
	glBindVertexArray(VAO);
	glDrawElements(m_material.m_primitive, m_indices.size(), GL_UNSIGNED_INT, 0);

}

void MKMesh::SetupBuffer()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(MKVertex), &m_vertices[0], GL_STATIC_DRAW);

	//EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MKVertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MKVertex), (void*)offsetof(MKVertex, m_normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MKVertex), (void*)offsetof(MKVertex, m_texCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(MKVertex), (void*)offsetof(MKVertex, m_tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(MKVertex), (void*)offsetof(MKVertex, m_bitangent));

	glBindVertexArray(0);
}

void MKMesh::ComputeBoundingBox()
{
	m_AABB_BBox.m_DownLeftPoint = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_AABB_BBox.m_UpRightPoint = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);
	if (m_vertices.size()!=0)
	{
		glm::vec3 p;
		for (int i = 0; i < m_vertices.size(); i++)
		{
			p = m_vertices[i].m_position;
			m_AABB_BBox.m_DownLeftPoint.x = p.x < m_AABB_BBox.m_DownLeftPoint.x ? p.x : m_AABB_BBox.m_DownLeftPoint.x;
			m_AABB_BBox.m_DownLeftPoint.y = p.y < m_AABB_BBox.m_DownLeftPoint.y ? p.y : m_AABB_BBox.m_DownLeftPoint.y;
			m_AABB_BBox.m_DownLeftPoint.z = p.z < m_AABB_BBox.m_DownLeftPoint.z ? p.z : m_AABB_BBox.m_DownLeftPoint.z;

			m_AABB_BBox.m_UpRightPoint.x = p.x > m_AABB_BBox.m_UpRightPoint.x ? p.x : m_AABB_BBox.m_UpRightPoint.x;
			m_AABB_BBox.m_UpRightPoint.y = p.y > m_AABB_BBox.m_UpRightPoint.y ? p.y : m_AABB_BBox.m_UpRightPoint.y;
			m_AABB_BBox.m_UpRightPoint.z = p.z > m_AABB_BBox.m_UpRightPoint.z ? p.z : m_AABB_BBox.m_UpRightPoint.z;
		}
		m_sphere_BBox.m_center = m_AABB_BBox.m_DownLeftPoint + m_AABB_BBox.m_UpRightPoint;
		m_sphere_BBox.m_center /= 2;
		m_sphere_BBox.m_radius = (m_AABB_BBox.m_DownLeftPoint - m_AABB_BBox.m_UpRightPoint).length() / 2;
	}
}