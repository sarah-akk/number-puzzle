#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>



class WindowSize {
public:
	int width;
	int height;

	WindowSize(int w, int h) : width(w), height(h) {}
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Window {
public:
 	static void poll();
	static void updateViewportGLFW(GLFWwindow* window, int width, int height);
	static void clear(GLFWwindow* window);
	static int shouldClose(GLFWwindow* window);
	static WindowSize getWindowSize(GLFWwindow* window);
};

#endif // !WINDOW_H
