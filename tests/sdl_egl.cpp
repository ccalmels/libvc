#include <iostream>
#include "egl.hpp"
#include "gl.hpp"

int main(int argc, char *argv[])
{
	int w = 800, h = 600;

	if (!egl::init("SDL", w, h))
		return -1;

	std::cerr << "EGL version: " << egl::version() << std::endl;
	std::cerr << "GL version : " << gl::version() << std::endl;

	egl::fini();
	return 0;
}
