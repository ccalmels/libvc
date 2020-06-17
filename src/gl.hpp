#pragma once

#include <string>
#include <GLES3/gl3.h>


class shaders {
public:
	shaders();
	~shaders();

	bool init(const std::string &vertex_src,
		  const std::string &fragment_src);
	bool load(const std::string &vertex_file,
		  const std::string &fragment_file);

	void use() const;
	GLint location(const std::string &name) const;
private:
	shaders(const shaders &) = delete;
	shaders &operator=(const shaders &) = delete;

	GLuint prog_;
};
