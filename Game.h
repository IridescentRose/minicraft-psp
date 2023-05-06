#pragma once

#include <oslib/oslib.h>

#include "Screen.h"
#include "InputHandler.h"
#include "entity/Player.h"
#include "level/Level.h"

typedef unsigned short ushort;

class Menu;

class Game {
public:
Game();
virtual ~Game();
void run();
void init();
void render();
void tick();
void setMenu(Menu*);
void scheduleLevelChange(int dir);
void changeLevel(int dir);
void won();
void resetGame();
bool running;
Player * player;
int gameTime;
private:
OSL_IMAGE * image;
OSL_IMAGE * renderImage;
OSL_IMAGE * spriteSheet;
Screen * screen;
Screen * lightScreen;
InputHandler * input;
Level * level;
Menu * menu;
Menu * previousMenu;
ushort * colors;
ushort * pixels;
int currentLevel;
Level ** levels;
int pendingLevelChange;
int playerDeadTime;
bool hasWon;
unsigned int frameskip;
int frames;
int ticks;
int wonTimer;
void renderGui();
};