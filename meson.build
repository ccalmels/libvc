project('libvc', 'cpp', default_options : [ 'cpp_std=c++14' ])

srcs = [ 'src/gl.cpp', 'src/egl.cpp' ]
deps = [ dependency('gl'), dependency('egl') ]

sdl2dep = dependency('sdl2', required : false)

if sdl2dep.found()
  srcs += [ 'src/sdl.cpp' ]
  deps += [ sdl2dep, dependency('wayland-egl') ]
  add_project_arguments('-DHAS_SDL2', language : 'cpp')
endif

lib = library('vc', sources : srcs,
	      dependencies : deps, install : true)

libvc_dep = declare_dependency(dependencies : deps,
			       include_directories : include_directories('src'),
			       link_with : lib)

install_headers([ 'src/sdl.hpp', 'src/egl.hpp', 'src/gl.hpp' ], subdir : 'vc')

import('pkgconfig').generate(name : meson.project_name(),
			     description : 'Simple C++ API for SDL/EGL/Opengl',
			     version : meson.project_version(),
			     libraries : lib)

if sdl2dep.found()
  sdl_test = executable('sdl_test', 'tests/sdl.cpp',
                      dependencies : libvc_dep)
  test('sdl test', sdl_test)

  sdl_egl_test = executable('sdl_egl_test', 'tests/sdl_egl.cpp',
			    dependencies : libvc_dep)
  test('sdl(egl) test', sdl_egl_test)

  gl_test = executable('gl_test', 'tests/gl.cpp',
		       dependencies : libvc_dep)
  test('gl test', gl_test)
endif

egl_test = executable('egl_test', 'tests/egl.cpp',
                      dependencies : libvc_dep)
test('egl test', egl_test)
