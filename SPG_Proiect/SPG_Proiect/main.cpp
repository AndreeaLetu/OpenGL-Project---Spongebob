#include <windows.h>
#include <freeglut.h>
#include <cmath>
#include <cstdio>
#include <vector>
#include "model_loader.h"
#include "texture.h"
#include "skybox.h"
#include "terrain.h"
#include "camera.h"
#include "world_objects.h"
#include "lighting.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

static bool IsOnRoad(float x, float z)
{
    float dist = sqrt(x * x + z * z);
    if (dist >= 18.0f && dist <= 42.0f) return true;
    if (z >= 37.0f && z <= 192.0f && fabs(x) <= 11.0f) return true;
    if (z <= -37.0f && z >= -192.0f && fabs(x) <= 11.0f) return true;
    if (x >= 37.0f && x <= 192.0f && fabs(z) <= 11.0f) return true;
    if (x <= -37.0f && x >= -192.0f && fabs(z) <= 11.0f) return true;
    return false;
}

static void BeginShadow()
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(-2.0f, -2.0f);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_EQUAL, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    glColor4f(0.08f, 0.08f, 0.08f, 0.60f);
}

static void EndShadow()
{
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

static void DrawProjectedShadow(float baseX, float baseY, float baseZ, float objectHeight, void(*drawFunc)())
{
    if (!shadowsEnabled || lightPos[1] <= 0.0f) return;
    if (IsOnRoad(baseX, baseZ)) return;

    float mat[16];
    BuildShadowMatrix(GetHeightAt(baseX, baseZ) + 0.25f, lightPos, mat);

    BeginShadow();
    glPushMatrix();
    glMultMatrixf(mat);
    drawFunc();
    glPopMatrix();
    EndShadow();
}

static void DrawProjectedShadowCustomLight(float baseX, float baseZ, float* customLightPos, void(*drawFunc)())
{
    if (!shadowsEnabled) return;
    if (IsOnRoad(baseX, baseZ)) return;

    float mat[16];
    BuildShadowMatrix(GetHeightAt(baseX, baseZ) + 0.25f, customLightPos, mat);

    BeginShadow();
    glPushMatrix();
    glMultMatrixf(mat);
    drawFunc();
    glPopMatrix();
    EndShadow();
}

static void DrawEllipseShadow(float cx, float cz, float rx, float rz, float alpha)
{
    if (!shadowsEnabled || lightPos[1] <= 0.0f) return;
    if (IsOnRoad(cx, cz)) return;
    float groundY = GetHeightAt(cx, cz) + 0.15f;
    BeginShadow();
    glColor4f(0.05f, 0.05f, 0.05f, alpha);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(cx, groundY, cz);
    for (int i = 0; i <= 32; i++) {
        float angle = 2.0f * (float)M_PI * i / 32;
        float px = cx + cos(angle) * rx;
        float pz = cz + sin(angle) * rz;
        glVertex3f(px, GetHeightAt(px, pz) + 0.15f, pz);
    }
    glEnd();
    EndShadow();
}


static void DrawPineappleShadowGeom() {
    glPushMatrix();
    glTranslatef(-55, GetHeightAt(-55, -25) + 1, -25);
    glScalef(0.02f, 0.02f, 0.02f);
    DrawOBJModelShadow(pineappleHouse);
    glPopMatrix();
}
static void DrawSquidwardShadowGeom() {
    glPushMatrix();
    glTranslatef(55, GetHeightAt(55, 25) + 0.2f, 25);
    glRotatef(180, 0, 1, 0);
    glScalef(1.2f, 1.2f, 1.2f);
    DrawOBJModelShadow(squidwardHouse);
    glPopMatrix();
}
static void DrawKrustyKrabShadowGeom() {
    glPushMatrix();
    glTranslatef(55, GetHeightAt(55, -25) + 0.2f, -25);
    glScalef(0.008f, 0.008f, 0.008f);
    DrawOBJModelShadow(krustyKrab);
    glPopMatrix();
}
static void DrawPatrickHouseShadowGeom() {
    glPushMatrix();
    glTranslatef(-55, GetHeightAt(-55, 25) + 0.1f, 25);
    glScalef(8, 8, 8);
    DrawOBJModelShadow(patrickHouse);
    glPopMatrix();
}

static void DrawSignShadowGeom() {
    float y = GetHeightAt(60.0f, -30.0f);
    GLUquadric* q = gluNewQuadric();
    glPushMatrix();
    glTranslatef(60.0f, y, -30.0f);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 0.15f, 0.12f, 7.0f, 8, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(60.0f, y + 8.5f, -30.0f);
    glScalef(2.8f, 3.5f, 0.4f);
    glutSolidSphere(1.0f, 14, 10);
    glPopMatrix();
    gluDeleteQuadric(q);
}

static float g_lampX = 0, g_lampZ = 0;
static void DrawLampShadowGeom() {
    float y = GetHeightAt(g_lampX, g_lampZ);
    GLUquadric* q = gluNewQuadric();
    glPushMatrix();
    glTranslatef(g_lampX, y, g_lampZ);
    glScalef(1.5f, 1.5f, 1.5f);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 0.18f, 0.14f, 12.0f, 8, 1);
    glPopMatrix();
    gluDeleteQuadric(q);
}

static float g_benchX = 0, g_benchZ = 0, g_benchRot = 0;
static void DrawBenchShadowGeom() {
    float y = GetHeightAt(g_benchX, g_benchZ);
    glPushMatrix();
    glTranslatef(g_benchX, y, g_benchZ);
    glRotatef(g_benchRot, 0, 1, 0);
    glScalef(1.6f, 1.6f, 1.6f);
    glPushMatrix(); glTranslatef(0, 1.0f, 0); glScalef(4.0f, 0.25f, 1.4f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 2.5f, -0.55f); glScalef(4.2f, 2.8f, 0.3f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(-2.1f, 1.8f, 0); glScalef(0.25f, 2.2f, 1.4f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(2.1f, 1.8f, 0); glScalef(0.25f, 2.2f, 1.4f); glutSolidCube(1.0f); glPopMatrix();
    float legX[4] = { -1.6f,-1.6f,1.6f,1.6f }, legZ[4] = { -0.5f, 0.5f,-0.5f,0.5f };
    GLUquadric* q = gluNewQuadric();
    for (int i = 0; i < 4; i++) {
        glPushMatrix(); glTranslatef(legX[i], 0, legZ[i]); glRotatef(-90, 1, 0, 0);
        gluCylinder(q, 0.16f, 0.13f, 1.0f, 8, 1); glPopMatrix();
    }
    gluDeleteQuadric(q);
    glPopMatrix();
}
static LampPos g_rightLamps[] = { {12.0f, 75.0f}, {12.0f, 125.0f}, {75.0f, -12.0f}, {125.0f,-12.0f}, {-75.0f, 12.0f}, {-125.0f,12.0f} };
static BenchInfo g_rightBenches[] = { {12.0f, 70.0f, 270.0f}, {12.0f, 80.0f, 270.0f}, {12.0f, 120.0f, 270.0f}, {12.0f, 130.0f, 270.0f}, { 70.0f,-12.0f, 0.0f}, { 80.0f,-12.0f, 0.0f}, {120.0f,-12.0f, 0.0f}, {130.0f,-12.0f, 0.0f}, {-70.0f, 12.0f, 180.0f}, {-80.0f, 12.0f, 180.0f}, {-120.0f,12.0f, 180.0f}, {-130.0f,12.0f, 180.0f} };

static void DrawLampToBenchShadow(float benchX, float benchZ, float benchRot, float lampX, float lampZ)
{
    float fakeLightPos[4] = { lampX, 15.0f, lampZ, 1.0f };
    float mat[16];
    BuildShadowMatrix(GetHeightAt(benchX, benchZ) + 0.1f, fakeLightPos, mat);
    BeginShadow();
    glColor4f(0.02f, 0.02f, 0.02f, 0.7f);
    glPushMatrix();
    glMultMatrixf(mat);
    g_benchX = benchX; g_benchZ = benchZ; g_benchRot = benchRot;
    DrawBenchShadowGeom();
    glPopMatrix();
    EndShadow();
}

static void DrawAllShadows()
{
    if (!shadowsEnabled || lightPos[1] <= 0.0f) return;
    glClear(GL_STENCIL_BUFFER_BIT);

    
    DrawProjectedShadow(-55, 0, -25, 12.0f, DrawPineappleShadowGeom);
    DrawProjectedShadow(55, 0, 25, 10.0f, DrawSquidwardShadowGeom);
    DrawProjectedShadow(-55, 0, 25, 5.0f, DrawPatrickHouseShadowGeom);
    {
        float kkLightPos[4] = { -100.0f, 200.0f, 100.0f, 0.0f };
        DrawProjectedShadowCustomLight(55.0f, -25.0f, kkLightPos, DrawKrustyKrabShadowGeom);
    }

   
    {
        float signLightPos[4] = { -200.0f, 250.0f, 0.0f, 0.0f };
        DrawProjectedShadowCustomLight(60.0f, -30.0f, signLightPos, DrawSignShadowGeom);
    }

    
    if (shadowsEnabled && lightPos[1] > 0.0f) {
        BeginShadow();
        DrawAllPlantsShadowGeom();
        EndShadow();
    }

   
    for (auto& lp : g_rightLamps) {
        g_lampX = lp.x; g_lampZ = lp.z;
        DrawProjectedShadow(lp.x, GetHeightAt(lp.x, lp.z), lp.z, 14.0f, DrawLampShadowGeom);
        DrawEllipseShadow(lp.x, lp.z, 2.2f, 2.2f, 0.4f);
    }

    for (auto& b : g_rightBenches) {
        g_benchX = b.x; g_benchZ = b.z; g_benchRot = b.rotY;
        DrawProjectedShadow(b.x, GetHeightAt(b.x, b.z), b.z, 3.5f, DrawBenchShadowGeom);
        float lX = (fabs(b.x) == 12.0f) ? b.x : (b.x > 0 ? 75.0f : -75.0f);
        float lZ = (fabs(b.x) == 12.0f) ? ((b.z > 0 && b.z < 100.0f) ? 75.0f : 125.0f) : b.z;
        DrawLampToBenchShadow(b.x, b.z, b.rotY, lX, lZ);
    }
}

void Init() {
    glEnable(GL_DEPTH_TEST); glEnable(GL_TEXTURE_2D);
    glClearColor(0.02f, 0.05f, 0.1f, 1.0f);
    glEnable(GL_FOG); glFogi(GL_FOG_MODE, GL_EXP2);
    GLfloat fogColor[4] = { 0.0f, 0.4f, 0.5f, 1.0f };
    glFogfv(GL_FOG_COLOR, fogColor); glFogf(GL_FOG_DENSITY, 0.005f);
   
    skyboxTex[0] = LoadTexture("px.png.png"); skyboxTex[1] = LoadTexture("nx.png.png");
    skyboxTex[2] = LoadTexture("py.png.png"); skyboxTex[3] = LoadTexture("ny.png.png");
    skyboxTex[4] = LoadTexture("pz.png.png"); skyboxTex[5] = LoadTexture("nz.png.png");
    sandTexture = LoadTexture("sand.png");
    GenerateTerrain(); InitWorldElements(); InitLighting(); InitLampLights();
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    if (followBoat) {

        float rad = (boatAngle - 180.0f) * (float)M_PI / 180.0f;
        float camDist  = 25.0f;
        float camHeight = 12.0f;
        float fcX = boatPosX - sin(rad) * camDist;
        float fcY = boatPosY + camHeight;
        float fcZ = boatPosZ - cos(rad) * camDist;
        float tX = boatPosX + sin(rad) * 8.0f;
        float tY = boatPosY + 2.0f;
        float tZ = boatPosZ + cos(rad) * 8.0f;
        gluLookAt(fcX, fcY, fcZ, tX, tY, tZ, 0, 1, 0);
    }
   else {
    float rad = sbAngle * (float)M_PI / 180.0f;
    float camDist  = 30.0f;
    float camHeight = 15.0f;

    float fcX = sbPosX - sin(rad) * camDist;
    float fcY = sbPosY + camHeight;
    float fcZ = sbPosZ - cos(rad) * camDist;

    float tX = sbPosX;
    float tY = sbPosY + 3.0f;
    float tZ = sbPosZ;

    gluLookAt(fcX, fcY, fcZ, tX, tY, tZ, 0, 1, 0);
}

    SetupLighting(); SetupLampLights(); DrawBubbles();

    glPushMatrix();
    float matrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    matrix[12] = matrix[13] = matrix[14] = 0;
    glLoadMatrixf(matrix);
    DrawSkybox(600.0f);
    glPopMatrix();

    DrawTerrain();
    DrawAllShadows();

    glDisable(GL_BLEND); glDisable(GL_STENCIL_TEST);
    glEnable(GL_TEXTURE_2D); glEnable(GL_DEPTH_TEST);

    DrawAllRoads();
    DrawStaticObjects();
    DrawStreetLamps();
    DrawControlledBoat();
    DrawSpongebob();
    DrawBubbles();

    glutSwapBuffers();
}

void Reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(60.0, (double)w / h, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void IdleFunc() { UpdateCarMovement(); glutPostRedisplay();   UpdateBoat(); UpdateSpongebob(); UpdateBubbles();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Spongebob World - Full Shadows");
    Init();
    glutDisplayFunc(Display); glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard); glutMouseFunc(MouseClick);
    glutMotionFunc(MouseMotion); glutSpecialFunc(SpecialKeys); glutIdleFunc(IdleFunc);
    glutMainLoop();
    return 0;
}