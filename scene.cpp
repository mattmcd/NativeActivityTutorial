#include "scene.hpp"

void Scene::init_display()
{
  // initialize OpenGL ES and EGL

  /* 
   * Here specify the attributes of the desired configuration.
   * Below, we select an EGLConfig with at least 8 bits per color
   * component compatible with on-screen windows
   */
  const EGLint attribs[] = {
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_DEPTH_SIZE, 8,
    EGL_NONE
  };
  EGLint dummy;
  EGLint numConfigs;
  EGLConfig config;
  EGLSurface surface;
  EGLContext context;

  EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

  eglInitialize(display, 0, 0);

  /* Here, the application chooses the configuration it desires. In this
     sample, we have a very simplified selection process, where we pick the
     first EGLConfig that matches our criteria */
  eglChooseConfig(display, attribs, &config, 1, &numConfigs);

  /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     guaranteed to be accepted by ANativeWindow_setBuffersGeometry(). As
     soon as we picked a EGLConfig, we can safely reconfigure the
     ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
  eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
  this->display = display;
  this->config = config;
}


int Scene::init_context()
{
  context = eglCreateContext(display, config, NULL, NULL);

  if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
  {
    return -1;
  }

  eglQuerySurface(display, surface, EGL_WIDTH, &w);
  eglQuerySurface(display, surface, EGL_HEIGHT, &h);

  this->context = context;

  // Initialize GL state.
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepthf(1.0f);

  this->init_scene();
  return 0;
}

void Scene::terminate()
{
  if (this->display != EGL_NO_DISPLAY)
  {
    eglMakeCurrent(this->display, EGL_NO_SURFACE, EGL_NO_SURFACE,
        EGL_NO_CONTEXT);
    if (this->context != EGL_NO_CONTEXT)
    {
      eglDestroyContext(this->display, this->context);
    }
    if (this->surface != EGL_NO_SURFACE)
    {
      eglDestroySurface(this->display, this->surface);
    }
    eglTerminate(this->display);
  }
  this->display = EGL_NO_DISPLAY;
  this->context = EGL_NO_CONTEXT;
  this->surface = EGL_NO_SURFACE;
}
