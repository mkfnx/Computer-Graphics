CFLAGS = -Wall

run: program
	./program

program: main.o raster.o
	gcc -o $@ $< raster.o ${CFLAGS} `pkg-config gtk+-3.0 --cflags --libs`

main.o: main.c
	gcc -c $< `pkg-config gtk+-3.0 --cflags --libs`

raster.o: raster.c
	gcc -c $< `pkg-config gtk+-3.0 --cflags --libs`

clean:
	rm main.o program raster.o
