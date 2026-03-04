
#include <windows.h>
#include <freeglut.h>
#include <cmath>
#include "terrain.h" 
#include "camera.h"

float camPosX = 0.0f, camPosY = 15.0f, camPosZ = 50.0f;

float yaw = -90.0f;
float pitch = 0.0f;

float camSpeed = 2.0f;

bool rightMousePressed = false;
int lastMouseX = -1;
int lastMouseY = -1;
float mouseSensitivity = 0.2f;

void Keyboard(unsigned char key, int, int)
{
    float dirX = cos(yaw * 3.14159f / 180.0f) * cos(pitch * 3.14159f / 180.0f);
    float dirY = sin(pitch * 3.14159f / 180.0f);
    float dirZ = sin(yaw * 3.14159f / 180.0f) * cos(pitch * 3.14159f / 180.0f);

    float rightX = dirZ;
    float rightZ = -dirX;

    switch (key)
    {
    case 'w': case 'W': camPosX += dirX * camSpeed; camPosZ += dirZ * camSpeed; break; 
    case 's': case 'S': camPosX -= dirX * camSpeed; camPosZ -= dirZ * camSpeed; break; 
    case 'a': case 'A': camPosX -= rightX * camSpeed; camPosZ -= rightZ * camSpeed; break; 
    case 'd': case 'D': camPosX += rightX * camSpeed; camPosZ += rightZ * camSpeed; break; 
    case 'q': case 'Q': camPosY += camSpeed; break; 
    case 'e': case 'E': camPosY -= camSpeed; break; 
    case 27: exit(0); 
    }

    float playerHeight = 5.0f;
    float terrainHeight = GetHeightAt(camPosX, camPosZ);
    if (camPosY < terrainHeight + playerHeight)
    {
        camPosY = terrainHeight + playerHeight;
    }

    glutPostRedisplay();
}


void MouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            rightMousePressed = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else
        {
            rightMousePressed = false;
        }
    }
}
void MouseMotion(int x, int y)
{
    if (rightMousePressed)
    {
        float deltaX = x - lastMouseX;
        float deltaY = y - lastMouseY;

  
        yaw += deltaX * mouseSensitivity;
        pitch -= deltaY * mouseSensitivity;


        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}