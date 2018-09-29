#include "MKPartices.h"
#include <iostream>
#include <chrono>
#include <random>
#include "MKMesh.h"

#define PI 3.14159265
const float deltaTime = 0.0167;
void MKParticleSystem::Init()
{
	// create opengl buffer
	// vao
	glGenVertexArrays(1, &particleVAO);
	glBindVertexArray(particleVAO);

	// vbo
	glGenBuffers(1, &particleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);

	// 分配存储空间
	glBufferData(GL_ARRAY_BUFFER,
		m_vertices.size() * sizeof(vec3) + (sizeof(vec3) + sizeof(vec4))* limitNum, NULL, GL_STATIC_DRAW);

	// 将Mesh顶点数据存入
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		m_vertices.size() * sizeof(vec3), &m_vertices[0]);

	// ebo
	glGenBuffers(1, &particleEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particleEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32), &m_indices[0], GL_STATIC_DRAW);
}
void MKParticleSystem::CreateNewParticles()
{
	// 随机
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	

	std::uniform_real_distribution<float> distribution(1.0, 2.0);
	//auto dice = std::bind(distribution, generator);
	for (int i = 0; i < limitNum - partices.size(); i++)
	{
		MKParticle *p = new MKParticle();
		p->m_shape = meshMap["sphere"];

		uint64 now = GetTickCount();
		p->m_position = m_position + vec3(distribution(generator), distribution(generator), distribution(generator)-1.0);


		p->m_vdir = normalize(vec3(30, now % 30, 0));

		p->m_vdir *= 0.05;

		p->m_aDir = vec3(0, -0.0001, 0);
		p->m_color = vec4(1.0, 1.0, 1.0,1.0);
		partices.push_back(p);
	}
	
	
}

void MKParticleSystem::Update(glm::mat4 model)
{

	for (ParticesList::iterator it = partices.begin(); it != partices.end();)
	{
		if (ShouldParticalDie(*it))
		{
			it = partices.erase(it);
		}
		else
		{
			(*it)->m_position += (*it)->m_vdir;
			(*it)->m_vdir += (*it)->m_aDir;
			(*it)->m_color.g = std::max<float>((*it)->m_color.g-0.01f, 0.0f);
			(*it)->m_color.b = std::max<float>((*it)->m_color.b - 0.01f, 0.0f);
			//(*it)->m_position += vec3(0.02, 0, 0);
			it++;
		}
	}
	uint64 now = GetTickCount();
	if (now - m_lastCreateTime>= 50)
	{
		CreateNewParticles();
		m_lastCreateTime = now;
	}
	if (partices.size() != 0)
	{
		Render(model);
		//Render2();
	}
	
	
}

bool MKParticleSystem::ShouldParticalDie(MKParticle *p)
{
	if (p->m_position.y <= -5)
	{
		return true;
	}
	return false;
}

void MKParticleSystem::Render(glm::mat4 model)
{
	uint64 beforeRender = GetTickCount();
	// bind vao
	glBindVertexArray(particleVAO);

	// bind vbo
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);

	// copy data

	// 从list中copy出来
	m_positions.clear();
	m_colors.clear();
		
	for (ParticesList::iterator it = partices.begin(); it != partices.end(); it++)
	{
		m_positions.push_back((*it)->m_position);
		m_colors.push_back((*it)->m_color);
	}

	// copy positions
	uint32 offset = sizeof(vec3) * m_vertices.size();
	glBufferSubData(GL_ARRAY_BUFFER, offset,
		sizeof(vec3) * m_positions.size(), &m_positions[0]);

	// copy colors
	offset += sizeof(vec3) * m_positions.size();
	glBufferSubData(GL_ARRAY_BUFFER, offset,
		sizeof(vec4) * m_colors.size(), &m_colors[0]);
	
	// 设置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
		(GLvoid *)(m_vertices.size() * sizeof(vec3)));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0,
		(GLvoid *)(sizeof(vec3) * m_vertices.size() + sizeof(vec3) * m_positions.size()));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// 设置多少个instance共享一个属性
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	particleShader->use();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	particleShader->setMat4("projection", projection);
	particleShader->setMat4("view", camera.GetViewMatrix());
	particleShader->setMat4("model", model);

	glEnable(GL_DEPTH_TEST);
	glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, m_positions.size());
	uint64 aferRender = GetTickCount();
	cout << "render time: " << aferRender - beforeRender <<" ms"<<endl;
}

void MKParticleSystem::Render2()
{

	m_positions.clear();
	m_colors.clear();

	for (ParticesList::iterator it = partices.begin(); it != partices.end(); it++)
	{
		m_positions.push_back((*it)->m_position);
		m_colors.push_back((*it)->m_color);
	}
	for (ParticesList::iterator it = partices.begin(); it != partices.end();it++)
	{
		glm::mat4 tran = glm::translate(mat4(1.0), (*it)->m_position);
		tran[3][3] = 1.0;
		(*it)->m_color.g -= 0.01;
		(*it)->m_color.b -= 0.01;
		((*it)->m_shape)->m_material->m_shader->setVec4("color", (*it)->m_color);
		((*it)->m_shape)->Draw(camera.GetModelMatrix() * tran);
	}
	/*ParticesList::iterator it = partices.begin();
	if (it != partices.end())
	{
		((*it)->m_shape)->DrawMultiple(m_positions, m_colors);
	}*/
	
	uint64 aferRender = GetTickCount();

}

void MKBlackHole::CreateNewParticles()
{
	// 随机
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator1(seed);
	//std::default_random_engine generator1;
	std::default_random_engine generator2(seed);
	std::default_random_engine generator3(seed);

	std::uniform_real_distribution<float> distribution(1.0, 5.0);
	std::uniform_real_distribution<float> da(0.0, 2 * PI);
	//auto dice = std::bind(distribution, generator);
	
	for (int i = 0; i < limitNum - partices.size(); i++)
	{
		MKParticle *p = new MKParticle();
		p->m_shape = meshMap["sphere"];

		uint64 now = GetTickCount();
		float r = distribution(generator1);
		float a = da(generator1);
		//float a = 90.0;
		float b = da(generator1);
		p->m_position = vec3(r*sin(a)*sin(b), r*cos(a), r*sin(a)*cos(b));
		//p->m_position = vec3(r*cos(a), r*sin(a),0 );

		p->m_vdir = normalize(glm::cross(vec3(0, 0, 1.0), p->m_position));
		//p->m_vdir =-normalize(p->m_position);

		p->m_vdir *= 0.01;

		p->m_aDir = -normalize(p->m_position);
		p->m_aDir *= m_aRatio;
		p->m_color = vec4(glm::mix(vec4(0.97, 0.93, 0.7, 0.4), vec4(0.92, 0.51, 0.27, 0.2), (r-1)/ 4.0));

		partices.push_back(p);
	}
}

void MKBlackHole::Init()
{
	// create opengl buffer
	// vao
	glGenVertexArrays(1, &particleVAO);
	glBindVertexArray(particleVAO);

	// vbo
	glGenBuffers(1, &particleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);

	// 分配存储空间
	glBufferData(GL_ARRAY_BUFFER,
		m_vertices.size() * sizeof(vec3) + (sizeof(vec3)+sizeof(vec4)) * limitNum, NULL, GL_STATIC_DRAW);

	// 将Mesh顶点数据存入
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		m_vertices.size() * sizeof(vec3), &m_vertices[0]);

	// ebo
	glGenBuffers(1, &particleEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particleEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32), &m_indices[0], GL_STATIC_DRAW);
}

void MKBlackHole::Update(glm::mat4 model)
{

	for (ParticesList::iterator it = partices.begin(); it != partices.end();)
	{
		if (ShouldParticalDie(*it))
		{
			it = partices.erase(it);
		}
		else
		{
			MKParticle *p = *it;
			(*it)->m_position += (*it)->m_vdir;
			/*p->m_vdir = normalize(glm::cross(vec3(0, 1.0, 0), p->m_position));;

			p->m_vdir *= 0.01;*/

			p->m_aDir = -normalize(p->m_position);
			p->m_aDir *= m_aRatio;
			p->m_vdir += p->m_aDir;
			float r = glm::length(p->m_position);
			p->m_color = vec4(glm::mix(vec4(0.97, 0.93, 0.7, 0.4), vec4(0.92, 0.51, 0.27, 0.2), (r - 1) / 4.0));

			it++;
		}
	}
	uint64 now = GetTickCount();
	if (now - m_lastCreateTime >= 50)
	{
		CreateNewParticles();
		m_lastCreateTime = now;
	}
	if (partices.size() != 0)
	{
		Render(model);
	}


}

void MKBlackHole::Render(glm::mat4 model )
{
	//uint64 beforeRender = GetTickCount();
	// bind vao
	glBindVertexArray(particleVAO);

	// bind vbo
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);

	// copy data

	// 从list中copy出来
	m_positions.clear();
	m_colors.clear();

	for (ParticesList::iterator it = partices.begin(); it != partices.end(); it++)
	{
		m_positions.push_back((*it)->m_position);
		m_colors.push_back((*it)->m_color);
	}

	// copy positions
	uint32 offset = sizeof(vec3) * m_vertices.size();
	glBufferSubData(GL_ARRAY_BUFFER, offset,
		sizeof(vec3) * m_positions.size(), &m_positions[0]);

	// copy colors
	offset += sizeof(vec3) * m_positions.size();
	glBufferSubData(GL_ARRAY_BUFFER, offset,
		sizeof(vec4) * m_colors.size(), &m_colors[0]);

	// 设置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
		(GLvoid *)(m_vertices.size() * sizeof(vec3)));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0,
		(GLvoid *)(sizeof(vec3) * m_vertices.size() + sizeof(vec3) * m_positions.size()));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// 设置多少个instance共享一个属性
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	particleShader->use();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	particleShader->setMat4("projection", projection);
	particleShader->setMat4("view", camera.GetViewMatrix());
	particleShader->setMat4("model", model);

	glEnable(GL_DEPTH_TEST);
	glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, m_positions.size());
	//uint64 aferRender = GetTickCount();
	//cout << "render time: " << aferRender - beforeRender << " ms" << endl;
}

bool MKBlackHole::ShouldParticalDie(MKParticle *p)
{
	if (glm::length(p->m_position) < 1.0)
	{
		return true;
	}
	return false;
}

void MKParticleBase::InitBuffer()
{
	// create opengl buffer
	// vao
	glGenVertexArrays(1, &particleVAO);
	glBindVertexArray(particleVAO);

	// vbo
	glGenBuffers(1, &particleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);

	// 分配存储空间
	glBufferData(GL_ARRAY_BUFFER,
		m_vertices.size() * sizeof(vec3) + (sizeof(vec3) + sizeof(vec4))* limitNum, NULL, GL_STATIC_DRAW);

	// 将Mesh顶点数据存入
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		m_vertices.size() * sizeof(vec3), &m_vertices[0]);

	// ebo
	glGenBuffers(1, &particleEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particleEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32), &m_indices[0], GL_STATIC_DRAW);
}

void MKParticleBase::Render()
{
	//uint64 beforeRender = GetTickCount();
	// bind vao
	glBindVertexArray(particleVAO);

	// bind vbo
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);

	// copy data

	// 从list中copy出来
	m_positions.clear();
	m_colors.clear();

	for (ParticesList::iterator it = partices.begin(); it != partices.end(); it++)
	{
		m_positions.push_back((*it)->m_position);
		m_colors.push_back((*it)->m_color);
	}

	// copy positions
	uint32 offset = sizeof(vec3) * m_vertices.size();
	glBufferSubData(GL_ARRAY_BUFFER, offset,
		sizeof(vec3) * m_positions.size(), &m_positions[0]);

	// copy colors
	offset += sizeof(vec3) * m_positions.size();
	glBufferSubData(GL_ARRAY_BUFFER, offset,
		sizeof(vec4) * m_colors.size(), &m_colors[0]);

	// 设置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
		(GLvoid *)(m_vertices.size() * sizeof(vec3)));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0,
		(GLvoid *)(sizeof(vec3) * m_vertices.size() + sizeof(vec3) * m_positions.size()));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// 设置多少个instance共享一个属性
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	particleShader->use();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	particleShader->setMat4("projection", projection);
	particleShader->setMat4("view", camera.GetViewMatrix());
	particleShader->setMat4("model", camera.GetModelMatrix());

	glEnable(GL_DEPTH_TEST);
	glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, m_positions.size());
	//uint64 aferRender = GetTickCount();
	//cout << "render time: " << aferRender - beforeRender << " ms" << endl;
}

void MKFire::CreateNewParticles()
{
	// 随机
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator1(seed);

	std::uniform_real_distribution<float> distribution(1.0, 5.0);
	std::uniform_real_distribution<float> da(0.0, 2 * PI);
	//auto dice = std::bind(distribution, generator);

	for (int i = 0; i <= emitNum && i < limitNum - partices.size(); i++)
	{
		MKParticle *p = new MKParticle();
		p->m_shape = meshMap["sphere"];

		uint64 now = GetTickCount();
		float r = distribution(generator1);
		float a = da(generator1);
		//float a = 90.0;
		float b = da(generator1);
		//p->m_position = vec3(r*sin(a)*sin(b), r*cos(a), r*sin(a)*cos(b));
		//p->m_position = vec3(0.2*cos(b),0, 0.2*sin(b) );
		p->m_position = vec3(0, 0, 0);
		p->m_vdir = normalize(vec3(r*sin(a)*sin(b), glm::abs(r*cos(a))+0.2, r*sin(a)*cos(b)));
		//p->m_vdir =-normalize(p->m_position);

		p->m_vdir *= 0.01;

		//p->m_aDir = normalize(p->m_position - vec3(0,1.0,0));
		vec3 OutSidePressure = -p->m_position;
		OutSidePressure.y = 0.0;
		p->m_aDir = normalize(vec3(0, 1.0, 0)+ OutSidePressure);
		p->m_aDir *= m_aRatio;//m_aRatio;
		p->m_color = vec4(1, 1, 1, 1.0);

		partices.push_back(p);
	}
}
void MKFire::Update()
{
	for (ParticesList::iterator it = partices.begin(); it != partices.end();)
	{
		if (ShouldParticalDie(*it))
		{
			it = partices.erase(it);
		}
		else
		{
			MKParticle *p = *it;
			(*it)->m_position += (*it)->m_vdir;
			/*p->m_vdir = normalize(glm::cross(vec3(0, 1.0, 0), p->m_position));;

			p->m_vdir *= 0.01;*/
			vec3 OutSidePressure = -p->m_position;
			OutSidePressure.y = 0.0;
			p->m_aDir = normalize(vec3(0, 1.0, 0) + OutSidePressure);
			p->m_aDir *= m_aRatio;
			p->m_vdir += p->m_aDir;
			float r = glm::length(p->m_position);
			if (r <= 2.0)
			{
				p->m_color = vec4(glm::mix(vec4(1, 1, 1, 1), vec4(0.97, 0.99, 0.51, 0.8), glm::sqrt(r/2.0) ));
			}
			else
			{
				p->m_color = vec4(glm::mix(vec4(0.97, 0.99, 0.51, 0.8), vec4(1.0, 0.2, 0.07, 0.5), glm::sqrt((r-2.0) / 6.0)));
			}
			

			it++;
		}
	}
	uint64 now = GetTickCount();
	if (now - m_lastCreateTime >= 100.0)
	{
		CreateNewParticles();
		m_lastCreateTime = now;
	}
	if (partices.size() != 0)
	{
		Render();
	}
}

bool MKFire::ShouldParticalDie(MKParticle *p)
{
	if (glm::length(p->m_position) > 8.0)
	{
		return true;
	}
	return false;
}