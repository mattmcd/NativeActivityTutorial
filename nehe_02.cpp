#include "scene_factory.hpp"
#include "nehe_02.hpp"

namespace 
{
  SceneRegister nehe02Reg = SceneRegister( "NEHE02", []() { return new NEHE_02(); });
}

void NEHE_02::init_scene() const {
  GLfloat triangle[] = {
     0.0f, 1.0f, 0.0f,              // Top
    -1.0f,-1.0f, 0.0f,              // Bottom Left
     1.0f,-1.0f, 0.0f };              // Bottom Right
   
  glVertexPointer(3, GL_FLOAT, 0, triangle);
  glEnableClientState(GL_VERTEX_ARRAY);
  glShadeModel(GL_FLAT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glViewport(0, 0, w, h);
  gluPerspectivef(45.0f, (1.0f * w) / h, 1.0f, 100.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void NEHE_02::draw( EGLint x, EGLint y) const {
  if (this->display == NULL)
  {
    // No display.
    return;
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
   glTranslatef( 0.0f, 0.0f,   -10.0f);
   glRotatef(y, 1.0f, 0.0f, 0.0f);
      glRotatef(-x, 0.0f, 1.0f, 0.0f);
      
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glFlush();

  eglSwapBuffers(this->display, this->surface);
}