#pragma once
#include <windows.h>
#include <freeglut.h>
#include "model_loader.h"


struct BenchInfo { float x, z, rotY; };
struct LampPos { float x, z; };



extern OBJModel pineappleHouse, squidwardHouse, patrickHouse, krustyKrab, signSpongebob;
extern OBJModel krustyKrabCar;
extern OBJModel policeBoat;

extern OBJModel spongebobModel;
extern float sbPosX, sbPosY, sbPosZ;
extern float sbAngle;


extern float carOrbitAngle, carOrbitSpeed, carOrbitRadius;
extern float boatPosX, boatPosY, boatPosZ, boatAngle, boatSpeed;
extern float boatPosX, boatPosY, boatPosZ, boatAngle, boatSpeed;
bool CheckSpongebobCollision(float newX, float newZ);

extern LampPos g_rightLamps[6];
extern BenchInfo g_rightBenches[12];


void InitWorldElements();
void DrawAllRoads();
void DrawStaticObjects();
void DrawStreetLamps();
void UpdateCarMovement();
void UpdateBoat();
void DrawControlledBoat();
bool IsPointOnRoad(float x, float z);
void DrawSpongebob();
void UpdateSpongebob();


void DrawNeonKelp(float x, float z, float h, float off);
void DrawPinkBubblePlant(float x, float z, float h, float off);
void DrawCyanVine(float x, float z, float h, float off);
void DrawAllPlantsShadowGeom();

void InitBubbles();
void UpdateBubbles();
void DrawBubbles();