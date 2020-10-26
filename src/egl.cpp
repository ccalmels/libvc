#include <iostream>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

static bool initialize(EGLDisplay display, EGLint *config_attr,
		       EGLNativeWindowType window)
{
	EGLConfig config;
	EGLint configs_count;
	EGLContext context;
	EGLSurface surface;
	EGLint egl_context_attr[] = {
		EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
		EGL_CONTEXT_MAJOR_VERSION, 4,
		EGL_CONTEXT_MINOR_VERSION, 5,
		EGL_NONE
	};

	if (eglInitialize(display, nullptr, nullptr) == EGL_FALSE)
		return false;

	if (eglChooseConfig(display, config_attr, &config, 1, &configs_count)
	    == EGL_FALSE)
		return false;

	if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
		return false;

	context = eglCreateContext(display, config, EGL_NO_CONTEXT, egl_context_attr);
	if (context == EGL_NO_CONTEXT)
		return false;

	if (window) {
		surface = eglCreateWindowSurface(display, config, window, nullptr);
		if (surface == EGL_NO_SURFACE)
			return false;
	} else
		surface = EGL_NO_SURFACE;

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
		return false;

	return true;
}

namespace egl {

bool init(int gpu)
{
#define MAX_DEVICES 4
	EGLDeviceEXT devs[MAX_DEVICES];
	EGLint devices_count;
	EGLDisplay display;
	EGLint egl_config_attr[] = {
		EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_NONE
	};

	std::string exts = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
	if (exts.find("EGL_EXT_device_base") == std::string::npos
	    || exts.find("EGL_EXT_platform_base") == std::string::npos) {
		std::cerr << "extensions not presents" << std::endl;
		return false;
	}

	auto eglQueryDevicesEXT =
		(PFNEGLQUERYDEVICESEXTPROC)
		eglGetProcAddress("eglQueryDevicesEXT");

	auto eglGetPlatformDisplayEXT =
		(PFNEGLGETPLATFORMDISPLAYEXTPROC)
		eglGetProcAddress("eglGetPlatformDisplayEXT");

	if (eglQueryDevicesEXT(MAX_DEVICES, devs, &devices_count) == EGL_FALSE) {
		std::cerr << "no devices detected" << std::endl;
		return false;
	}

	if (gpu >= devices_count) {
		std::cerr << "ask for device " << gpu << ", but only "
			  << devices_count << " found" << std::endl;
		return false;
	}

	display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, devs[gpu], 0);
	if (display == EGL_NO_DISPLAY)
		return false;

	if (!initialize(display, egl_config_attr, 0))
	    return false;

	return true;
}

bool init(const std::string &name, int &width, int &height, int flags)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDL_Init: " << SDL_GetError() << std::endl;
		return false;
	}

	auto window = SDL_CreateWindow(
		name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_OPENGL | flags);
	if (!window) {
		std::cerr << "SDL_CreateWindow: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_GetWindowSize(window, &width, &height);

	SDL_SysWMinfo wm;
	SDL_VERSION(&wm.version);
	SDL_GetWindowWMInfo(window, &wm);
	if (wm.subsystem != SDL_SYSWM_X11) {
		std::cerr << "not running on X11" << std::endl;
		return false;
	}

	EGLDisplay  display;
	EGLint egl_config_attr[] = {
		EGL_BUFFER_SIZE,    1,
		EGL_DEPTH_SIZE,     1,
		EGL_STENCIL_SIZE,   0,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_NONE
	};

	display = eglGetDisplay(wm.info.x11.display);
	if (display == EGL_NO_DISPLAY)
		return false;

	if (!initialize(display, egl_config_attr,
			(EGLNativeWindowType)wm.info.x11.window))
		return false;

	eglSwapInterval(display, 1);
	return true;
}

void fini()
{
	eglTerminate(eglGetCurrentDisplay());
}

std::string version()
{
	return eglQueryString(eglGetCurrentDisplay(), EGL_VERSION);
}

}
