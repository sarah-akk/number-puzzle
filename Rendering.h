	#pragma once
	#ifndef RENDERING_H
	#define RENDERING_H
	#define EDGEGUARD 10
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
	#include <fstream>
	#include <iostream>
	#include <stb_image.h>
	#include <string>
	#include "Board.h"
	#include "Window.h"
	#include "shaderClass.h"


	class computedTransforms {
	public:
		float width;
		float height;
	};

	////////////////////////////////////////////////////////////////////

	class LoadedTexture {
	public:
		int width;
		int height;
		unsigned int texture_id;
	};

	////////////////////////////////////////////////////////////////////

	class Button {
	public:
		glm::vec2 position;
		glm::vec2 size;
	};

	////////////////////////////////////////////////////////////////////


	class Rendering {

	public:


		static computedTransforms calculateRenderingGrid(GameBoard* board, WindowSize size);

		static void renderTexturedQuad(Shader* shader, LoadedTexture* texture, glm::mat4* model, glm::mat4* proj, glm::vec2 trans, glm::vec3 color);

		static LoadedTexture loadTexture(const char* filepath);

		//static void  checkCompileErrors(GLuint shader, std::string type);

	};

	#endif // !RENDERING_H
