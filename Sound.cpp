#include "Sound.h"

Sound* Sound::playerHurt;
Sound* Sound::playerDeath;
Sound* Sound::monsterHurt;
Sound* Sound::test;
Sound* Sound::pickup;
Sound* Sound::bossdeath;
Sound* Sound::craft;

Sound::Sound(const char* name)
{
    clip = oslLoadSoundFileWAV(name, OSL_FMT_NONE);
}

Sound::~Sound()
{
	oslDeleteSound(clip);
	clip = nullptr;
}

void Sound::initSounds()
{
    playerHurt = new Sound("sounds/playerhurt.wav");
    playerDeath = new Sound("sounds/death.wav");
    monsterHurt = new Sound("sounds/monsterhurt.wav");
    test = new Sound("sounds/test.wav");
    pickup = new Sound("/sounds/pickup.wav");
    bossdeath = new Sound("sounds/bossdeath.wav");
    craft = new Sound("sounds/craft.wav");
}

void Sound::play()
{
    oslPlaySound(clip, 0);
}
