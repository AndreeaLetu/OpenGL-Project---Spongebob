#include <windows.h>
#include <freeglut.h>
#include <cmath>
#include <vector>
#include <cstdio>
#include "world_objects.h"
#include "terrain.h"
#include "texture.h"
#include "model_loader.h"
#include "camera.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

GLuint roadTexture;
GLuint signTexture;

OBJModel pineappleHouse, squidwardHouse, patrickHouse, krustyKrab, signSpongebob;
OBJModel krustyKrabCar;
OBJModel policeBoat;

OBJModel spongebobModel;
float sbPosX = 0.0f;
float sbPosY = 0.0f;
float sbPosZ = 60.0f;
float sbAngle = 0.0f;

float carOrbitAngle = 0.0f;
float carOrbitSpeed = 20.0f;
float carOrbitRadius = 30.0f;

float boatPosX = 0.0f;
float boatPosY = 0.0f;
float boatPosZ = 50.0f;  
float boatAngle = 0.0f;
float boatSpeed = 0.0f;


struct Bubble {
    float x, y, z;
    float speedX, speedY, speedZ;
    float size;
    float alpha;
};

static const int NUM_BUBBLES = 40;
static Bubble bubbles[NUM_BUBBLES];
void InitBubbles()
{
   
    float posX[] = {
        -120.0f,  80.0f, -50.0f,  30.0f, 110.0f,
        -90.0f,   45.0f,-130.0f,  70.0f, -20.0f,
         15.0f, -75.0f,  95.0f, -40.0f,  60.0f,
        140.0f, -110.0f, 25.0f, -85.0f, 100.0f,
        -55.0f,   35.0f,-145.0f, 55.0f, -10.0f,
         85.0f,  -65.0f, 120.0f,-30.0f,  75.0f,
        -100.0f,  50.0f, -15.0f, 90.0f,-125.0f,
         40.0f,  -80.0f, 130.0f,-45.0f,  20.0f
    };
    float posZ[] = {
         30.0f, -90.0f, 120.0f, -50.0f,  70.0f,
        -130.0f, 60.0f,  10.0f, -75.0f, 100.0f,
        -20.0f,  85.0f, -55.0f, 140.0f, -95.0f,
         45.0f, -110.0f, 80.0f, -35.0f,  55.0f,
        125.0f,  -65.0f, 15.0f,-120.0f,  90.0f,
        -40.0f,  105.0f,-85.0f,  35.0f, -60.0f,
         70.0f, -145.0f, 50.0f,-100.0f,  25.0f,
        -70.0f,   95.0f,-25.0f, 115.0f, -50.0f
    };
    float sizes[] = {
        0.3f, 0.5f, 0.2f, 0.4f, 0.6f,
        0.3f, 0.2f, 0.5f, 0.4f, 0.3f,
        0.6f, 0.2f, 0.4f, 0.3f, 0.5f,
        0.2f, 0.4f, 0.6f, 0.3f, 0.2f,
        0.5f, 0.3f, 0.4f, 0.2f, 0.6f,
        0.3f, 0.5f, 0.2f, 0.4f, 0.3f,
        0.6f, 0.2f, 0.5f, 0.3f, 0.4f,
        0.2f, 0.6f, 0.3f, 0.5f, 0.4f
    };
    float speedsY[] = {
        0.04f, 0.02f, 0.05f, 0.03f, 0.06f,
        0.02f, 0.04f, 0.03f, 0.05f, 0.02f,
        0.06f, 0.03f, 0.04f, 0.02f, 0.05f,
        0.03f, 0.06f, 0.02f, 0.04f, 0.03f,
        0.05f, 0.02f, 0.06f, 0.04f, 0.03f,
        0.05f, 0.02f, 0.04f, 0.06f, 0.03f,
        0.02f, 0.05f, 0.03f, 0.06f, 0.04f,
        0.03f, 0.02f, 0.05f, 0.04f, 0.06f
    };

    for (int i = 0; i < NUM_BUBBLES; i++) {
        bubbles[i].x = posX[i];
        bubbles[i].z = posZ[i];
        bubbles[i].y = GetHeightAt(posX[i], posZ[i]) + 1.0f + i * 0.5f;
        bubbles[i].speedX = (i % 2 == 0) ? 0.02f : -0.02f;
        bubbles[i].speedY = speedsY[i];
        bubbles[i].speedZ = (i % 3 == 0) ? 0.02f : -0.02f;
        bubbles[i].size = sizes[i];
        bubbles[i].alpha = 0.6f;
    }
}
void UpdateBubbles()
{
    float t = GetTickCount() * 0.001f;

  
    static float respawnX[] = {
        -120.0f, 80.0f, -50.0f, 30.0f, 110.0f,
        -90.0f, 45.0f, -130.0f, 70.0f, -20.0f,
        15.0f, -75.0f, 95.0f, -40.0f, 60.0f,
        140.0f, -110.0f, 25.0f, -85.0f, 100.0f,
        -55.0f, 35.0f, -145.0f, 55.0f, -10.0f,
        85.0f, -65.0f, 120.0f, -30.0f, 75.0f,
        -100.0f, 50.0f, -15.0f, 90.0f, -125.0f,
        40.0f, -80.0f, 130.0f, -45.0f, 20.0f
    };
    static float respawnZ[] = {
        30.0f, -90.0f, 120.0f, -50.0f, 70.0f,
        -130.0f, 60.0f, 10.0f, -75.0f, 100.0f,
        -20.0f, 85.0f, -55.0f, 140.0f, -95.0f,
        45.0f, -110.0f, 80.0f, -35.0f, 55.0f,
        125.0f, -65.0f, 15.0f, -120.0f, 90.0f,
        -40.0f, 105.0f, -85.0f, 35.0f, -60.0f,
        70.0f, -145.0f, 50.0f, -100.0f, 25.0f,
        -70.0f, 95.0f, -25.0f, 115.0f, -50.0f
    };

    for (int i = 0; i < NUM_BUBBLES; i++) {
        bubbles[i].y += bubbles[i].speedY;
        bubbles[i].x += bubbles[i].speedX + sin(t + i * 1.3f) * 0.02f;
        bubbles[i].z += bubbles[i].speedZ + cos(t + i * 0.9f) * 0.02f;

        float maxHeight = GetHeightAt(bubbles[i].x, bubbles[i].z) + 30.0f;

        if (bubbles[i].y > maxHeight ||
            bubbles[i].x > 150.0f || bubbles[i].x < -150.0f ||
            bubbles[i].z >  150.0f || bubbles[i].z < -150.0f)
        {
            bubbles[i].x = respawnX[i];
            bubbles[i].z = respawnZ[i];
            bubbles[i].y = GetHeightAt(respawnX[i], respawnZ[i]) + 1.0f;
        }
    }
}

void DrawBubbles()
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    for (int i = 0; i < NUM_BUBBLES; i++) {
        glPushMatrix();
        glTranslatef(bubbles[i].x, bubbles[i].y, bubbles[i].z);
        glColor3f(0.6f, 0.85f, 1.0f);
        glutSolidSphere(bubbles[i].size, 10, 10); 
        glPopMatrix();
    }

    glEnable(GL_LIGHTING);
}
struct Building { float x, z, radius; };
static Building buildings[] = {
   
    { -55.0f, -25.0f, 12.0f },   
    {  55.0f,  25.0f, 10.0f },   
    { -55.0f,  25.0f, 14.0f },   
    {  55.0f, -25.0f, 12.0f },   

    
    {  85.0f,  85.0f,  8.0f },   
    { -95.0f, -70.0f,  8.0f },   

    
    {  92.0f,  80.0f,  3.0f },   
    { -88.0f, -62.0f,  3.0f },  

  
    {  12.0f,  75.0f,  2.5f },
    {  12.0f, 125.0f,  2.5f },
    {  75.0f, -12.0f,  2.5f },
    { 125.0f, -12.0f,  2.5f },
    { -75.0f,  12.0f,  2.5f },
    {-125.0f,  12.0f,  2.5f },

 
    {  12.0f,  70.0f,  3.5f },
    {  12.0f,  80.0f,  3.5f },
    {  12.0f, 120.0f,  3.5f },
    {  12.0f, 130.0f,  3.5f },
    {  70.0f, -12.0f,  3.5f },
    {  80.0f, -12.0f,  3.5f },
    { 120.0f, -12.0f,  3.5f },
    { 130.0f, -12.0f,  3.5f },
    { -70.0f,  12.0f,  3.5f },
    { -80.0f,  12.0f,  3.5f },
    {-120.0f,  12.0f,  3.5f },
    {-130.0f,  12.0f,  3.5f },
};
static int numBuildings = sizeof(buildings) / sizeof(buildings[0]);

static bool CheckBoatCollision(float newX, float newZ)
{
    
    for (int i = 0; i < numBuildings; i++) {
        float dx = newX - buildings[i].x;
        float dz = newZ - buildings[i].z;
        float dist = sqrt(dx * dx + dz * dz);
        if (dist < buildings[i].radius)
            return true;
    }

    float rad = carOrbitAngle * (float)M_PI / 180.0f;
    float carX = cos(rad) * carOrbitRadius;
    float carZ = sin(rad) * carOrbitRadius;
    float dx = newX - carX;
    float dz = newZ - carZ;
    float dist = sqrt(dx * dx + dz * dz);
    if (dist < 6.0f)  
        return true;

    return false;
}


void UpdateBoat()
{
    float rad = (boatAngle - 180.0f) * (float)M_PI / 180.0f;

    float newX = boatPosX + sin(rad) * boatSpeed;
    float newZ = boatPosZ + cos(rad) * boatSpeed;

    if (!CheckBoatCollision(newX, newZ)) {
        boatPosX = newX;
        boatPosZ = newZ;
    }
    else {
        float onlyX = boatPosX + sin(rad) * boatSpeed;
        if (!CheckBoatCollision(onlyX, boatPosZ)) {
            boatPosX = onlyX;
        }
        else {
            float onlyZ = boatPosZ + cos(rad) * boatSpeed;
            if (!CheckBoatCollision(boatPosX, onlyZ)) {
                boatPosZ = onlyZ;
            }
            else {
                boatSpeed *= -0.3f;
            }
        }
    }

  
    float terrainY = GetHeightAt(boatPosX, boatPosZ);

    if (IsPointOnRoad(boatPosX, boatPosZ)) {
       
        boatPosY = terrainY + 1.2f;
    }
    else {
       
        boatPosY = terrainY;
    }

    boatSpeed *= 0.95f;
    if (fabs(boatSpeed) < 0.001f) boatSpeed = 0.0f;
}


void DrawControlledBoat() {
    glPushMatrix();
  
    glTranslatef(boatPosX, boatPosY + 0.3f, boatPosZ);
    glRotatef(boatAngle, 0, 1, 0);
    glRotatef(-90.0f, 0, 1, 0);
    glScalef(0.7f, 0.7f, 0.7f);
    DrawOBJModel(policeBoat);
    glPopMatrix();
}
void DrawNeonKelp(float x, float z, float h, float off) {
    float yb = GetHeightAt(x, z);
    float t = GetTickCount() * 0.001f;
    glPushMatrix();
    glTranslatef(x, yb, z);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= 8; i++) {
        float f = (float)i / 8.0f;
        float wv = sin(t * 2.0f + f * 3.0f + off) * 0.5f * f;
        glColor3f(0.0f, 0.9f, 0.2f);
        glVertex3f(wv - 0.2f, i * (h / 8.0f), 0);
        glVertex3f(wv + 0.2f, i * (h / 8.0f), 0.1f);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void DrawPinkBubblePlant(float x, float z, float h, float off) {
    float yb = GetHeightAt(x, z);
    glPushMatrix();
    glTranslatef(x, yb, z);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor3f(0.9f, 0.1f, 0.6f);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    GLUquadric* q = gluNewQuadric();
    gluCylinder(q, 0.2f, 0.1f, h, 6, 1);
    gluDeleteQuadric(q);
    glPopMatrix();
    for (int i = 1; i <= 4; i++) {
        glPushMatrix();
        float ang = i * 1.5f + off;
        glTranslatef(cos(ang) * 0.6f, i * (h / 4.0f), sin(ang) * 0.6f);
        glColor3f(1.0f, 0.4f, 0.8f);
        glutSolidSphere(0.5f, 8, 8);
        glPopMatrix();
    }
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void DrawCyanVine(float x, float z, float h, float off) {
    float yb = GetHeightAt(x, z);
    float t = GetTickCount() * 0.001f;
    glPushMatrix();
    glTranslatef(x, yb, z);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glLineWidth(4.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 12; i++) {
        float f = (float)i / 12.0f;
        float ang = f * 5.0f + t + off;
        glColor3f(0.1f, 0.8f, 1.0f);
        glVertex3f(cos(ang) * 0.5f * f, i * (h / 12.0f), sin(ang) * 0.5f * f);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void DrawRoadStrip(float x1, float z1, float x2, float z2, float w) {
    float dx = x2 - x1, dz = z2 - z1;
    float dst = sqrt(dx * dx + dz * dz);
    if (dst < 0.1f) return;
    float px = -dz / dst * (w / 2.0f);
    float pz = dx / dst * (w / 2.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, roadTexture);
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1);
    int steps = 40;
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        float cx = x1 + t * dx, cz = z1 + t * dz;
        float lx = cx - px, lz = cz - pz;
        float rx = cx + px, rz = cz + pz;
        glTexCoord2f(0, t * 5); glVertex3f(lx, GetHeightAt(lx, lz) + 1.2f, lz);
        glTexCoord2f(1, t * 5); glVertex3f(rx, GetHeightAt(rx, rz) + 1.2f, rz);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

void DrawRoundabout(float iR, float oR) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, roadTexture);
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1);
    int segments = 120;
    float texScale = 0.15f;
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        float a = 2.0f * M_PI * i / segments;
        float xi = cos(a) * iR, zi = sin(a) * iR;
        float xo = cos(a) * oR, zo = sin(a) * oR;
        float arcLen = 2.0f * M_PI * oR * (float)i / segments;
        glTexCoord2f(0.0f, arcLen * texScale); glVertex3f(xi, GetHeightAt(xi, zi) + 1.2f, zi);
        glTexCoord2f(1.0f, arcLen * texScale); glVertex3f(xo, GetHeightAt(xo, zo) + 1.2f, zo);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

void UpdateCarMovement() {
    static DWORD lastTime = GetTickCount();
    DWORD currentTime = GetTickCount();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    carOrbitAngle += carOrbitSpeed * deltaTime;
    if (carOrbitAngle >= 360.0f) carOrbitAngle -= 360.0f;
    glutPostRedisplay();
}

void InitWorldElements() {
    roadTexture = LoadTexture("road.jpg");
    signTexture = LoadTexture("models/sign/Env_Island_GooLagoon_Panel_01.jpg");
    LoadOBJModel("models/pineapple/pineapple.obj", pineappleHouse);
    LoadOBJModel("models/squidward/MSH_SquidwardHouse.obj", squidwardHouse);
    LoadOBJModel("models/patrick/MSH_PatrickHouse.obj", patrickHouse);
    LoadOBJModel("models/krusty/SK_BB_Krusty_Krab.obj", krustyKrab);
    LoadOBJModel("models/sign/Env_GooLagoon_Panel01.obj", signSpongebob);
    if (!LoadOBJModel("models/car/KKCar.obj", krustyKrabCar))
        printf("EROARE: KKCar.obj nu a fost gasit!\n");
    if (!LoadOBJModel("models/police/policeboat.obj", policeBoat))
        printf("EROARE: policeboat.obj nu a fost gasit!\n");
    if (!LoadOBJModel("models/spongebob/spongebob.obj", spongebobModel))
        printf("EROARE: spongebob.obj nu a fost gasit!\n");
    InitBubbles();
}

void DrawSpongebob()
{
    if (spongebobModel.subMeshes.empty()) return;
    if (followBoat) return;

    glPushMatrix();
    glTranslatef(sbPosX, sbPosY + 0.5f, sbPosZ); 
    glRotatef(sbAngle, 0, 1, 0);
    glScalef(3.0f, 3.0f, 3.0f);
    DrawOBJModel(spongebobModel);
    glPopMatrix();
}
void UpdateSpongebob()
{
    float terrainY = GetHeightAt(sbPosX, sbPosZ);

    if (IsPointOnRoad(sbPosX, sbPosZ))
        sbPosY = terrainY + 1.2f;  
    else
        sbPosY = terrainY;         
}
void DrawAllRoads() {
    DrawRoundabout(20, 40);
    DrawRoadStrip(0, 39.5f, 0, 190, 20);
    DrawRoadStrip(0, -39.5f, 0, -190, 20);
    DrawRoadStrip(39.5f, 0, 190, 0, 20);
    DrawRoadStrip(-39.5f, 0, -190, 0, 20);
}

bool IsPointOnRoad(float x, float z) {
    float dist = sqrt(x * x + z * z);
    if (dist >= 20.0f && dist <= 40.0f) return true;
    if (z >= 39.5f && z <= 190.0f && fabs(x) <= 10.0f) return true;
    if (z <= -39.5f && z >= -190.0f && fabs(x) <= 10.0f) return true;
    if (x >= 39.5f && x <= 190.0f && fabs(z) <= 10.0f) return true;
    if (x <= -39.5f && x >= -190.0f && fabs(z) <= 10.0f) return true;
    return false;
}

void DrawShipWreck(float x, float z) {
    float y = GetHeightAt(x, z);
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(20.0f, 0, 1, 0); glRotatef(15.0f, 0, 0, 1);
    glDisable(GL_TEXTURE_2D); glDisable(GL_LIGHTING);
    glColor3f(0.45f, 0.25f, 0.10f);
    glPushMatrix(); glScalef(6.0f, 1.6f, 2.5f); glutSolidCube(2.0f); glPopMatrix();
    glColor3f(0.35f, 0.18f, 0.08f);
    glPushMatrix(); glTranslatef(-3.0f, 1.0f, 0.0f); glRotatef(-30.0f, 0, 0, 1);
    glScalef(3.0f, 0.6f, 2.0f); glutSolidCube(2.0f); glPopMatrix();
    glColor3f(0.50f, 0.30f, 0.10f);
    glPushMatrix(); glTranslatef(1.0f, 2.5f, 0.0f); glRotatef(-60.0f, 0, 0, 1);
    GLUquadric* q = gluNewQuadric(); glRotatef(-90.0f, 1, 0, 0);
    gluCylinder(q, 0.15, 0.10, 4.0, 8, 1); gluDeleteQuadric(q); glPopMatrix();
    glPopMatrix();
}

void DrawAnchor(float x, float z) {
    float y = GetHeightAt(x, z);
    glPushMatrix(); glTranslatef(x, y, z);
    glDisable(GL_TEXTURE_2D); glDisable(GL_LIGHTING);
    glColor3f(0.2f, 0.2f, 0.25f);
    GLUquadric* q = gluNewQuadric();
    glPushMatrix(); glRotatef(-90, 1, 0, 0); gluCylinder(q, 0.15, 0.15, 4.0, 10, 1); glPopMatrix();
    glPushMatrix(); glTranslatef(0, -0.5f, 0); glutSolidSphere(0.3f, 10, 10); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.5f, 0); glRotatef(45, 0, 0, 1);
    glScalef(2.0f, 0.2f, 0.2f); glutSolidCube(1.0f); glPopMatrix();
    gluDeleteQuadric(q); glPopMatrix();
}

void DrawBrokenPlank(float x, float z, float rotY) {
    float y = GetHeightAt(x, z);
    glPushMatrix(); glTranslatef(x, y + 0.15f, z);
    glRotatef(rotY, 0, 1, 0); glRotatef(-8.0f, 1, 0, 0);
    glDisable(GL_TEXTURE_2D); glDisable(GL_LIGHTING);
    glColor3f(0.45f, 0.25f, 0.10f);
    glScalef(3.0f, 0.2f, 0.8f); glutSolidCube(1.0f);
    glPopMatrix();
}

void DrawLightCone(float x, float baseY, float z, float h) {
    float t = GetTickCount() * 0.002f;
    float flicker = 0.9f + sin(t) * 0.1f;
    float coneLength = 18.0f, coneRadius = 6.5f;
    int steps = 36;
    glDisable(GL_TEXTURE_2D); glDisable(GL_LIGHTING);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);
    glPushMatrix();
    glTranslatef(x - (0.85f * 1.5f), baseY + (h + 1.85f) * 1.5f, z);
    glRotatef(90.0f, 1, 0, 0);
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1.0f, 0.95f, 0.55f, 0.05f * flicker);
    glVertex3f(0, 0, 0);
    for (int i = 0; i <= steps; i++) {
        float a = 2.0f * M_PI * i / steps;
        glColor4f(1.0f, 0.95f, 0.55f, 0.0f);
        glVertex3f(cos(a) * coneRadius, sin(a) * coneRadius, coneLength);
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1.0f, 0.98f, 0.7f, 0.12f * flicker);
    glVertex3f(0, 0, 0);
    for (int i = 0; i <= steps; i++) {
        float a = 2.0f * M_PI * i / steps;
        glColor4f(1.0f, 0.98f, 0.7f, 0.0f);
        glVertex3f(cos(a) * (coneRadius * 0.4f), sin(a) * (coneRadius * 0.4f), coneLength);
    }
    glEnd();
    glPopMatrix();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void DrawCartoonBench(float x, float z, float rotY) {
    float y = GetHeightAt(x, z);
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotY, 0, 1, 0);
    glScalef(1.6f, 1.6f, 1.6f);
    glDisable(GL_TEXTURE_2D); glDisable(GL_LIGHTING);

    glColor3f(0.15f, 0.75f, 0.25f);
    glPushMatrix(); glTranslatef(0, 1.0f, 0); glScalef(4.0f, 0.25f, 1.4f); glutSolidCube(1.0f); glPopMatrix();

    glColor3f(1.0f, 0.85f, 0.0f);
    glPushMatrix(); glTranslatef(0, 1.13f, 0); glScalef(4.1f, 0.08f, 1.5f); glutSolidCube(1.0f); glPopMatrix();

    glColor3f(0.10f, 0.45f, 0.90f);
    glPushMatrix(); glTranslatef(0, 1.75f, -0.55f); glScalef(4.0f, 0.9f, 0.22f); glutSolidCube(1.0f); glPopMatrix();

    glColor3f(1.0f, 0.85f, 0.0f);
    glPushMatrix(); glTranslatef(0, 2.22f, -0.55f); glScalef(4.1f, 0.12f, 0.28f); glutSolidCube(1.0f); glPopMatrix();

    float legX[4] = { -1.6f,-1.6f,1.6f,1.6f }, legZ[4] = { -0.5f,0.5f,-0.5f,0.5f };
    GLUquadric* q = gluNewQuadric();
    for (int i = 0; i < 4; i++) {
        glColor3f(0.95f, 0.45f, 0.05f);
        glPushMatrix(); glTranslatef(legX[i], 0, legZ[i]); glRotatef(-90, 1, 0, 0);
        gluCylinder(q, 0.16f, 0.13f, 1.0f, 10, 1); glPopMatrix();
        glColor3f(0.60f, 0.25f, 0.05f);
        glPushMatrix(); glTranslatef(legX[i], 0.05f, legZ[i]); glScalef(1.0f, 0.2f, 1.0f);
        glutSolidSphere(0.22f, 10, 6); glPopMatrix();
    }

    glColor3f(0.95f, 0.35f, 0.55f);
    glPushMatrix(); glTranslatef(-2.1f, 1.45f, 0); glScalef(0.22f, 0.65f, 1.4f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(2.1f, 1.45f, 0); glScalef(0.22f, 0.65f, 1.4f); glutSolidCube(1.0f); glPopMatrix();

    glColor3f(1.0f, 1.0f, 0.3f);
    for (int i = -1; i <= 1; i++) {
        glPushMatrix(); glTranslatef(i * 1.3f, 1.75f, -0.67f);
        glScalef(0.5f, 0.5f, 0.1f); glutSolidSphere(0.3f, 6, 4); glPopMatrix();
    }
    gluDeleteQuadric(q);
    glPopMatrix();
}

void DrawStaticObjects() {
    glPushMatrix();
    glTranslatef(-55, GetHeightAt(-55, -25) + 1, -25);
    glScalef(0.02f, 0.02f, 0.02f);
    DrawOBJModel(pineappleHouse);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(55, GetHeightAt(55, 25) + 0.2f, 25);
    glRotatef(180, 0, 1, 0); glScalef(1.2f, 1.2f, 1.2f);
    DrawOBJModel(squidwardHouse);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-55, GetHeightAt(-55, 25) + 0.1f, 25);
    glScalef(8, 8, 8);
    DrawOBJModel(patrickHouse);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(55, GetHeightAt(55, -25) + 0.2f, -25);
    glScalef(0.008f, 0.008f, 0.008f);
    DrawOBJModel(krustyKrab);
    glPopMatrix();
    glPushMatrix();
    {
        float rad = carOrbitAngle * M_PI / 180.0f;
        float cx = cos(rad) * carOrbitRadius;
        float cz = sin(rad) * carOrbitRadius;
        float cy = GetHeightAt(cx, cz) + 2.2f;  

        glTranslatef(cx, cy, cz);
        float dirX = -sin(rad), dirZ = cos(rad);
        float angle = atan2(dirX, dirZ) * 180.0f / M_PI;
        glRotatef(angle + 90.0f, 0, 1, 0);
        glRotatef(180.0f, 0, 1, 0);
        glScalef(3.0f, 3.0f, 3.0f);
        glTranslatef(0, 0.2f, 0);
        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 0.6f, 0.1f);
        DrawOBJModel(krustyKrabCar);
        glEnable(GL_LIGHTING);
    }
    glPopMatrix();

    DrawShipWreck(85.0f, 85.0f);
    DrawShipWreck(-95.0f, -70.0f);
    DrawAnchor(92.0f, 80.0f);
    DrawAnchor(-88.0f, -62.0f);

    DrawBrokenPlank(70.0f, 84.0f, 25.0f);
    DrawBrokenPlank(73.0f, 87.0f, 80.0f);
    DrawBrokenPlank(77.0f, 83.0f, 130.0f);
    DrawBrokenPlank(-83.0f, -66.0f, 40.0f);
    DrawBrokenPlank(-87.0f, -70.0f, 95.0f);
    DrawBrokenPlank(-91.0f, -73.0f, 10.0f);

    srand(42);
    for (int i = 0; i < 60; i++) {
        float rx = (rand() % 300) - 150.0f;
        float rz = (rand() % 300) - 150.0f;
        if (IsPointOnRoad(rx, rz)) continue;
        int type = rand() % 3;
        
        if (type == 0) DrawNeonKelp(rx, rz, 6.0f, (float)i);
        else if (type == 1) DrawPinkBubblePlant(rx, rz, 5.0f, (float)i);
        else                DrawCyanVine(rx, rz, 7.0f, (float)i);
    }
}

void DrawSingleMarineLamp(float x, float z, float h) {
    float y = GetHeightAt(x, z);
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(1.5f, 1.5f, 1.5f);
    GLUquadric* q = gluNewQuadric();

    glColor3f(0.0f, 0.65f, 0.85f);
    glPushMatrix(); glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 0.18f, 0.14f, h, 12, 3); glPopMatrix();

    glColor3f(0.75f, 0.30f, 0.85f);
    glPushMatrix(); glTranslatef(0, h, 0);
    glRotatef(25.0f, 0, 0, 1); glRotatef(-90.0f, 1, 0, 0);
    gluCylinder(q, 0.08f, 0.06f, 2.0f, 10, 2); glPopMatrix();

    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.95f, 0.6f);
    glPushMatrix(); glTranslatef(-0.85f, h + 1.85f, 0);
    glutSolidSphere(0.4f, 16, 16); glPopMatrix();
    glEnable(GL_LIGHTING);

    gluDeleteQuadric(q);
    glPopMatrix();
}

void DrawStreetLamps()
{
    float h = 12.0f;

    DrawSingleMarineLamp(12.0f, 75.0f, h);
    DrawLightCone(12.0f, GetHeightAt(12.0f, 75.0f), 75.0f, h);
    DrawCartoonBench(12.0f, 70.0f, 270.0f);
    DrawCartoonBench(12.0f, 80.0f, 270.0f);

    DrawSingleMarineLamp(12.0f, 125.0f, h);
    DrawLightCone(12.0f, GetHeightAt(12.0f, 125.0f), 125.0f, h);
    DrawCartoonBench(12.0f, 120.0f, 270.0f);
    DrawCartoonBench(12.0f, 130.0f, 270.0f);

    DrawSingleMarineLamp(75.0f, -12.0f, h);
    DrawLightCone(75.0f, GetHeightAt(75.0f, -12.0f), -12.0f, h);
    DrawCartoonBench(70.0f, -12.0f, 0.0f);
    DrawCartoonBench(80.0f, -12.0f, 0.0f);

    DrawSingleMarineLamp(125.0f, -12.0f, h);
    DrawLightCone(125.0f, GetHeightAt(125.0f, -12.0f), -12.0f, h);
    DrawCartoonBench(120.0f, -12.0f, 0.0f);
    DrawCartoonBench(130.0f, -12.0f, 0.0f);

    DrawSingleMarineLamp(-75.0f, 12.0f, h);
    DrawLightCone(-75.0f, GetHeightAt(-75.0f, 12.0f), 12.0f, h);
    DrawCartoonBench(-70.0f, 12.0f, 180.0f);
    DrawCartoonBench(-80.0f, 12.0f, 180.0f);

    DrawSingleMarineLamp(-125.0f, 12.0f, h);
    DrawLightCone(-125.0f, GetHeightAt(-125.0f, 12.0f), 12.0f, h);
    DrawCartoonBench(-120.0f, 12.0f, 180.0f);
    DrawCartoonBench(-130.0f, 12.0f, 180.0f);
}

void DrawAllPlantsShadowGeom() {
    srand(42);
    for (int i = 0; i < 60; i++) {
        float rx = (rand() % 300) - 150.0f;
        float rz = (rand() % 300) - 150.0f;
        if (IsPointOnRoad(rx, rz)) continue;

        int type = rand() % 3;
        float sr = 0.8f, alpha = 0.28f;
        if (type == 0) { sr = 0.5f; alpha = 0.22f; }
        if (type == 1) { sr = 1.2f; alpha = 0.30f; }
        if (type == 2) { sr = 0.6f; alpha = 0.20f; }

       
        float y = GetHeightAt(rx, rz) + 0.25f;
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(0.08f, 0.08f, 0.08f, alpha);
        glVertex3f(rx, y, rz);
        for (int j = 0; j <= 16; j++) {
            float angle = 2.0f * 3.14159f * j / 16;
            float px = rx + cos(angle) * sr;
            float pz = rz + sin(angle) * sr;
            glVertex3f(px, GetHeightAt(px, pz) + 0.25f, pz);
        }
        glEnd();
    }
}
 bool CheckSpongebobCollision(float newX, float newZ)
{
    for (int i = 0; i < numBuildings; i++) {
        float dx = newX - buildings[i].x;
        float dz = newZ - buildings[i].z;
        float dist = sqrt(dx * dx + dz * dz);
        if (dist < buildings[i].radius)
            return true;
    }
    return false;
}