#pragma once

#include <string>

namespace egl {

bool init(int gpu = 0);
bool init(const std::string &name, int &width, int &height, int flags = 0);
void fini();

std::string version();

};
