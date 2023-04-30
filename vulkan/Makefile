CFLAGS = -std=c++17 -02

LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: main.cpp
	g++ $(CFlAGS) -o Test main.cpp $(LDFLAGS)

.PHONE: test clean

test: VulkanTest
			./VulkanTest

clean:
	rm -f VulkanTest

