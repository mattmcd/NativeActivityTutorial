#ifndef SCENE_FACTORY_HPP
#define SCENE_FACTORY_HPP

#include "scene.hpp"
#include <string>
#include <map>
#include <functional>

class SceneFactory {
  public:
    Scene* getScene( std::string name );
    static SceneFactory& getInstance();
    void registerScene( std::string name, std::function<Scene* ()> fcn) ;
    ~SceneFactory(){};
  private:
    std::map<std::string, std::function<Scene* ()>> SceneCreatorFcns;
    SceneFactory(){};
};

class SceneRegister {
  // Helper class to allow registration of scenes by construction of global
  // variables in a hidden namespace.  
  // See p164 of Joshi "C++ design patterns for derivatives pricing"
  // Note that the C++11 language features make this pattern a lot simpler.

  public:
    SceneRegister( std::string name, std::function<Scene* ()> fcn ) {
      SceneFactory& theFactory = SceneFactory::getInstance();
      theFactory.registerScene( name, fcn );
    }
};

#endif