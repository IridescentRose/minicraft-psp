#include "Logger.h"

VIRTUAL_FILE* Logger::logFile = nullptr;

void Logger::init()
{
    logFile = VirtualFileOpen((void*)("host0:/log.txt"), 0, VF_FILE, VF_O_WRITE);
}

void Logger::log(const std::string& msg)
{
    VirtualFilePuts((msg + "\n").c_str(), logFile);
}

void Logger::log(const std::string& msg, int number)
{
    char str[255];
    sprintf(str, "%s %d\n", msg.c_str(), number);
    VirtualFilePuts(str, logFile);
}