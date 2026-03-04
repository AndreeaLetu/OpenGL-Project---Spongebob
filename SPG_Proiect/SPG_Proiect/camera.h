#pragma once
#pragma once

void Keyboard(unsigned char key, int x, int y);
void MouseClick(int button, int state, int x, int y);
void MouseMotion(int x, int y);

extern float camPosX, camPosY, camPosZ;
extern float yaw, pitch;