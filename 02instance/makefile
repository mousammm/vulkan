# CFLAGS = -std=c++17 -O2

CFLAGS = -std=c++17 

LDFLAGS = -lglfw -lvulkan -ldl -lpthread 

vulkanTest: main.cpp
	g++ $(CFLAGS) -o vulkanTest main.cpp $(LDFLAGS)

.PHONY: test clean

test: vulkanTest
	./vulkanTest

clean:
	rm -f vulkanTest

