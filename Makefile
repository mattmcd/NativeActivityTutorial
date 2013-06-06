all:  test_Cube # libcube.so libcube_cpp.so

cube_cpp.o:  engine.hpp scene.hpp

engine.o: engine.hpp scene.hpp

scene.o: scene.hpp

scene_factory.o: scene_factory.hpp scene_factory.cpp scene.hpp
	$(CXX) -std=c++0x -c -o scene_factory.o scene_factory.cpp

boxes_scene.o: scene.hpp boxes_scene.hpp
	$(CXX) -std=c++0x -c -o boxes_scene.o boxes_scene.cpp

test_Cube: cube_cpp.o engine.o scene.o boxes_scene.o scene_factory.o
	$(CXX) -std=c++0x -o test_Cube cube_cpp.o scene.o boxes_scene.o engine.o scene_factory.o -shared -lEGL -lGLESv1_CM -lm

libcube.so: cube.o
		$(CC) -o libcube.so cube.o -shared -lEGL -lGLESv1_CM -lm

libcube_cpp.so: cube_cpp.o engine.o scene.o boxes_scene.o scene_factory.o
		$(CXX) -std=c++0x -o libcube_cpp.so cube_cpp.o engine.o scene_factory.o scene.o boxes_scene.o -shared -lEGL -lGLESv1_CM -lm
clean:
		rm engine.o
		rm libcube.so cube.o
		rm libcube_cpp.so cube_cpp.o
		rm test_Cube