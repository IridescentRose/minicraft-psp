#pragma once

#include <string>
#include <oslib/VirtualFile.h>

class Logger
{
public:
    static void init();
    static void log(const std::string& msg);
    static void log(const std::string& msg, int number);

private:
    static VIRTUAL_FILE* logFile;
};
