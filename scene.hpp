#ifndef SCENE_HPP
#define SCENE_HPP

#include <math.h>
#include <EGL/egl.h>
#include <GLES/gl.h>

class Scene
{
  public:
  	void init(EGLint w, EGLint h);
  	void draw();

  private:
/**
 * Initialize an EGL context for the current display.
 */

	// GLUES

	static void __gluMakeIdentityf(GLfloat m[16])
	{
		m[0 + 4 * 0] = 1;
		m[0 + 4 * 1] = 0;
		m[0 + 4 * 2] = 0;
		m[0 + 4 * 3] = 0;
		m[1 + 4 * 0] = 0;
		m[1 + 4 * 1] = 1;
		m[1 + 4 * 2] = 0;
		m[1 + 4 * 3] = 0;
		m[2 + 4 * 0] = 0;
		m[2 + 4 * 1] = 0;
		m[2 + 4 * 2] = 1;
		m[2 + 4 * 3] = 0;
		m[3 + 4 * 0] = 0;
		m[3 + 4 * 1] = 0;
		m[3 + 4 * 2] = 0;
		m[3 + 4 * 3] = 1;
	}

	#define __glPi 3.14159265358979323846

	static void gluPerspectivef(GLfloat fovy, GLfloat aspect, GLfloat zNear,
						 GLfloat zFar)
	{
		GLfloat m[4][4];
		GLfloat sine, cotangent, deltaZ;
		GLfloat radians = (GLfloat) (fovy / 2.0f * __glPi / 180.0f);


		deltaZ = zFar - zNear;
		sine = (GLfloat) sin(radians);
		if ((deltaZ == 0.0f) || (sine == 0.0f) || (aspect == 0.0f))
		{
			return;
		}
		cotangent = (GLfloat) (cos(radians) / sine);

		__gluMakeIdentityf(&m[0][0]);
		m[0][0] = cotangent / aspect;
		m[1][1] = cotangent;
		m[2][2] = -(zFar + zNear) / deltaZ;
		m[2][3] = -1.0f;
		m[3][2] = -2.0f * zNear * zFar / deltaZ;
		m[3][3] = 0;
		glMultMatrixf(&m[0][0]);
	}
};

#endif