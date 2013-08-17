all:  test_Cube # libcube.so libcube_cpp.so

cube_cpp.o:  cube_cpp.cpp engine.hpp scene_factory.hpp
	$(CXX) -std=c++11 -c -o cube_cpp.o cube_cpp.cpp

engine.o: engine.hpp scene.hpp

scene.o: scene.hpp

scene_factory.o: scene_factory.hpp scene_factory.cpp scene.hpp
	$(CXX) -std=c++11 -c -o scene_factory.o scene_factory.cpp

boxes_scene.o: scene.hpp scene_factory.hpp boxes_scene.hpp boxes_scene.cpp
	$(CXX) -std=c++11 -c -o boxes_scene.o boxes_scene.cpp

nehe_02.o: scene.hpp scene_factory.hpp nehe_02.hpp nehe_02.cpp
	$(CXX) -std=c++11 -c -o nehe_02.o nehe_02.cpp

test_Cube: cube_cpp.o engine.o scene_factory.o scene.o boxes_scene.o nehe_02.o
	$(CXX) -std=c++11 -o test_Cube cube_cpp.o scene.o boxes_scene.o nehe_02.o engine.o scene_factory.o -shared -lEGL -lGLESv1_CM -lm

libcube.so: cube.o
		$(CC) -o libcube.so cube.o -shared -lEGL -lGLESv1_CM -lm

libcube_cpp.so: cube_cpp.o engine.o scene.o boxes_scene.o scene_factory.o
		$(CXX) -std=c++11 -o libcube_cpp.so cube_cpp.o engine.o scene_factory.o scene.o boxes_scene.o -shared -lEGL -lGLESv1_CM -lm
clean:
		rm engine.o
		rm libcube.so cube.o
		rm libcube_cpp.so cube_cpp.o
		rm test_Cube