#ifndef WORLD_OBJECTS_H
#define WORLD_OBJECTS_H

#include <freeglut.h>

extern GLuint roadTexture;

void InitWorldElements();
void DrawAllRoads();
void DrawStaticObjects();
void DrawStreetLamps();
void UpdateCarMovement();

#endif