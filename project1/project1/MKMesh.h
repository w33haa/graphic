#ifndef MK_MESH
#define MK_MESH



#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MKShader.h"
#include "MKCamera.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "MKType.h"
#include "MKTools.h"
#include "MKCommon.h"


using namespace std;

struct MKVertex {
	MKVertex(glm::vec3 position) :m_position(position) {}
	// position
	glm::vec3 m_position;
	// normal
	glm::vec3 m_normal;
	// texCoords
	glm::vec2 m_texCoords;
	// tangent
	glm::vec3 m_tangent;
	// bitangent
	glm::vec3 m_bitangent;
};



struct MKTexture
{
	MKTexture() {}
	MKTexture( string tpath)
		:path(tpath)
	{
		textureId = loadTexture(tpath.c_str());
	}
	uint32 textureId;
	string path;
};

struct MKUniformXML
{
	enum MKUniformXMLType
	{
		UNIFORM_INT,
		UNIFORM_FLOAT,
	};
	union MKUniformValue
	{
		int ival;
		float fval;
	};
	MKUniformXMLType m_type;
	MKUniformValue m_value;
};
struct MKMaterial
{
	MKMaterial() {}
	//MKMaterial(vector<MKTexture> &textures) :m_textures(textures) {}
	MKMaterial(MKShader *shader):m_shader(shader)
	{
		
	}
	MKMaterial(const MKMaterial &m):m_shader(m.m_shader), m_texturesInfo(m.m_texturesInfo),
		m_primitive(m.m_primitive), m_depthTest(m.m_depthTest), m_blend(m.m_blend), m_culling(m.m_culling)
	{

	}
	void loadFromXML(const char *path);
	void LoadResouces();
	void UseShader(MKShader *shader)
	{
		m_shader = shader;
	}
	//使用的shader
	MKShader *m_shader;
	
	unordered_map<string, MKTexture> m_texturesInfo;
	unordered_map<string, MKUniformXML> m_uniformInfo;

	//rendering mode
	uint32 m_primitive;
	bool m_depthTest;
	bool m_blend;
	bool m_culling;
};


class MKMesh {
public:

	//vector<MKVertex> m_vertices;
	// position
	vector<glm::vec3> m_positions;
	// normal
	vector<glm::vec3> m_normal;
	// texCoords
	vector<glm::vec2> m_texCoords;
	// tangent
	vector<glm::vec3> m_tangent;
	// bitangent
	vector<glm::vec3> m_bitangent;

	vector<uint32> m_indices;

	// 材质
	MKMaterial *m_material;

	uint32 VAO;
	// model matrix
	glm::mat4 m_model;
	// 包围盒
	MKAABBBoundingBox m_AABB_BBox;
	MKSphereBoundingBox m_sphere_BBox;

	MKMesh(){}
	MKMesh(vector<glm::vec3> &position, vector<uint32> &indices) :m_positions(position), m_indices(indices)
	{
		SetupBuffer();
	}
	MKMesh(vector<glm::vec3> &position, vector<uint32> &indices, vector<glm::vec2> &textCoords) 
		:m_positions(position), m_indices(indices), m_texCoords(textCoords)
	{
		SetupBuffer();
	}

	MKMesh(vector<glm::vec3> &position, vector<glm::vec3> &normal, vector<glm::vec2> &textCoords,
		vector<glm::vec3> &tangent, vector<glm::vec3> &bitangent, vector<uint32> &indices) :
		m_positions(position), m_normal(normal), m_texCoords(textCoords), m_tangent(tangent), m_bitangent(bitangent), m_indices(indices)
	{
		SetupBuffer();
	}
		
	//MKMesh(vector<MKVertex> &vertices, vector<uint32> &indices);

	// 这里未深拷贝顶点数据
	MKMesh(MKMesh &m):
		m_positions(m.m_positions), m_normal(m.m_normal), m_texCoords(m.m_texCoords),
		m_tangent(m.m_tangent), m_bitangent(m.m_bitangent), m_indices(m.m_indices), m_material(m.m_material),
		VAO(m.VAO), VBO(m.VBO), EBO(m.EBO)
	{
	}

	void UseMaterial(MKMaterial *material);
	// render the mesh
	void Draw(glm::mat4 model = camera.GetModelMatrix(), float frameTime = 0.0f);

	// multiple draw
	void DrawMultiple(vector<glm::vec3> &positions, vector<glm::vec3> &colors);
	//compute bounding box
	void ComputeBoundingBox(glm::mat4 modelToWorld = glm::mat4(1.0));
	
private:
	/*  Render data  */
	unsigned int VBO, EBO;

	/*  Functions    */
	// initializes all the buffer objects/arrays
	void SetupBuffer();

};

#endif