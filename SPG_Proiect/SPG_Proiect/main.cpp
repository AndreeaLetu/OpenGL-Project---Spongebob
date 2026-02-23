#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include <windows.h>
#include <freeglut.h>
#include <cmath>
#include <cstdio>
#include <direct.h>
#include "stb_image.h"

//Textures

GLuint skyboxTex[6];
GLuint sandTexture;

// Camera
float camPosX = 0.0f, camPosY = 15.0f, camPosZ = 50.0f;
float yaw = -90.0f;   
float pitch = 0.0f;   
float camSpeed = 2.0f;

// Mouse control
bool rightMousePressed = false;
int lastMouseX = -1;
int lastMouseY = -1;
float mouseSensitivity = 0.2f; // Camera sensibility 

//Load Texture 
GLuint LoadTexture(const char* filename)
{
    FILE* f = fopen(filename, "rb");
    
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return texture;
}

//Skybox
void DrawSkybox(float size)
{
    float s = size / 2.0f;

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG); 
    glDepthMask(GL_FALSE);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Right (+X) px
    glBindTexture(GL_TEXTURE_2D, skyboxTex[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, s);
    glTexCoord2f(0, 1); glVertex3f(s, s, -s);
    glEnd();

    // Left (-X) nx
    glBindTexture(GL_TEXTURE_2D, skyboxTex[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, s);
    glTexCoord2f(1, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(-s, s, -s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, s);
    glEnd();

    // Top (+Y) py
    glBindTexture(GL_TEXTURE_2D, skyboxTex[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-s, s, s);
    glTexCoord2f(1, 0); glVertex3f(s, s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, -s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, -s);
    glEnd();

    // Bottom (-Y) ny
    glBindTexture(GL_TEXTURE_2D, skyboxTex[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(s, -s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, -s, s);
    glEnd();

    // Front (+Z) pz
    glBindTexture(GL_TEXTURE_2D, skyboxTex[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, s);
    glEnd();

    // Back  (-Z) nz
    glBindTexture(GL_TEXTURE_2D, skyboxTex[5]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(-s, s, -s);
    glTexCoord2f(0, 1); glVertex3f(s, s, -s);
    glEnd();

    glDepthMask(GL_TRUE);
    glEnable(GL_FOG); 
}

// Terrain drawing with procedural height and sand texture
void DrawTerrain()
{
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    int size = 100;
    float scale = 2.0f;

    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, sandTexture);
    float sandTexScale = 0.2f;

    for (int z = -size; z < size; z++)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int x = -size; x <= size; x++)
        {
            float realX = x * scale;
            float realZ1 = z * scale;
            float realZ2 = (z + 1) * scale;

            float y1 = sin(realX * 0.02f) * cos(realZ1 * 0.02f) * 15.0f
                + sin(realX * 0.1f) * cos(realZ1 * 0.1f) * 2.0f;

            float y2 = sin(realX * 0.02f) * cos(realZ2 * 0.02f) * 15.0f
                + sin(realX * 0.1f) * cos(realZ2 * 0.1f) * 2.0f;

            glTexCoord2f(x * sandTexScale, z * sandTexScale);
            glVertex3f(realX, y1, realZ1);

            glTexCoord2f(x * sandTexScale, (z + 1) * sandTexScale);
            glVertex3f(realX, y2, realZ2);
        }
        glEnd();
    }
}

// Keyboard input for camera movement
void Keyboard(unsigned char key, int x, int y)
{
    float dirX = cos(yaw * 3.14159f / 180.0f) * cos(pitch * 3.14159f / 180.0f);
    float dirZ = sin(yaw * 3.14159f / 180.0f) * cos(pitch * 3.14159f / 180.0f);
    float rightX = dirZ;
    float rightZ = -dirX;

    switch (key) {
    case 'w': case 'W': camPosX += dirX * camSpeed; camPosZ += dirZ * camSpeed; break;
    case 's': case 'S': camPosX -= dirX * camSpeed; camPosZ -= dirZ * camSpeed; break;
    case 'a': case 'A': camPosX -= rightX * camSpeed; camPosZ -= rightZ * camSpeed; break;
    case 'd': case 'D': camPosX += rightX * camSpeed; camPosZ += rightZ * camSpeed; break;
    case 'q': case 'Q': camPosY += camSpeed; break;
    case 'e': case 'E': camPosY -= camSpeed; break;
    case 27: exit(0); // ESC
    }
    glutPostRedisplay();
}


// Right click to look around (mouse control)
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
        else if (state == GLUT_UP)
        {
            rightMousePressed = false; 
            glutSetCursor(GLUT_CURSOR_INHERIT); 
        }
    }
}

// Mouse drag to rotate camera
void MouseMotion(int x, int y)
{
    if (rightMousePressed)
    {
        float deltaX = (float)(x - lastMouseX);
        float deltaY = (float)(y - lastMouseY);

        yaw += deltaX * mouseSensitivity;
        pitch -= deltaY * mouseSensitivity;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay(); 
    }
}

// Display function to render the scene
void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

 
    float dirX = cos(yaw * 3.14159f / 180.0f) * cos(pitch * 3.14159f / 180.0f);
    float dirY = sin(pitch * 3.14159f / 180.0f);
    float dirZ = sin(yaw * 3.14159f / 180.0f) * cos(pitch * 3.14159f / 180.0f);

    gluLookAt(camPosX, camPosY, camPosZ,
        camPosX + dirX, camPosY + dirY, camPosZ + dirZ,
        0.0f, 1.0f, 0.0f);

    glPushMatrix();
    float matrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    matrix[12] = matrix[13] = matrix[14] = 0;
    glLoadMatrixf(matrix);
    DrawSkybox(600.0f);
    glPopMatrix();

    DrawTerrain();

    glutSwapBuffers();
}

// Reshape
void Reshape(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

// Init
void Init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.2f, 0.2f, 0.2f, 1);

	// Fog setup
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);
    GLfloat fogColor[4] = { 0.6f, 0.7f, 0.8f, 1.0f }; 
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.003f);
    glHint(GL_FOG_HINT, GL_DONT_CARE);

    skyboxTex[0] = LoadTexture("px.png.png");
    skyboxTex[1] = LoadTexture("nx.png.png");
    skyboxTex[2] = LoadTexture("py.png.png");
    skyboxTex[3] = LoadTexture("ny.png.png");
    skyboxTex[4] = LoadTexture("pz.png.png");
    skyboxTex[5] = LoadTexture("nz.png.png");
    sandTexture = LoadTexture("sand.png");
}

// Main function
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Spongebob Game ");

    Init();

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(MouseClick);
    glutMotionFunc(MouseMotion);

    glutMainLoop();
    return 0;
}