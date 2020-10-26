#include <iostream>
#include "egl.hpp"
#include "gl.hpp"

int main(int argc, char *argv[])
{
	if (!egl::init())
		return -1;

	std::cerr << "EGL version   : " << egl::version() << std::endl;
	std::cerr << "OpenGL verison: " << gl::version() << std::endl;

	egl::fini();
	return 0;
}
