#include <cassert>
#include <iostream>

#include "gl.hpp"

static void print_shader_log(GLuint shader)
{
	int len = 0;
	char *buffer;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

	buffer = new char[len];

	glGetShaderInfoLog(shader, len, &len, buffer);

	std::cerr << "shader logs: " << std::endl << buffer;
	delete [] buffer;
}

static bool compile_shader(GLuint shader, const std::string &source)
{
	GLint status;
	char *src = (char*)source.c_str();

	glShaderSource(shader, 1, &src, 0);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		std::cerr << "glCompileShader: " << std::endl
			  << src << std::endl;
		print_shader_log(shader);
		return false;
	}

	return true;
}

static GLuint attach_shader(GLuint program, GLuint type, const std::string &src)
{
	GLuint shader;

	shader = glCreateShader(type);
	if (!shader) {
		std::cerr << "glCreateShader: " << glGetError() << std::endl;
		return 0;
	}

	if (!compile_shader(shader, src)) {
		glDeleteShader(shader);
		return 0;
	}

	glAttachShader(program, shader);
	return shader;
}

static void clean_shader(GLuint program, GLuint shader)
{
	glDetachShader(program, shader);
	glDeleteShader(shader);
}

static void print_program_log(GLuint prog)
{
	int len = 0;
	char *buffer;

	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);

	buffer = new char[len];

	glGetProgramInfoLog(prog, len, &len, buffer);

	std::cerr << "program logs: " << std::endl << buffer;
	delete [] buffer;
}

static bool link(GLuint program)
{
	GLint status;

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		std::cerr << "glLinkProgram: " << std::endl;
		print_program_log(program);
		return false;
	}

	return true;
}

GLuint create_program(const std::string &vertex_src,
		      const std::string &fragment_src)
{
	GLuint vertex, fragment, id;
	bool ret = false;

	id = glCreateProgram();
	if (!id) {
		std::cerr << "glCreateProgram: " << glGetError() << std::endl;
		return 0;
	}

	vertex = attach_shader(id, GL_VERTEX_SHADER, vertex_src);
	if (!vertex)
		goto delete_program;

	fragment = attach_shader(id, GL_FRAGMENT_SHADER, fragment_src);
	if (!fragment)
		goto clean_vertex;

	ret = link(id);

	clean_shader(id, fragment);
clean_vertex:
	clean_shader(id, vertex);
	if (ret)
		return id;
delete_program:
	glDeleteProgram(id);
	return 0;
}

static GLuint create_texture(GLenum format, int w, int h, const void *data)
{
	GLuint id;

	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h , 0, format,
		     GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
	return id;
}

namespace gl {

program::program(const std::string &vertex_src, const std::string &fragment_src)
	: id(create_program(vertex_src, fragment_src))
{
	assert(id);
}

program::~program()
{
	glDeleteProgram(id);
}

program::program(program &&o) : id(o.id)
{
	o.id = 0;
}

program &program::operator=(program &&o)
{
	if (id != o.id) {
		glDeleteProgram(id);

		id = o.id;
		o.id = 0;
	}
	return *this;
}

void program::use() const
{
	glUseProgram(id);
}

GLint program::location(const std::string &name) const
{
	GLint ret;

	ret = glGetUniformLocation(id, name.c_str());
	if (ret == -1)
		ret = glGetAttribLocation(id, name.c_str());

	return ret;
}

bool program::set(const std::string &name, int v0) const
{
	GLint loc;

	loc = glGetUniformLocation(id, name.c_str());
	if (loc == -1)
		return false;

	glUniform1i(loc, v0);
	return true;
}

bool program::set(const std::string &name, float v0) const
{
	GLint loc;

	loc = glGetUniformLocation(id, name.c_str());
	if (loc == -1)
		return false;

	glUniform1f(loc, v0);
	return true;
}

bool program::set(const std::string &name, float v0, float v1) const
{
	GLint loc;

	loc = glGetUniformLocation(id, name.c_str());
	if (loc == -1)
		return false;

	glUniform2f(loc, v0, v1);
	return true;
}

bool program::set(const std::string &name, float v0, float v1, float v2) const
{
	GLint loc;

	loc = glGetUniformLocation(id, name.c_str());
	if (loc == -1)
		return false;

	glUniform3f(loc, v0, v1, v2);
	return true;
}

texture::texture(GLenum format, int w, int h, const void *data)
	: w(w), h(h), format(format), id(create_texture(format, w, h, data)) {}

texture::~texture()
{
	glDeleteTextures(1, &id);
}

texture::texture(texture &&o) : w(o.w), h(o.h), format(o.format), id(o.id)
{
	o.id = 0;
}

texture &texture::operator=(texture &&o)
{
	if (id != o.id) {
		glDeleteTextures(1, &id);

		w = o.w;
		h = o.h;
		format = o.format;
		id = o.id;

		o.id = 0;
	}
	return *this;
}

void texture::update(const void *data)
{
	glBindTexture(GL_TEXTURE_2D, id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, format,
			GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::active(int n)
{
	glActiveTexture(GL_TEXTURE0 + n);
	glBindTexture(GL_TEXTURE_2D, id);
}

framebuffer::framebuffer(int w, int h)
{
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	tex = create_texture(GL_RGBA, w, h, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			       GL_TEXTURE_2D, tex, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

framebuffer::framebuffer() : id(0), tex(0) {}

framebuffer::~framebuffer()
{
	glDeleteFramebuffers(1, &id);
	glDeleteTextures(1, &tex);
}

framebuffer::framebuffer(framebuffer &&o) : id(o.id), tex(o.tex)
{
	o.id = 0;
	o.tex = 0;
}

framebuffer &framebuffer::operator=(framebuffer &&o)
{
	if (id != o.id) {
		glDeleteFramebuffers(1, &id);
		glDeleteTextures(1, &tex);

		id = o.id;
		tex = o.tex;

		o.id = 0;
		o.tex = 0;
	}
	return *this;
}

void framebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER)
	       == GL_FRAMEBUFFER_COMPLETE);
}

framebuffer &framebuffer::get_default()
{
	static framebuffer def;
	return def;
}

std::string version()
{
	return (char*)glGetString(GL_VERSION);
}

}
