#include "skybox.h"

GLuint skyboxTex[6];

void DrawSkybox(float size)
{
    float s = size / 2.0f;

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDepthMask(GL_FALSE);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);

    // +X
    glBindTexture(GL_TEXTURE_2D, skyboxTex[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, s);
    glTexCoord2f(0, 1); glVertex3f(s, s, -s);
    glEnd();

    // -X
    glBindTexture(GL_TEXTURE_2D, skyboxTex[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, s);
    glTexCoord2f(1, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(-s, s, -s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, s);
    glEnd();

    // +Y
    glBindTexture(GL_TEXTURE_2D, skyboxTex[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-s, s, s);
    glTexCoord2f(1, 0); glVertex3f(s, s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, -s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, -s);
    glEnd();

    // -Y
    glBindTexture(GL_TEXTURE_2D, skyboxTex[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, -s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, -s);
    glTexCoord2f(1, 1); glVertex3f(s, -s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, -s, s);
    glEnd();

    // +Z
    glBindTexture(GL_TEXTURE_2D, skyboxTex[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-s, -s, s);
    glTexCoord2f(1, 0); glVertex3f(s, -s, s);
    glTexCoord2f(1, 1); glVertex3f(s, s, s);
    glTexCoord2f(0, 1); glVertex3f(-s, s, s);
    glEnd();

    // -Z
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