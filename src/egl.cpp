#include <iostream>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

static const EGLint configAttribs[] = {
	EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
	EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
	EGL_NONE
};

namespace egl {

bool init(int gpu)
{
#define MAX_DEVICES 4
	EGLDeviceEXT devs[MAX_DEVICES];
	EGLint devices_count, configs_count;
	EGLDisplay dpy;
	EGLConfig config;
	EGLContext context;

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
		std::cerr << "ask for device " << gpu << ", but only " << devices_count << " found" << std::endl;
		return false;
	}

	dpy = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, devs[gpu], 0);
	if (dpy == EGL_NO_DISPLAY)
		return false;

	if (eglInitialize(dpy, nullptr, nullptr) == EGL_FALSE)
		return false;

	if (eglChooseConfig(dpy, configAttribs, &config, 1, &configs_count) == EGL_FALSE)
		return false;

	if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
		return false;

	context = eglCreateContext(dpy, config, EGL_NO_CONTEXT, nullptr);
	if (context == EGL_NO_CONTEXT)
		return false;

	if (eglMakeCurrent(dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, context) == EGL_FALSE)
		return false;

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
