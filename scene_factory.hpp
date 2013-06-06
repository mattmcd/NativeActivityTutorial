#ifndef SCENE_FACTORY_HPP
#define SCENE_FACTORY_HPP

#include "scene.hpp"
#include <string>
#include <map>

class SceneFactory {
  typedef Scene* (*CreateSceneFcn) (void);

  public:
    Scene* getScene( std::string name );
    static SceneFactory& getInstance();
    void registerScene( std::string name, CreateSceneFcn) ;
    ~SceneFactory(){};
  private:
    std::map<std::string, CreateSceneFcn> SceneCreatorFcns;
    SceneFactory(){};
};

#endif
