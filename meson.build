project('libvc', 'cpp', default_options: [ 'cpp_std=c++14' ])

deps = [ dependency('sdl2'),
	 dependency('glesv2') ]

lib = library('vc',
	      sources: [ 'src/sdl.cpp',
			 'src/gl.cpp' ],
	      dependencies: deps, install: true)

libvc_dep = declare_dependency(dependencies: deps,
			       include_directories : include_directories('src'),
			       link_with : lib)

install_headers([ 'src/sdl.hpp', 'src/gl.hpp' ], subdir: 'vc')

import('pkgconfig').generate(name: meson.project_name(),
			     description: 'Simple C++ API for SDL/Opengl',
			     version: meson.project_version(),
			     libraries: lib)
