#pragma once

#include <string>

namespace egl {

bool init(int gpu = 0);
void fini();

std::string version();

};
