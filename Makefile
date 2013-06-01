all: libcube.so test_Cube libcube_cpp.so

cube_cpp.o:  engine.hpp

engine.o: engine.hpp

test_Cube: cube_cpp.o engine.o
	$(CXX) -o test_Cube cube_cpp.o engine.o -shared -lEGL -lGLESv1_CM -lm

libcube.so: cube.o
		$(CC) -o libcube.so cube.o -shared -lEGL -lGLESv1_CM -lm

libcube_cpp.so: cube_cpp.o engine.o
		$(CXX) -o libcube_cpp.so cube_cpp.o engine.o -shared -lEGL -lGLESv1_CM -lm
clean:
		rm engine.o
		rm libcube.so cube.o
		rm libcube_cpp.so cube_cpp.o
		rm test_Cube
