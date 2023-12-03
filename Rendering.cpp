#include "Rendering.h"
#include "Board.h"
#include "Window.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <glad/glad.h>
#include "shaderClass.h"
using namespace std;


PrecomputedRenderableTargetTransforms Rendering::calculateRenderingGrid(GameBoard* board, WindowSize size)
{
	float scale = 1.0;

	PrecomputedRenderableTargetTransforms info;

	int space_after_offset_x = size.width - (board->col_max * EDGEGUARD);

	info.width = space_after_offset_x / board->col_max;
	info.width *= scale;


	int space_after_offset_y = size.height - (board->row_max * EDGEGUARD);

	info.height = space_after_offset_y / board->row_max;
	info.height *= scale;

	return info;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LoadedTexture Rendering::loadTexture(const char* filepath)
{
	LoadedTexture info = { 0, 0, 0 };
	int nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(filepath, &info.width, &info.height, &nrChannels, 0);

	if (data != NULL) {
		glGenTextures(1, &info.texture_id);
		glBindTexture(GL_TEXTURE_2D, info.texture_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.width, info.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		return info;
	}
	throw std::runtime_error("Couldn't load the image: " + std::string(filepath));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Rendering::renderTexturedQuad(Shader* shader, LoadedTexture * texture, glm::mat4 * model, glm::mat4 * proj, glm::vec2 trans, glm::vec3 color)
{

	unsigned int vao ;
	unsigned int vbo ;

	float vertices[] = {
		-0.5f, 0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	shader->Activate();

	// Set shader uniforms
	glUniformMatrix4fv(glGetUniformLocation(shader->shader_id, "model"), 1, GL_FALSE, glm::value_ptr(*model));
	glUniformMatrix4fv(glGetUniformLocation(shader->shader_id, "projection"), 1, GL_FALSE, glm::value_ptr(*proj));
	glUniform2fv(glGetUniformLocation(shader->shader_id, "size"), 1, &trans[0]);
	glUniform3fv(glGetUniformLocation(shader->shader_id, "color"), 1, &color[0]);

	// Activate texture unit 0
	cout << texture->texture_id ;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->texture_id);


	// Draw the quad
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);


}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
