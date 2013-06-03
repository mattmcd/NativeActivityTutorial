#include "engine.hpp"

int Engine::init_display()
{
  scene->init_display();

  ANativeWindow_setBuffersGeometry(this->app->window, 0, 0, scene->format);

  EGLSurface surface = eglCreateWindowSurface(scene->display, scene->config, this->app->window, NULL);

  scene->surface = surface;

  if ( scene->init_context() < 0 ) {
    LOGW("Unable to eglMakeCurrent");
  }

  this->state.angle = 0;

  return 0;
}

/**
 * Just the current frame in the display.
 */

void Engine::draw_frame()
{
  scene->draw( this->state.x, this->state.y );
}

/**
 * Tear down the EGL context currently associated with the display.
 */

void Engine::term_display()
{
  scene->terminate();
  this->animating = 0;
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
