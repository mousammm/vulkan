# CFLAGS = -std=c++17 -O2 # for release

CFLAGS = -std=c++17 

# LDFLAGS = -lglfw -lvulkan -ldl -lpthread -DNDEBUG  # for release build

LDFLAGS = -lglfw -lvulkan -ldl -lpthread 

vulkanTest: main.cpp
	g++ $(CFLAGS) -o vulkanTest main.cpp $(LDFLAGS)

.PHONY: test clean

test: vulkanTest
	./vulkanTest

clean:
	rm -f vulkanTest

