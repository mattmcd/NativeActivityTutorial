all: libcube.so testCube

testCube: cube_cpp.cpp engine.hpp engine.cpp
	$(CXX) -o testCube cube_cpp.cpp engine.cpp -shared -lEGL -lGLESv1_CM -lm

libcube.so: cube.o
		$(CC) -o libcube.so cube.o -shared -lEGL -lGLESv1_CM -lm

libcube_cpp.so: cube_cpp.o
		$(CXX) -o libcube_cpp.so cube_cpp.o -shared -lEGL -lGLESv1_CM -lm
clean:
		rm libcube.so cube.o
		rm libcube_cpp.sp cube_cpp.o