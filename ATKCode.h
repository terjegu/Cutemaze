#ifndef ATKCODE_H
#define ATKCODE_H

std::vector<std::string> recognize();
int inithtk(int argc, char *argv[], const char * app_version, bool noGraphics=FALSE); // Changed noGraphics from Boolean to bool

#endif