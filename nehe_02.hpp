#ifndef NEHE_02_HPP
#define NEHE_02_HPP

#include "scene.hpp"

class NEHE_02 : public Scene {
  public:
    virtual void init_scene() const;
    virtual void draw( EGLint x, EGLint y) const;
};

#endif
