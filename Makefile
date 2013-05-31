all: libcube.so testCube libcube_cpp.so

cube_cpp.o: engine.hpp

engine.o: engine.hpp

testCube: cube_cpp.o engine.o
	$(CXX) -o testCube cube_cpp.o engine.o -shared -lEGL -lGLESv1_CM -lm

libcube.so: cube.o
		$(CC) -o libcube.so cube.o -shared -lEGL -lGLESv1_CM -lm

libcube_cpp.so: cube_cpp.o engine.o
		$(CXX) -o libcube_cpp.so cube_cpp.o engine.o -shared -lEGL -lGLESv1_CM -lm
clean:
		rm libcube.so cube.o
		rm libcube_cpp.sp cube_cpp.o