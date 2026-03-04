#include "terrain.h"
#include <cmath>
#include <vector>

GLuint sandTexture;

struct TerrainVertex {
    float x, y, z;
    float u, v;
};

std::vector<TerrainVertex> terrainData;
std::vector<int> rowStart;
std::vector<int> rowCount;

std::vector<std::vector<float>> heightGrid;

int mapSize = 100;
float mapScale = 2.0f;

void GenerateTerrain()
{
    float texScale = 0.2f;

    heightGrid.resize(mapSize * 2 + 1, std::vector<float>(mapSize * 2 + 1, 0.0f));

    for (int z = -mapSize; z <= mapSize; z++)
    {
        for (int x = -mapSize; x <= mapSize; x++)
        {
            float realX = x * mapScale;
            float realZ = z * mapScale;

            float y = sin(realX * 0.01f) * 12.0f;
            y += cos(realZ * 0.015f) * 10.0f;
            y += sin(realX * 0.05f) * cos(realZ * 0.05f) * 3.0f;

            heightGrid[x + mapSize][z + mapSize] = y;
        }
    }

    for (int z = -mapSize; z < mapSize; z++)
    {
        rowStart.push_back(terrainData.size());
        int count = 0;

        for (int x = -mapSize; x <= mapSize; x++)
        {
            float realX = x * mapScale;
            float realZ1 = z * mapScale;
            float realZ2 = (z + 1) * mapScale;

            float y1 = heightGrid[x + mapSize][z + mapSize];
            float y2 = heightGrid[x + mapSize][z + 1 + mapSize];

            terrainData.push_back({ realX, y1, realZ1, x * texScale, z * texScale });
            terrainData.push_back({ realX, y2, realZ2, x * texScale, (z + 1) * texScale });

            count += 2;
        }
        rowCount.push_back(count);
    }
}

float GetHeightAt(float x, float z)
{
    int gridX = (int)round(x / mapScale) + mapSize;
    int gridZ = (int)round(z / mapScale) + mapSize;

   
    if (gridX < 0) gridX = 0;
    if (gridX > mapSize * 2) gridX = mapSize * 2;
    if (gridZ < 0) gridZ = 0;
    if (gridZ > mapSize * 2) gridZ = mapSize * 2;

    return heightGrid[gridX][gridZ];
}


void DrawTerrain()
{
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, sandTexture);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(TerrainVertex), &terrainData[0].x);
    glTexCoordPointer(2, GL_FLOAT, sizeof(TerrainVertex), &terrainData[0].u);

    for (size_t i = 0; i < rowStart.size(); i++)
    {
        glDrawArrays(GL_TRIANGLE_STRIP, rowStart[i], rowCount[i]);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}