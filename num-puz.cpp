#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shaderClass.h"
#include "Window.h"
#include "Board.h"
#include "Rendering.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <vector>


WindowSize  test = { 610, 610 };
GameBoard board;

int has_won(GameBoard* board)
{
	int last_number = 0;
	for (int i = 0; i < GameBoard::WIDTH; i++) {
		for (int j = 0; j < GameBoard::HEIGHT; j++) {
			if (board->board_segments[i][j] == last_number) {
				last_number++;
			}
			else
				return 0;
		}
	}
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[])

{
	if (argc != GameBoard::GRIDLENGTH + 1) {
		// Handle incorrect number of arguments
		cout << "input valid numbers!";
		return 1; 
	}


	std::vector<int> initialPositions;
	for (int i = 0; i < 9 ; ++i) {
		int cellPosition = std::stoi(argv[i + 1]);  // Adjust index here
		initialPositions.push_back(cellPosition);
	}
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	void processInput(GLFWwindow * window, GameBoard * board);
	bool isMouseClick(GLFWwindow * window);

	GLFWwindow* window = glfwCreateWindow(610, 610, "Number Puzzle", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, Window::updateViewportGLFW);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << ("Failed to initialize GLAD");

	}

	gladLoadGL();


	GameBoard board;

	board.generateBoard(initialPositions,&board);

	//board.randomizeBoard(&board);


	cout << "ok";

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	LoadedTexture textures[11] = { 0 };

	for (size_t i = 1; i <= 11 ; i++)
	{
		std::string number = std::to_string(i);

		std::string path = "assets/" + std::to_string(i) + ".jpg";

		textures[i - 1] = Rendering::loadTexture(path.c_str());
	}

     computedTransforms transform = Rendering::calculateRenderingGrid(&board, test);
	//std::cout << transform.height << " " << transform.width;

	Shader shader = Shader("defult.vert", "defult.frag");


	unsigned int vao, vbo, EBO;

	float vertices[] = {
		// positions          // texture coords
		 1.0f,  1.0f,    1.0f, 1.0f, // top right
		 1.0f,  0.0f,    1.0f, 0.0f, // bottom right
		 0.0f,  0.0f,    0.0f, 0.0f, // bottom left
		 0.0f,  1.0f,    0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &EBO);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	while (!Window::shouldClose(window))
	{

		Window::poll();
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0f, 0.5f, 0.6f, 1.0f);

		processInput(window, &board);


		for (int i = 0; i < GameBoard::WIDTH; i++) {
			for (int j = 0; j < GameBoard::HEIGHT; j++) {

				int index = board.board_segments[i][j];


				glm::mat4 projection = glm::ortho(0.0f, 610.0f, 610.0f, 0.0f, -1.0f, 1.0f);

				glm::vec3 color = { 1.0, 1.0, 1.0 };

				glm::vec3 pos(((j + 1) - 1) * transform.width + EDGEGUARD * (j + 1),
					((i + 1) - 1) * transform.height + EDGEGUARD * (i + 1), 0.0f);

				//std::cout << ((seg->col + 1) - 1) * transform.width + EDGEGUARD * (seg->col + 1)<<"  ";
				//5std::cout << ((seg->row + 1) - 1)* transform.height + EDGEGUARD * (seg->row + 1)<<" ";

				glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), pos);

				glm::vec2 size = { transform.width, transform.height };
				//std::cout<< transform.width<<" ";

				if (index < 9)
					Rendering::renderTexturedQuad(&shader, &textures[index], &model_matrix, &projection, size, color);
					

				glBindVertexArray(vao);

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}

		}


		if (has_won(&board))
		{
			glm::mat4 projection = glm::ortho(0.0f, 610.0f, 610.0f, 0.0f, -1.0f, 1.0f);

			glm::vec3 pos(0.0f, 0.0f, 0.0f);

			glm::vec2 size = { 610.0f, 610.0f };

			glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), pos);

			glm::vec3 color = { 1.0, 1.0, 1.0 };

			Rendering::renderTexturedQuad(&shader, &textures[9], &model_matrix, &projection, size, color);

			glBindVertexArray(vao);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			double mouseX, mouseY;

			Button restartButton;

			restartButton.position = glm::vec2(0.0f, 490.0f);
			restartButton.size = glm::vec2(250.0f, 100.0f);

			glm::mat4 restartModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(restartButton.position, 0.0f));
			glm::mat4 restartProjection = glm::ortho(0.0f, 610.0f, 610.0f, 0.0f, -1.0f, 1.0f);


			glm::vec3 restartColor = { 1.0f, 1.0f, 1.0f }; // Adjust the color as needed
			Rendering::renderTexturedQuad(&shader, &textures[10], &restartModelMatrix, &restartProjection, restartButton.size, restartColor);

			glBindVertexArray(vao);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


			glfwGetCursorPos(window, &mouseX, &mouseY);

			if (mouseX >= restartButton.position.x && mouseX <= restartButton.position.x + restartButton.size.x &&
				mouseY >= restartButton.position.y && mouseY <= restartButton.position.y + restartButton.size.y &&
				isMouseClick(window)) {

				board.generateBoard(initialPositions,&board);
				//board.randomizeBoard(&board);

			}
		}



		Window::clear(window);
	}

	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void processInput(GLFWwindow* window, GameBoard* board)
{
	static bool rightKeyReleased = true;
	static bool leftKeyReleased = true;
	static bool upKeyReleased = true;
	static bool downKeyReleased = true;

	int indx1 = 0, indx2 = 0;

	// Find the position of the '8' tile
	for (int i = 0; i < GameBoard::WIDTH; i++) {
		for (int j = 0; j < GameBoard::HEIGHT; j++) {
			if (board->board_segments[i][j] == 8) {
				indx1 = i;
				indx2 = j;
				break;
			}
		}
	}

	// Handle key presses
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && rightKeyReleased && indx2 > 0 ) {
		int value = board->board_segments[indx1][indx2 - 1];
		board->board_segments[indx1][indx2 - 1] = 8;
		board->board_segments[indx1][indx2] = value;
		rightKeyReleased = false;
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) {
		rightKeyReleased = true;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && leftKeyReleased && indx2 < GameBoard::WIDTH - 1) {
		int value = board->board_segments[indx1][indx2 + 1];
		board->board_segments[indx1][indx2 + 1] = 8;
		board->board_segments[indx1][indx2] = value;
		leftKeyReleased = false;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) {
		leftKeyReleased = true;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && upKeyReleased && indx1 < GameBoard::HEIGHT - 1 ) {
		int value = board->board_segments[indx1 + 1][indx2];
		board->board_segments[indx1 + 1][indx2] = 8;
		board->board_segments[indx1][indx2] = value;
		upKeyReleased = false;
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) {
		upKeyReleased = true;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && downKeyReleased && indx1 > 0 ) {
		int value = board->board_segments[indx1 - 1][indx2];
		board->board_segments[indx1 - 1][indx2] = 8;
		board->board_segments[indx1][indx2] = value;
		downKeyReleased = false;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE) {
		downKeyReleased = true;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool isMouseClick(GLFWwindow* window) {
	return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}
