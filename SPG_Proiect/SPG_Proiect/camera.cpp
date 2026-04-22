#include <windows.h>
#include <freeglut.h>
#include <cmath>
#include "terrain.h"
#include "camera.h"
#include "world_objects.h"

float camPosX = 0.0f, camPosY = 15.0f, camPosZ = 50.0f;
float yaw = -90.0f;
float pitch = 0.0f;
float camSpeed = 2.0f;
bool rightMousePressed = false;
int lastMouseX = -1;
int lastMouseY = -1;
float mouseSensitivity = 0.2f;
bool followBoat = false;

void Keyboard(unsigned char key, int, int)
{
    float rad = sbAngle * 3.14159f / 180.0f;

    switch (key)
    {
    case 'w': case 'W':
    {
        float newX = sbPosX + sin(rad) * camSpeed;
        float newZ = sbPosZ + cos(rad) * camSpeed;
        if (!CheckSpongebobCollision(newX, newZ)) {
            sbPosX = newX;
            sbPosZ = newZ;
        }
        break;
    }
    case 's': case 'S':
    {
        float newX = sbPosX - sin(rad) * camSpeed;
        float newZ = sbPosZ - cos(rad) * camSpeed;
        if (!CheckSpongebobCollision(newX, newZ)) {
            sbPosX = newX;
            sbPosZ = newZ;
        }
        break;
    }
    case 'a': case 'A':
        sbAngle -= 3.0f;
        break;
    case 'd': case 'D':
        sbAngle += 3.0f;
        break;
    case 'q': case 'Q':
        camPosY += camSpeed;
        break;
    case 'e': case 'E':
        camPosY -= camSpeed;
        break;
    case 'f': case 'F':
        followBoat = !followBoat;
        if (!followBoat) {
            float r = (boatAngle - 180.0f) * 3.14159f / 180.0f;
            float leftX = -cos(r);
            float leftZ = sin(r);
            sbPosX = boatPosX + leftX * 5.0f;
            sbPosZ = boatPosZ + leftZ * 5.0f;
            sbPosY = GetHeightAt(sbPosX, sbPosZ);
            sbAngle = -boatAngle;
        }
        printf("followBoat: %s\n", followBoat ? "ON" : "OFF");
        break;
    case 27: exit(0);
    }
    glutPostRedisplay();
}

void SpecialKeys(int key, int, int)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        boatSpeed += 0.15f;
        break;
    case GLUT_KEY_DOWN:
        boatSpeed -= 0.15f;
        break;
    case GLUT_KEY_LEFT:
        boatAngle += 4.0f;
        break;
    case GLUT_KEY_RIGHT:
        boatAngle -= 4.0f;
        break;
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

        if (!followBoat) {
            sbAngle += deltaX * mouseSensitivity;
            pitch -= deltaY * mouseSensitivity;
            if (pitch > 45.0f) pitch = 45.0f;
            if (pitch < -45.0f) pitch = -45.0f;
        }

        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}