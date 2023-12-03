#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"shaderClass.h"
#include "Window.h"
#include "Board.h"
#include "Rendering.h"

WindowSize  test = { 600, 600 };
GameBoard board;


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(600, 600, "Number Puzzle", NULL, NULL);

	glfwMakeContextCurrent(window);


	glfwSetWindowSizeCallback(window, Window::updateViewportGLFW);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << ("Failed to initialize GLAD");

	}

	gladLoadGL();


	GameBoard::generateBoard(&board);


	LoadedTexture textures[GameBoard::GRIDLENGTH] = { 0 };

	for (size_t i = 1; i <= GameBoard::GRIDLENGTH; i++)
	{
		std::string number = std::to_string(i);

		std::string path = "assets/" + number + ".png";

		textures[i - 1] = Rendering::loadTexture(path.c_str());
	}

	PrecomputedRenderableTargetTransforms transform = Rendering::calculateRenderingGrid(&board, test);


	Shader shader = Shader("defult.vert", "defult.frag");


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (size_t i = 0; i <= board.board_size; i++)
	{
		GameBoardSegment* seg = &board.board_segments[i];

		if (seg->is_spacer) {
			continue;
		}

		glm::vec3 pos(((seg->col + 1) - 1) * transform.width + EDGEGUARD * (seg->col + 1),
			((seg->row + 1) - 1) * transform.height + EDGEGUARD * (seg->row + 1), 0.0f);

		glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), pos);

		glm::vec2 position = { transform.width, transform.height };
		glm::vec3 color = { 1.0f, 1.0f, 1.0f };

		glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(test.width),
			static_cast<float>(test.height), 0.0f, -1.0f, 1.0f);


		Rendering::renderTexturedQuad(&shader, &textures[seg->identity - 1], &model_matrix, &proj, position, color);

	}

	while (!Window::shouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Window::poll();

		
		Window::clear(window);
	}

}