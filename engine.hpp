#ifndef ENGINE_H
#define ENGINE_H

#include <android_native_app_glue.h>
#include <math.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

#include "scene.hpp"

/**
 * Our saved state data.
 */
struct saved_state
{
  float angle;
  int32_t x;
  int32_t y;
};

/**
 * Shared state for our app.
 */
class Engine
{
  public:
    Engine()
    {
      scene = new Scene();
    }
    ~Engine()
    {
      delete scene;
    }
    int init_display();
    void draw_frame();
    void term_display();
    void init_app(struct android_app *state);

    // private:
    struct android_app *app;

    ASensorManager *sensorManager;
    const ASensor *accelerometerSensor;
    ASensorEventQueue *sensorEventQueue;

    int animating;
    //	int32_t width;
    //	int32_t height;
    struct saved_state state;

    Scene *scene;
};

#endif
