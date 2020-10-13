#include <iostream>
#include "egl.hpp"

int main(int argc, char *argv[])
{
	if (!egl::init())
		return -1;

	std::cerr << "EGL version: " << egl::version() << std::endl;

	egl::fini();
	return 0;
}
