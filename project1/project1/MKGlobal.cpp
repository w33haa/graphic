#include "MKGlobal.h"
#include "MKMesh.h"
#include "MKTools.h"

MKShader * cubeShader = NULL;
MKShader * defaultShader;
MKShader * instancingShader;
MKShader * particleShader;
MKShader * fireShader;
unordered_map<string, MKMesh*> meshMap;
unordered_map<string, MKMaterial*> materialMap;
string xmlDir = "../../resources/config/";

// Default camera values
const float YAW = -80.0f;
const float PITCH = -45.0f;
const float ZOOM = 45.0f;
const float SENSITIVITY = 0.1f;

// screen
const uint32 SCR_WIDTH = 800;
const uint32 SCR_HEIGHT = 600;

uint32 drawCallPerLoop = 0;

float CAMERA_MOVE_SPEED = 0.01f;

InstancingTest *instest;
MKParticleSystem *psystem;
MKBlackHole *bh;
MKBillBord *bb;
MKFire *fire;
MKTimer gTimer;
float frameTime = 0.0f;