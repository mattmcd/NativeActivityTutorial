#include "scene_factory.hpp"

void SceneFactory::registerScene( std::string name, CreateSceneFcn fcn) {
  SceneCreatorFcns.insert( 
    std::pair<std::string, CreateSceneFcn>(name, fcn));
}

Scene* SceneFactory::getScene( std::string name) {
  auto it = SceneCreatorFcns.find(name);
  if ( it == SceneCreatorFcns.end() ) {
    return NULL;
  }
  return (it->second)();
}

SceneFactory& SceneFactory::getInstance(){
  static SceneFactory theFactory;
  return theFactory;
}
