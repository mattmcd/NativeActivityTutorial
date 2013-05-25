all: libcube.so

libcube.so: cube.o
		$(CC) -o libcube.so cube.o -shared -lEGL -lGLESv1_CM -lm
clean:
		rm libcube.so cube.o