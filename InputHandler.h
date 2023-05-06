#pragma once

#include <oslib/oslib.h>
#include <vector>

#include "InputHandler_Key.h"

class InputHandler
{
public:
	InputHandler();
	~InputHandler();
    std::vector<InputHandler_Key*> keys;
    InputHandler_Key* up;
    InputHandler_Key* down;
    InputHandler_Key* left;
    InputHandler_Key* right;
    InputHandler_Key* attack;
    InputHandler_Key* menu;
    InputHandler_Key* exit;
    InputHandler_Key* pause;
    void releaseAll();
    void tick();
    void add(InputHandler_Key*);
    void handleKeys(OSL_CONTROLLER*);

private:
    void init();


public:
    friend class InputHandler_Key;
};
