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


void MKMaterial::LoadResouces()
{
	for (unordered_map<string, MKTexture>::iterator it = m_texturesInfo.begin(); it != m_texturesInfo.end(); it++)
	{
		// load texture
		it->second.textureId = loadTexture(it->second.path.c_str());
	}
}
void MKMesh::UseMaterial(MKMaterial *material)
{
	if (material == NULL) return;
	m_material = material;
	int i = 0;
	// 将Sampler与纹理单元绑定
	for (unordered_map<string, MKTexture>::iterator it = material->m_texturesInfo.begin(); it != material->m_texturesInfo.end();i++, it++)
	{
		// load texture
		//it->second.textureId = loadTexture(it->second.path.c_str());
		string samplerName = it->first + "Sampler";
		material->m_shader->setInt(samplerName, i);
	}
	// set uniform
	for (unordered_map<string, MKUniformXML>::iterator it = material->m_uniformInfo.begin(); it != material->m_uniformInfo.end(); it++)
	{
		if(it->second.m_type == MKUniformXML::UNIFORM_INT)
		{
			material->m_shader->setInt(it->first, it->second.m_value.ival);
		}
		else if (it->second.m_type == MKUniformXML::UNIFORM_FLOAT)
		{
			material->m_shader->setFloat(it->first, it->second.m_value.fval);
		}
	}
}

// render the mesh
void MKMesh::Draw(glm::mat4 model,  float frameTime)
{
	gTimer.Start();
	if (m_positions.size() == 0)
	{
		return;
	}
	int i = 0;
	// 将纹理与纹理单元绑定，绘制时才绑定
	for (unordered_map<string, MKTexture>::iterator it = m_material->m_texturesInfo.begin(); it != m_material->m_texturesInfo.end(); i++, it++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, it->second.textureId);
		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, i == 0? GL_REPEAT: GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, i == 0? GL_REPEAT : GL_CLAMP_TO_EDGE);*/
	}
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glActiveTexture(GL_TEXTURE1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glActiveTexture(GL_TEXTURE0);


	//UseMaterial(m_material);
	m_material->m_shader->use();
	// 光照属性设置
	m_material->m_shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	//m_shader->setVec3("light.position", lightPos);
	m_material->m_shader->setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
	m_material->m_shader->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
	m_material->m_shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	m_material->m_shader->setVec3("viewPos", camera.m_position);
	m_material->m_shader->setVec3("lightPos", camera.m_position);
	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	m_material->m_shader->setMat4("projection", projection);
	m_material->m_shader->setMat4("view", view);	
	m_material->m_shader->setMat4("model", model);
	m_material->m_shader->setFloat("frameTime", frameTime);
	
	//// draw mesh
	if (m_material->m_depthTest)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
	
	glBindVertexArray(VAO);

	//cout << "period1 costs:" << gTimer.Period() << " ms" << endl;
	glDrawElements(m_material->m_primitive, m_indices.size(), GL_UNSIGNED_INT, 0);
	//cout << "period2 costs:" << gTimer.Period() << " ms" << endl;

}
void MKMesh::DrawMultiple(vector<glm::vec3> &positions, vector<glm::vec3> &colors)
{
	if (m_positions.size() == 0)
	{
		return;
	}
	int i = 0;
	// 将纹理与纹理单元绑定，绘制时才绑定
	for (unordered_map<string, MKTexture>::iterator it = m_material->m_texturesInfo.begin(); it != m_material->m_texturesInfo.end(); i++, it++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, it->second.textureId);
	}
	glActiveTexture(GL_TEXTURE0);


	//UseMaterial(m_material);
	m_material->m_shader->use();
	// 光照属性设置
	m_material->m_shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	//m_shader->setVec3("light.position", lightPos);
	m_material->m_shader->setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
	m_material->m_shader->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
	m_material->m_shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	m_material->m_shader->setVec3("viewPos", camera.m_position);
	m_material->m_shader->setVec3("lightPos", camera.m_position);
	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	m_material->m_shader->setMat4("projection", projection);
	m_material->m_shader->setMat4("view", view);
	
	//// draw mesh
	if (m_material->m_depthTest)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	glBindVertexArray(VAO);
	for (int i = 0; i < positions.size(); i++)
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, positions[i]);
		m_material->m_shader->setMat4("model",camera.GetModelMatrix() *  model);
		m_material->m_shader->setVec4("color", glm::vec4(1.0,0,0,1));
		glDrawElements(m_material->m_primitive, m_indices.size(), GL_UNSIGNED_INT, 0);
	}
	
}
void MKMesh::SetupBuffer()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	int VEC3SIZE = 3 * sizeof(float), VEC2SIZE = 2 * sizeof(float);

	// VBO
	int totalBufferSize = (m_positions.size() + m_normal.size() +  m_tangent.size() + m_bitangent.size())*VEC3SIZE
		+ m_texCoords.size()*VEC2SIZE;
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, totalBufferSize, NULL, GL_STATIC_DRAW);

	// 填入数据
	
	int offsetOfPosition, sizeOfPosition;
	int offsetOfNormal, sizeOfNormal;
	int offsetOftexCoords, sizeOftexCoords;
	int offsetOfTangent, sizeOfTangent;
	int offsetOfBitangent, sizeOfBitangent;

	offsetOfPosition = 0;
	sizeOfPosition = m_positions.size() * VEC3SIZE;
	//sizeOfPosition = m_positions.size() * sizeof(glm::vec3);

	offsetOfNormal = sizeOfPosition;
	sizeOfNormal = m_normal.size() * VEC3SIZE;

	offsetOftexCoords = offsetOfNormal + sizeOfNormal;
	sizeOftexCoords = m_texCoords.size() * VEC2SIZE;

	offsetOfTangent = offsetOftexCoords + sizeOftexCoords;
	sizeOfTangent = m_tangent.size() * VEC3SIZE;

	offsetOfBitangent = offsetOfTangent + sizeOfTangent;
	sizeOfBitangent = m_bitangent.size() * VEC3SIZE;

	// 这里出过bug, 须知&m_positions 和 &m_positions[0]的地址是不同的
	glBufferSubData(GL_ARRAY_BUFFER, offsetOfPosition, sizeOfPosition, &m_positions[0]);
	if (m_normal.size() != 0)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offsetOfNormal, sizeOfNormal, &m_normal[0]);
	}
	if (m_texCoords.size() != 0)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offsetOftexCoords, sizeOftexCoords, &m_texCoords[0]);
	}
	if (m_tangent.size() != 0)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offsetOfTangent, sizeOfTangent, &m_tangent[0]);
	}
	if (m_bitangent.size() != 0)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offsetOfBitangent, sizeOfBitangent, &m_bitangent[0]);
	}
	
	//EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32), &m_indices[0], GL_STATIC_DRAW);

	// 设置顶点属性格式
	
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VEC3SIZE, (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VEC3SIZE, (void*)(offsetOfNormal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VEC2SIZE, (void*)(offsetOftexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, VEC3SIZE, (void*)(offsetOfTangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, VEC3SIZE, (void*)(offsetOfBitangent));

	glBindVertexArray(0);
}

void MKMesh::ComputeBoundingBox(glm::mat4 modelToWorld)
{
	m_AABB_BBox.m_DownLeftPoint = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_AABB_BBox.m_UpRightPoint = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	if (m_positions.size()!=0)
	{
		glm::vec3 p;
		for (int i = 0; i < m_positions.size(); i++)
		{
			p = glm::vec3( modelToWorld * glm::vec4(m_positions[i],1));
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