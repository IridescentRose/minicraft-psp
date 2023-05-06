#pragma once

#include "oslib/oslib.h"

class Sound {
public:
    Sound(const char* name);
    virtual ~Sound();
    static void initSounds();
    void play();

    static Sound* playerHurt;
    static Sound* playerDeath;
    static Sound* monsterHurt;
    static Sound* test;
    static Sound* pickup;
    static Sound* bossdeath;
    static Sound* craft;

private:
    OSL_SOUND* clip;
};
