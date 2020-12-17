#pragma once

#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

namespace gl {

class program {
public:
	program(const std::string &vertex_src,
		const std::string &fragment_src);
        ~program();

	program(program &&o);
	program &operator=(program &&o);

        void use() const;
        GLint location(const std::string &name) const;
	bool set(const std::string &name, int v0) const;
	bool set(const std::string &name, float v0) const;
	bool set(const std::string &name, float v0, float v1) const;
	bool set(const std::string &name, float v0, float v1, float v2) const;
private:
	program(const program&) = delete;
	program &operator=(const program &) = delete;

        GLuint id;
};

class texture {
public:
	texture(GLenum format, int w, int h, const void *data = nullptr);
	~texture();

	texture(texture &&o);
	texture &operator=(texture &&o);

	void update(const void *data);
	void active(int n = 0);
private:
	texture(const texture &) = delete;
	texture &operator=(const texture &) = delete;
public:
	int w, h;
	GLenum format;
	GLuint id;
};

class framebuffer {
public:
	framebuffer(int w, int h);
	~framebuffer();

	framebuffer(framebuffer &&o);
	framebuffer &operator=(framebuffer &&o);

	void bind();

	static framebuffer &get_default();
private:
	framebuffer();
	framebuffer(const framebuffer &) = delete;
	framebuffer &operator=(const framebuffer &) = delete;

	GLuint id, tex;
};

std::string version();
bool has_extension(const std::string &name);

}
