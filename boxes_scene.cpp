#include "boxes_scene.hpp"

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

void BoxesScene::init_scene() const
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

void BoxesScene::draw(EGLint x, EGLint y) const
{
  if (this->display == NULL)
  {
    // No display.
    return;
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  int nRows = 4;
  int nCols = 4;
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

void BoxesScene::draw_box() const
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

void BoxesCircleScene::draw(EGLint x, EGLint y) const
{
  if (this->display == NULL)
  {
    // No display.
    return;
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  int n =8;
  float r = sqrt(3)*3;
  float pi = 4.0*atan(1);

  for (int i = 0; i < n; ++i)
  {
    glLoadIdentity();
    glTranslatef(r*cos( (2*pi*i)/n), r*sin((2*pi*i)/n),   -16);
    glRotatef(y, 1.0f, 0.0f, 0.0f);
    glRotatef(-x, 0.0f, 1.0f, 0.0f);
    this->draw_box();
  }

  glFlush();

  eglSwapBuffers(this->display, this->surface);
}