#include "lighting.h"
#include <cmath>
#include "terrain.h"  

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

float lightPos[4] = { 200.0f, 300.0f, 100.0f, 0.0f };
float lightAmbient[4] = { 0.15f, 0.20f, 0.30f, 1.0f };
float lightDiffuse[4] = { 0.85f, 0.80f, 0.65f, 1.0f };
float lightSpecular[4] = { 0.60f, 0.60f, 0.50f, 1.0f };
float shadowMatrix[16] = {};
bool shadowsEnabled = true;
bool lightingEnabled = true;
static float lightAngle = 45.0f;

float GetLightAngle() { return lightAngle; }

void UpdateLightAngle(float delta)
{
    lightAngle += delta;
    if (lightAngle > 360.0f) lightAngle -= 360.0f;

    float rad = lightAngle * (float)M_PI / 180.0f;
    float radius = 400.0f;
    lightPos[0] = cos(rad) * radius;
    lightPos[1] = sin(rad) * radius;
    lightPos[2] = sin(rad * 0.5f) * 150.0f;
    lightPos[3] = 0.0f;

    float t = (sin(rad) + 1.0f) * 0.5f;
    float tDusk = fabs(cos(rad));
    lightDiffuse[0] = 0.3f + t * 0.55f + tDusk * 0.30f;
    lightDiffuse[1] = 0.2f + t * 0.60f;
    lightDiffuse[2] = 0.1f + t * 0.55f;
    lightAmbient[0] = 0.05f + t * 0.12f;
    lightAmbient[1] = 0.08f + t * 0.14f;
    lightAmbient[2] = 0.15f + t * 0.18f;
}

void InitLighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    float globalAmbient[4] = { 0.10f, 0.12f, 0.18f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    float matSpec[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0f);
}

void SetupLighting() {
   
    GLfloat light_position[] = { 100.0f, 150.0f, 50.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    lightPos[0] = 100.0f;
    lightPos[1] = 150.0f;
    lightPos[2] = 50.0f;
}

void BuildShadowMatrix(float groundY, float* lp, float* mat)
{
    float a = 0.0f, b = 1.0f, c = 0.0f, d = -groundY;
    float dot = a * lp[0] + b * lp[1] + c * lp[2] + d * lp[3];
    mat[0] = dot - lp[0] * a; mat[4] = -lp[0] * b;
    mat[8] = -lp[0] * c; mat[12] = -lp[0] * d;
    mat[1] = -lp[1] * a; mat[5] = dot - lp[1] * b;
    mat[9] = -lp[1] * c; mat[13] = -lp[1] * d;
    mat[2] = -lp[2] * a; mat[6] = -lp[2] * b;
    mat[10] = dot - lp[2] * c; mat[14] = -lp[2] * d;
    mat[3] = -lp[3] * a; mat[7] = -lp[3] * b;
    mat[11] = -lp[3] * c; mat[15] = dot - lp[3] * d;
}

void BeginShadowDraw()
{
    if (!shadowsEnabled) return;
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(-1.0f, -1.0f);
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_EQUAL, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
}

void EndShadowDraw()
{
    if (!shadowsEnabled) return;
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glEnable(GL_LIGHTING);
}
void InitLampLights()
{
    for (int i = 1; i <= 7; i++)
        glEnable(GL_LIGHT0 + i);
}

void SetupLampLights()
{
    
    struct LP { float x, z; };
    LP lamps[] = {
        {-12.f, 75.f}, {12.f,  75.f},
        {-12.f,-75.f}, {12.f, -75.f},
        { 75.f,-12.f}, {75.f,  12.f},
        {-48.f, 48.f},
    };

    float lampAmbient[4] = { 0.0f,  0.0f,  0.0f,  1.0f };
    float lampDiffuse[4] = { 1.0f,  0.95f, 0.6f,  1.0f }; 
    float lampSpecular[4] = { 0.8f,  0.8f,  0.5f,  1.0f };

    for (int i = 0; i < 7; i++)
    {
        GLenum light = GL_LIGHT1 + i;
        float lampH = 9.0f + 1.6f; 
        float y = GetHeightAt(lamps[i].x, lamps[i].z) + lampH;

        float pos[4] = { lamps[i].x + 0.85f, y, lamps[i].z, 1.0f }; 

        glLightfv(light, GL_AMBIENT, lampAmbient);
        glLightfv(light, GL_DIFFUSE, lampDiffuse);
        glLightfv(light, GL_SPECULAR, lampSpecular);
        glLightfv(light, GL_POSITION, pos);

       
        glLightf(light, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(light, GL_LINEAR_ATTENUATION, 0.05f);
        glLightf(light, GL_QUADRATIC_ATTENUATION, 0.002f);
    }
}