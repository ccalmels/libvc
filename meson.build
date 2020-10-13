project('libvc', 'cpp', default_options: [ 'cpp_std=c++14' ])

deps = [ dependency('sdl2'),
	 dependency('egl'),
	 dependency('gl') ]

lib = library('vc',
	      sources: [ 'src/sdl.cpp',
			 'src/egl.cpp',
			 'src/gl.cpp' ],
	      dependencies: deps, install: true)

libvc_dep = declare_dependency(dependencies: deps,
			       include_directories : include_directories('src'),
			       link_with : lib)

install_headers([ 'src/sdl.hpp', 'src/egl.hpp', 'src/gl.hpp' ], subdir: 'vc')

import('pkgconfig').generate(name: meson.project_name(),
			     description: 'Simple C++ API for SDL/EGL/Opengl',
			     version: meson.project_version(),
			     libraries: lib)

sdl_test = executable('sdl_test', 'tests/sdl.cpp',
                      dependencies: libvc_dep)
test('sdl test', sdl_test)

egl_test = executable('egl_test', 'tests/egl.cpp',
                      dependencies: libvc_dep)
test('egl test', egl_test)

program_test = executable('program_test', 'tests/program.cpp',
			  dependencies: libvc_dep)
test('gl::program test', program_test)
