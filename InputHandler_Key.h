#pragma once

class InputHandler;

class InputHandler_Key
{
public:
	InputHandler_Key(InputHandler* inputHandler);
	~InputHandler_Key();
    int presses, absorbs;
    bool down, clicked;

    void toggle(bool pressed);
    void tick();

private:
    friend class InputHandler;
};
