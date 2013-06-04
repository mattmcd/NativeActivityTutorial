#ifndef BOXES_SCENE_HPP
#define BOXES_SCENE_HPP

#include "scene.hpp"

class BoxesScene : public Scene  {
  public:
    virtual void init_scene() const;
    virtual void draw(EGLint x, EGLint y) const;
  protected:
    void draw_box() const;
};

class BoxesCircleScene : public BoxesScene  {
  public:
    virtual void draw(EGLint x, EGLint y) const;
};


#endif