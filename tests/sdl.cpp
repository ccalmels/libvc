#include <iostream>
#include "sdl.hpp"

int main(int argc, char* argv[])
{
	int w = 640;
	int h = 480;

	if (!sdl::init("test", w, h))
		return -1;

	std::cerr << "SDL window: " << sdl::version() << std::endl;

	sdl::fini();
	return 0;
}
