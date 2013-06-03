#include "scene.hpp"

GLfloat box[] = {
  // FRONT
  -0.5f, -0.5f, 0.5f,
  0.5f, -0.5f, 0.5f,
  -0.5f, 0.5f, 0.5f,
  0.5f, 0.5f, 0.5f,
  // BACK
  -0.5f, -0.5f, -0.5f,
  -0.5f, 0.5f, -0.5f,
  0.5f, -0.5f, -0.5f,
  0.5f, 0.5f, -0.5f,
  // LEFT
  -0.5f, -0.5f, 0.5f,
  -0.5f, 0.5f, 0.5f,
  -0.5f, -0.5f, -0.5f,
  -0.5f, 0.5f, -0.5f,
  // RIGHT
  0.5f, -0.5f, -0.5f,
  0.5f, 0.5f, -0.5f,
  0.5f, -0.5f, 0.5f,
  0.5f, 0.5f, 0.5f,
  // TOP
  -0.5f, 0.5f, 0.5f,
  0.5f, 0.5f, 0.5f,
  -0.5f, 0.5f, -0.5f,
  0.5f, 0.5f, -0.5f,
  // BOTTOM
  -0.5f, -0.5f, 0.5f,
  -0.5f, -0.5f, -0.5f,
  0.5f, -0.5f, 0.5f,
  0.5f, -0.5f, -0.5f,
};

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

void Scene::init_scene()
{
  glVertexPointer(3, GL_FLOAT, 0, box);
  glEnableClientState(GL_VERTEX_ARRAY);
  glShadeModel(GL_FLAT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glViewport(0, 0, w, h);
  gluPerspectivef(45.0f, (1.0f * w) / h, 1.0f, 100.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Scene::draw(EGLint x, EGLint y)
{
  if (this->display == NULL)
  {
    // No display.
    return;
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  int nRows = 10;
  int nCols = 15;
  float a = sqrt(3);

  for (int i = 0; i < nRows; ++i)
  {
    for (int j = 0; j < nCols; ++j)
    {
      glLoadIdentity();
      glTranslatef(a * j - a * nCols * 0.5, a * i - a * nRows * 0.5,
          -16);
      glRotatef(y, 1.0f, 0.0f, 0.0f);
      glRotatef(-x, 0.0f, 1.0f, 0.0f);
      this->draw_box();
    }
  }

  glFlush();

  eglSwapBuffers(this->display, this->surface);
}

void Scene::draw_box()
{
  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
  glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
  glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
  glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
  glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
  glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
  glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
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
