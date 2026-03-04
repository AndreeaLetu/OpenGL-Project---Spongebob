#pragma once
#pragma once
#include <windows.h>
#include <freeglut.h>

extern GLuint sandTexture;

void GenerateTerrain();
void DrawTerrain();
float GetHeightAt(float x, float z);
