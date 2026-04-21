#pragma once
#include <windows.h>
#include <freeglut.h>

extern float lightPos[4];
extern float lightAmbient[4];
extern float lightDiffuse[4];
extern float lightSpecular[4];
extern float shadowMatrix[16];
extern bool shadowsEnabled;
extern bool lightingEnabled;

void InitLighting();
void SetupLighting();
void BuildShadowMatrix(float groundY, float* lightPos4, float* matrix);
void BeginShadowDraw();
void EndShadowDraw();
void UpdateLightAngle(float delta);
float GetLightAngle();
void InitLampLights();
void SetupLampLights();