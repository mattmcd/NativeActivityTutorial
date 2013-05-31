#include "engine.hpp"

/**
 * Initialize an EGL context for the current display.
 */

//GLUES

static void __gluMakeIdentityf(GLfloat m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

#define __glPi 3.14159265358979323846

void gluPerspectivef(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    GLfloat m[4][4];
    GLfloat sine, cotangent, deltaZ;
    GLfloat radians=(GLfloat)(fovy/2.0f*__glPi/180.0f);


    deltaZ=zFar-zNear;
    sine=(GLfloat)sin(radians);
    if ((deltaZ==0.0f) || (sine==0.0f) || (aspect==0.0f))
    {
        return;
    }
    cotangent=(GLfloat)(cos(radians)/sine);

    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = cotangent / aspect;
    m[1][1] = cotangent;
    m[2][2] = -(zFar + zNear) / deltaZ;
    m[2][3] = -1.0f;
    m[3][2] = -2.0f * zNear * zFar / deltaZ;
    m[3][3] = 0;
    glMultMatrixf(&m[0][0]);
}
 
 
GLfloat box[] = {
	// FRONT
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	// BACK
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	// LEFT
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	// RIGHT
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	// TOP
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 -0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	// BOTTOM
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f, -0.5f,
};    


int Engine::init_display()
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
	EGLint w, h, dummy, format;
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

	ANativeWindow_setBuffersGeometry(this->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, this->app->window, NULL);
	context = eglCreateContext(display, config, NULL, NULL);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
	{
		LOGW("Unable to eglMakeCurrent");
		return -1;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	this->display = display;
	this->context = context;
	this->surface = surface;
	this->width = w;
	this->height = h;
	this->state.angle = 0;

	// Initialize GL state.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepthf(1.0f);
	glVertexPointer(3, GL_FLOAT, 0, box);
	glEnableClientState(GL_VERTEX_ARRAY);
	glShadeModel(GL_FLAT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);
	gluPerspectivef(45.0f, (1.0f * w) / h, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return 0;
}

/**
 * Just the current frame in the display.
 */

void Engine::draw_frame()
{
	if (this->display == NULL)
	{
		// No display.
		return;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, -1, -6);
	glRotatef(this->state.y, 1.0f, 0.0f, 0.0f);
	glRotatef(-this->state.x, 0.0f, 1.0f, 0.0f);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
	glFlush();
	eglSwapBuffers(this->display, this->surface);
}

/**
 * Tear down the EGL context currently associated with the display.
 */

void Engine::term_display()
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
	this->animating = 0;
	this->display = EGL_NO_DISPLAY;
	this->context = EGL_NO_CONTEXT;
	this->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app *app,
								   AInputEvent * event)
{
	// struct engine* engine = (struct engine*)app->userData;
	Engine *engine = static_cast < Engine * >(app->userData);
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		engine->animating = 1;
		engine->state.x = AMotionEvent_getX(event, 0);
		engine->state.y = AMotionEvent_getY(event, 0);
		return 1;
	}
	return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app *app, int32_t cmd)
{
	Engine *engine = static_cast < Engine * >(app->userData);
	switch (cmd)
	{
	case APP_CMD_SAVE_STATE:
		// The system has asked us to save our current state.  Do so.
		engine->app->savedState = malloc(sizeof(struct saved_state));
		*((struct saved_state *)engine->app->savedState) = engine->state;
		engine->app->savedStateSize = sizeof(struct saved_state);
		break;
	case APP_CMD_INIT_WINDOW:
		// The window is being shown, get it ready.
		if (engine->app->window != NULL)
		{
			engine->init_display();
			engine->draw_frame();
		}
		break;
	case APP_CMD_TERM_WINDOW:
		// The window is being hidden or closed, clean it up.
		engine->term_display();
		break;
	case APP_CMD_GAINED_FOCUS:
		// When our app gains focus, we start monitoring the accelerometer.
		if (engine->accelerometerSensor != NULL)
		{
			ASensorEventQueue_enableSensor(engine->sensorEventQueue,
										   engine->accelerometerSensor);
			// We'd like to get 60 events per second (in us).
			ASensorEventQueue_setEventRate(engine->sensorEventQueue,
										   engine->accelerometerSensor,
										   (1000L / 60) * 1000);
		}
		break;
	case APP_CMD_LOST_FOCUS:
		// When our app loses focus, we stop monitoring the accelerometer.
		// this->is to avoid consuming battery while not being used.
		if (engine->accelerometerSensor != NULL)
		{
			ASensorEventQueue_disableSensor(engine->sensorEventQueue,
											engine->accelerometerSensor);
		}
		// Also stop animating.
		engine->animating = 0;
		engine->draw_frame();
		break;
	}
}

/* Initialise engine */
void Engine::init_app(struct android_app *state)
{

	state->userData = this;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;
	this->app = state;

	// Prepare to monitor accelerometer
	this->sensorManager = ASensorManager_getInstance();
	this->accelerometerSensor =
		ASensorManager_getDefaultSensor(this->sensorManager,
										ASENSOR_TYPE_ACCELEROMETER);
	this->sensorEventQueue =
		ASensorManager_createEventQueue(this->sensorManager, state->looper,
										LOOPER_ID_USER, NULL, NULL);

	if (state->savedState != NULL)
	{
		// We are starting with a previous saved state; restore from it.
		this->state = *(struct saved_state *)state->savedState;
	}
}