all: libcube.so test_Cube libcube_cpp.so

cube_cpp.o:  engine.hpp scene.hpp

engine.o: engine.hpp scene.hpp

scene.o: scene.hpp

boxes_scene.o: scene.hpp boxes_scene.hpp

test_Cube: cube_cpp.o engine.o scene.o boxes_scene.o
	$(CXX) -std=c++0x -o test_Cube cube_cpp.o scene.o boxes_scene.o engine.o -shared -lEGL -lGLESv1_CM -lm

libcube.so: cube.o
		$(CC) -std=c++0x -o libcube.so cube.o -shared -lEGL -lGLESv1_CM -lm

libcube_cpp.so: cube_cpp.o engine.o scene.o boxes_scene.o 
		$(CXX) -std=c++0x -o libcube_cpp.so cube_cpp.o engine.o scene.o boxes_scene.o -shared -lEGL -lGLESv1_CM -lm
clean:
		rm engine.o
		rm libcube.so cube.o
		rm libcube_cpp.so cube_cpp.o
		rm test_Cube