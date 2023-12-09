#include "Rendering.h"
#include "Board.h"
#include "Window.h"
#include "shaderClass.h"

#define STB_IMAGE_IMPLEMENTATION
#include <fstream>
#include <sstream>
#include <vector>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <learnopengl/filesystem.h>

#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>


computedTransforms Rendering::calculateRenderingGrid(GameBoard* board, WindowSize size)
{
	float scale = 1.0;

	computedTransforms info;

	int space_after_offset_x = size.width - (board->col_max * EDGEGUARD);

	info.width = space_after_offset_x / board->col_max;
	info.width *= scale;


	int space_after_offset_y = size.height - (board->row_max * EDGEGUARD);

	info.height = space_after_offset_y / board->row_max;
	info.height *= scale;

	return info;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LoadedTexture Rendering::loadTexture(const char* filepath)
{
	LoadedTexture info = { 0, 0, 0 };
	 int nrChannels;
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Rendering::renderTexturedQuad(Shader* shader, LoadedTexture * texture, glm::mat4 * model, glm::mat4 * proj, glm::vec2 size, glm::vec3 color)
{
	
	
	unsigned int projectionLoc = glGetUniformLocation(shader->ID, "projection");
	unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(*proj));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(*model));

	glUniform2fv(glGetUniformLocation(shader->ID, "size"), 1, &size[0]);
	glUniform3fv(glGetUniformLocation(shader->ID, "color"), 1, &color[0]);

	shader->Activate();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->texture_id);



}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

