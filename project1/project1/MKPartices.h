#ifndef MK_PARTICES_H
#define MK_PARTICES_H

#include <vector>
#include <list>
#include <time.h>
#include <windows.h>
#include "MKType.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h> 

#include "MKShader.h"
#include "MKCamera.h"
using namespace std;
using namespace glm;

extern MKCamera camera;

class MKMesh;

class InstancingTest
{
	uint32 sphereVAO;
	uint32 sphereVBO;
	uint32 sphereEBO;

	vector<glm::vec3> vertices;
	vector<uint32> indices;

	vector<glm::vec3> positions;
	vector<glm::vec3> colors;
public:
	InstancingTest(vector<glm::vec3> &v, vector<uint32> &i):vertices(v), indices(i)
	{
		SetBuffer();
	}

	void SetBuffer()
	{
		// set vao
		glGenVertexArrays(1, &sphereVAO);
		glBindVertexArray(sphereVAO);

		// set vbo
		glGenBuffers(1, &sphereVBO);
		glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// set ebo
		glGenBuffers(1, &sphereEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32), &indices[0], GL_STATIC_DRAW);

		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
	}

	void Draw()
	{
		instancingShader->use();
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		instancingShader->setMat4("projection", projection);
		instancingShader->setMat4("view", camera.GetViewMatrix());
		instancingShader->setMat4("model", camera.GetModelMatrix());
		
		glEnable(GL_DEPTH_TEST);

		glBindVertexArray(sphereVAO);
		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, 2000);

	}
	
	void Draw2()
	{
		instancingShader->use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		instancingShader->setMat4("projection", projection);
		instancingShader->setMat4("view", camera.GetViewMatrix());
		instancingShader->setMat4("model", camera.GetModelMatrix());
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(sphereVAO);

		for (int i = 0; i < 2000; i++)
		{
			instancingShader->setInt("drawId", i);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}
	}
	
};


class MKParticle
{
public:
	MKParticle() {}
	MKMesh *m_shape;
	// 速度方向
	vec3 m_vdir;
	// 位置
	vec3 m_position;
	float m_a;
	vec3 m_aDir;
	float m_mass;
	vec3 m_fdir;
	float m_f;
	// 颜色
	vec4 m_color;
};
class MKParticleSystem
{
public:
	MKParticleSystem()
	{
		limitNum = 1000;
		m_position = vec3(0, 0, 0);
		m_emitDir = vec3(1.0, 0.0, 0);
		m_lastCreateTime = GetTickCount();
	}
	MKParticleSystem(vector<vec3> vertices, vector<uint32> indices):m_vertices(vertices),m_indices(indices)
	{
		limitNum = 1000;
		m_position = vec3(0, 0, 0);
		m_emitDir = vec3(1.0, 0.0, 0);
		m_lastCreateTime = GetTickCount();
		Init();
	}
	typedef list<MKParticle *> ParticesList;
	ParticesList partices;
	uint32  limitNum;


	vec3 m_position;  //粒子发射位置
	vec3 m_emitDir;
	uint64 m_lastCreateTime;
	
	// mesh顶点 instacing位置， 颜色
	vector<vec3> m_vertices;
	vector<uint32> m_indices;
	vector<vec3> m_positions;
	vector<vec4> m_colors;

	// opengl buffer
	uint32 particleVAO;
	uint32 particleVBO;
	uint32 particleEBO;

	void Init();
	void CreateNewParticles();
	void Update(glm::mat4 model = camera.GetModelMatrix()); //每帧更新粒子位置，形状，颜色等
	void Render(glm::mat4 model = camera.GetModelMatrix());
	void Render2();//绘制粒子
	bool ShouldParticalDie(MKParticle *p); //检查粒子是否到了生命周期
};
class MKBlackHole
{
public:
	MKBlackHole()
	{
		limitNum = 1;
		m_center = vec3(0.0, 0.0, 0.0);
		m_lastCreateTime = GetTickCount();
		m_aRatio = 0.0001;
		Init();
	}
	MKBlackHole(vector<vec3> &vertices, vector<uint32> &indices) :m_vertices(vertices), m_indices(indices)
	{
		limitNum = 2000;
		m_lastCreateTime = GetTickCount();
		m_aRatio = 0.0003;
		Init();
	}
	typedef list<MKParticle *> ParticesList;
	ParticesList partices;
	uint32  limitNum;
	float m_aRatio;

	// mesh顶点 instacing位置， 颜色
	vector<vec3> m_vertices;
	vector<uint32> m_indices;
	vector<vec3> m_positions;
	vector<vec4> m_colors;

	// opengl buffer
	uint32 particleVAO;
	uint32 particleVBO;
	uint32 particleEBO;

	uint64 m_lastCreateTime;
	vec3 m_center;
	void Init();
	void CreateNewParticles();
	void Update(glm::mat4 model = camera.GetModelMatrix()); //每帧更新粒子位置，形状，颜色等
	void Render(glm::mat4 model = camera.GetModelMatrix()); //绘制粒子
	bool ShouldParticalDie(MKParticle *p); //检查粒子是否到了生命周期
};
class MKParticleBase
{
public:
	typedef list<MKParticle *> ParticesList;
	ParticesList partices;
	uint32  limitNum;
	uint32  emitNum;
	float m_aRatio;

	// mesh顶点 instacing位置， 颜色
	vector<vec3> m_vertices;
	vector<uint32> m_indices;
	vector<vec3> m_positions;
	vector<vec4> m_colors;

	// opengl buffer
	uint32 particleVAO;
	uint32 particleVBO;
	uint32 particleEBO;
	uint64 m_lastCreateTime;
	vec3 m_center;
	
	MKParticleBase(vector<vec3> &vertices, vector<uint32> &indices) : m_vertices(vertices), m_indices(indices)
	{
		
	}
	void InitBuffer();
	virtual void CreateNewParticles() = 0;
	virtual void Update() = 0; //每帧更新粒子位置，形状，颜色等
	void Render(); //绘制粒子
	virtual bool ShouldParticalDie(MKParticle *p) = 0; //检查粒子是否到了生命周期
};

class MKFire : public MKParticleBase
{
public:
	MKFire(vector<vec3> &vertices, vector<uint32> &indices): MKParticleBase( vertices,indices)
	{
		limitNum = 4000;
		emitNum = 50;
		m_lastCreateTime = GetTickCount();
		m_aRatio = 0.0001;
		InitBuffer();
	}

	void CreateNewParticles();
	void Update();
	bool ShouldParticalDie(MKParticle *p);
};
#endif
