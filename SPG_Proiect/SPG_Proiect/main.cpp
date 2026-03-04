#include <windows.h>
#include <freeglut.h>
#include <cmath>

#include "texture.h"
#include "skybox.h"
#include "terrain.h"
#include "camera.h"
void Init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.2f, 0.2f, 0.2f, 1);

   
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);
  
    GLfloat fogColor[4] = { 0.0f, 0.4f, 0.5f, 1.0f };
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.005f); 
    glHint(GL_FOG_HINT, GL_DONT_CARE);
   

    skyboxTex[0] = LoadTexture("px.png.png");
    skyboxTex[1] = LoadTexture("nx.png.png");
    skyboxTex[2] = LoadTexture("py.png.png");
    skyboxTex[3] = LoadTexture("ny.png.png");
    skyboxTex[4] = LoadTexture("pz.png.png");
    skyboxTex[5] = LoadTexture("nz.png.png");
    sandTexture = LoadTexture("sand.png");
    GenerateTerrain();
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float dirX = cos(yaw * 3.14159f / 180.0f) * cos(pitch * 3.14159f / 180.0f);
    float dirY = sin(pitch * 3.14159f / 180.0f);
    float dirZ = sin(yaw * 3.14159f / 180.0f) * cos(pitch * 3.14159f / 180.0f);

    gluLookAt(camPosX, camPosY, camPosZ,
        camPosX + dirX, camPosY + dirY, camPosZ + dirZ,
        0, 1, 0);

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

void Reshape(int w, int h)
{
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Spongebob Game");

    Init();

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(MouseClick);
    glutMotionFunc(MouseMotion);

    glutMainLoop();
    return 0;
}