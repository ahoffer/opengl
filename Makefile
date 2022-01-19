all:
	g++ ah_window.cpp ah_graphics.cpp main.cpp glad.c -ldl -lglfw
	./a.out

clean:
	rm a.out