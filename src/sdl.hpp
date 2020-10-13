#pragma once

#include <string>

namespace sdl {

bool init(const std::string &name, int &width, int &height, int flags = 0);
void fini();

std::string version();

}
