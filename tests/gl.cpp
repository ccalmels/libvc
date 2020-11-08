#include "sdl.hpp"
#include "gl.hpp"

static const std::string vertex = R""(
#version 400

        void main() {
        }
	)"";

static const std::string fragment = R""(
#version 400

	void main() {
}
	)"";

int main(int argc, char *argv[])
{
	int w = 640;
	int h = 480;

	if (!sdl::init("test", w, h))
		return -1;

	gl::program p(vertex, fragment);

	p.use();

	sdl::fini();
	return 0;
}
