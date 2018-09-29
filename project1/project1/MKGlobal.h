#ifndef MKGlobal_H
#define MKGlobal_H
#include <string>
#include <unordered_map>
#include "MKShader.h"

//#include "MKMesh.h"

using namespace std;

class MKMesh;
class MKMaterial;
class InstancingTest;
class MKParticleSystem;
class MKBlackHole;
class MKTimer;
class MKBillBord;
class MKFire;

extern MKShader * cubeShader;
extern MKShader * defaultShader;
extern MKShader * instancingShader;
extern MKShader * particleShader;
extern MKShader * fireShader;
extern unordered_map<string, MKMesh*> meshMap;
extern unordered_map<string, MKMaterial*> materialMap;
extern string xmlDir;

// Default camera values
extern const float YAW;
extern const float PITCH;
extern const float ZOOM;
extern const float SENSITIVITY;

// screen
extern const uint32 SCR_WIDTH;
extern const uint32 SCR_HEIGHT;

// 统计数据
extern uint32 drawCallPerLoop;

extern float CAMERA_MOVE_SPEED;

// instancing test
extern InstancingTest *instest;
extern MKParticleSystem *psystem;
extern MKBlackHole *bh;
extern MKBillBord *bb;
extern MKFire *fire;
// timer
extern MKTimer gTimer;

extern float frameTime;
#endif
