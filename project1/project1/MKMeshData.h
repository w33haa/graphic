#ifndef MKMESHDATA
#define MKMESHDATA

#include <string>
#include <unordered_map>
#include "MKShader.h"
#include "MKMesh.h"
using namespace std;

// 顶点和法向量数据输入
float cubeVertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,

};

//tangent/bittangent
float cubeTB[] =
{
	1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,

	1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

	0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

};

float tetVertices[] =
{
	//points       //normal       //texture coord
	0.5, 0.0, 0.5, 0.0, -1.0, 0.0, 0.5, 0.5,
	-0.5, 0.0, -0.5, 0.0, -1.0, 0.0, 0.0, 0.0,
	-0.5, 0.0, 0.5, 0.0, -1.0, 0.0, 0.0, 0.5,

	0.5, 0.0, 0.5, 0.0, -1.0, 0.0, 0.5, 0.5,
	0.5, 0.0, -0.5, 0.0, -1.0, 0.0, 0.5, 0.0,
	-0.5, 0.0, -0.5, 0.0, -1.0, 0.0, 0.0, 0.0,

	0.5, 0.0, 0.5, 0.0, 0.707, 0.707, 1.0, 0.0,
	0.0, 0.5, 0.0, 0.0, 0.707, 0.707, 0.5, 0.707,
	-0.5, 0.0, 0.5, 0.0, 0.707, 0.707, 0.0, 0.0,

	0.5, 0.0, -0.5, 0.0, 0.707, -0.707, 0.0, 0.0,
	-0.5, 0.0, -0.5, 0.0, 0.707, -0.707, 1.0, 0.0,
	0.0, 0.5, 0.0, 0.0, 0.707, -0.707, 0.5, 0.707,

	-0.5, 0.0, 0.5, -0.707, 0.707, 0.0, 1.0, 0.0,
	0.0, 0.5, 0.0, -0.707, 0.707, 0.0, 0.5, 0.707,
	-0.5, 0.0, -0.5, -0.707, 0.707, 0.0, 0.0, 0.0,

	0.5, 0.0, 0.5, 0.707, 0.707, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.707, 0.707, 0.0, 0.5, 0.707,
	0.5, 0.0, -0.5, 0.707, 0.707, 0.0, 1.0, 0.0,


};
float tetTB[] =
{
	1.0, 0.0, 0.0, 0.0, 0.0, -1.0,
	1.0, 0.0, 0.0, 0.0, 0.0, -1.0,
	1.0, 0.0, 0.0, 0.0, 0.0, -1.0,

	1.0, 0.0, 0.0, 0.0, 0.0, -1.0,
	1.0, 0.0, 0.0, 0.0, 0.0, -1.0,
	1.0, 0.0, 0.0, 0.0, 0.0, -1.0,

	1.0, 0.0, 0.0, 0.0, 0.707, 0.707,
	1.0, 0.0, 0.0, 0.0, 0.707, 0.707,
	1.0, 0.0, 0.0, 0.0, 0.707, 0.707,

	1.0, 0.0, 0.0, 0.0, -0.707, 0.707,
	1.0, 0.0, 0.0, 0.0, -0.707, 0.707,
	1.0, 0.0, 0.0, 0.0, -0.707, 0.707,

	0.707, 0.707, 0.0, 0.0, 0.0, 1.0,
	0.707, 0.707, 0.0, 0.0, 0.0, 1.0,
	0.707, 0.707, 0.0, 0.0, 0.0, 1.0,

	0.707, -0.707, 0.0, 0.0, 0.0, 1.0,
	0.707, -0.707, 0.0, 0.0, 0.0, 1.0,
	0.707, -0.707, 0.0, 0.0, 0.0, 1.0,
};

// sphere


float octVertices[] =
{
	0.0f, -1.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	0.0f,  0.0f,  1.0f,
	-1.0f,  0.0f,  0.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  1.0f,  0.0f
};

float octIndices[] =
{
	0,    1,    2,
	0,    2,    3,
	0,    3,    4,
	0,    4,    1,
	1,    5,    2,
	2,    5,    3,
	3,    5,    4,
	4,    5,    1
};

template<> struct hash<glm::vec3>
{
	std::size_t operator()(const glm::vec3 & s) const noexcept
	{
		std::size_t hash = s.x * 10000;
		hash += s.y * 100;
		hash += s.z * 10;
		
		return hash;
	}
};

void splitTriangle(vector<glm::vec3> inputV, vector<unsigned int> inputI, vector<glm::vec3> &outputV, vector<unsigned int> &outputI, int level)
{
	unordered_map<glm::vec3, int> pindex;
	for (int l = 0; l < level; l++)
	{
		outputI.clear();
		for (int i = 0; i < inputI.size(); i += 3)
		{
			int idx0, idx1,idx2, idx01, idx12, idx20;
			idx0 = inputI[i];
			idx1 = inputI[i + 1];
			idx2 = inputI[i + 2];
			glm::vec3 v0(inputV[idx0]);
			glm::vec3 v1(inputV[idx1]);
			glm::vec3 v2(inputV[idx2]);
			glm::vec3 v01 = glm::normalize(v0 + v1);
			glm::vec3 v12 = glm::normalize(v1 + v2);
			glm::vec3 v20 = glm::normalize(v2 + v0);
			
			if (pindex.find(v01) != pindex.end())
			{
				idx01 = pindex[v01];
			}
			else
			{
				idx01 = outputV.size();
				outputV.push_back(v01);
				pindex[v01] = idx01;
			}

			if (pindex.find(v12) != pindex.end())
			{
				idx12 = pindex[v12];
			}
			else
			{
				idx12 = outputV.size();
				outputV.push_back(v12);
				pindex[v12] = idx12;
			}

			if (pindex.find(v20) != pindex.end())
			{
				idx20 = pindex[v20];
			}
			else
			{
				idx20 = outputV.size();
				outputV.push_back(v20);
				pindex[v20] = idx20;
			}

			outputI.push_back(idx0);
			outputI.push_back(idx01);
			outputI.push_back(idx20);

			outputI.push_back(idx01);
			outputI.push_back(idx1);
			outputI.push_back(idx12);

			outputI.push_back(idx20);
			outputI.push_back(idx01);
			outputI.push_back(idx12);

			outputI.push_back(idx20);
			outputI.push_back(idx12);
			outputI.push_back(idx2);
			
		}
		pindex.clear();
		inputV= outputV;
		inputI= outputI;
		
	}
}


#endif