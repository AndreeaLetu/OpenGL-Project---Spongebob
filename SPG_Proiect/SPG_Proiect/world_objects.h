#pragma once
#include <windows.h>
#include <freeglut.h>
#include "model_loader.h"

extern OBJModel pineappleHouse, squidwardHouse, patrickHouse, krustyKrab, signSpongebob;
extern OBJModel krustyKrabCar;

extern float carOrbitAngle;
extern float carOrbitSpeed;
extern float carOrbitRadius;

void InitWorldElements();
void DrawAllRoads();
void DrawStaticObjects();
void DrawStreetLamps();
void UpdateCarMovement();
bool IsPointOnRoad(float x, float z);
void DrawLightCone(float x, float baseY, float z, float h);
void DrawCartoonBench(float x, float z, float rotY);