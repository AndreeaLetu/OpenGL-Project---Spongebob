#pragma once
#include <windows.h>
#include <freeglut.h>
#include <vector>

struct SimpleVertex {
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};

struct SubMesh {
    std::vector<SimpleVertex> vertices;
    GLuint textureID = 0;
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
};

struct OBJModel {
    std::vector<SubMesh> subMeshes;
};

bool LoadOBJModel(const char* filename, OBJModel& outModel);
void DrawOBJModel(const OBJModel& model);