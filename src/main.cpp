int width = 960, height = 960;
float scale = 150;

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "grid.h"

#include "render_code/polyengine/Shader.h"
#include "render_code/polyengine/Textures.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include <siv/PerlinNoise.hpp>

GLuint vao, vbo;
bool wireframe = false , swap = false;

union IntFloat {
	int x;
	float y;
};

struct Tile {
	int tile;
	float height;
};

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		wireframe = !wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
	}
	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		swap = !swap;
	}
}

void windowSizeCallback(GLFWwindow* window, int newWidth, int newHeight) {
	width = newWidth;
	height = newHeight;
	glViewport(0, 0, newWidth, newHeight);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	scale -= yoffset * scale*0.25;
	scale = glm::clamp<float>(scale, 1, 1000);
}

struct TileVertex {
	glm::vec2 aPos;
	int tile;
	glm::vec2 aUV;
	float height;
};

std::vector<TileVertex> vertexBuffer;

siv::PerlinNoise perlin{ std::random_device() };

void AddChunkToVertexBuffer(Grid<Tile>& grid, ChunkCoordinate coord) {
	GridChunk chunk = grid.GetChunk(coord);
	for (int y = 15; y >= 0; y--) {
		for (int x = 0; x < 16; x++) {
			if (chunk.Tiles[y][x].tile == 0) continue;
			Tile tileStruct = chunk.Tiles[y][x];
			int tile = tileStruct.tile;
			float height = tileStruct.height;
			vertexBuffer.push_back({ {(coord.x * 16) + 0 + x,(coord.y * 16) + 0 + y} , tile , {0,0},height});
			vertexBuffer.push_back({ {(coord.x * 16) + 0 + x,(coord.y * 16) + 1 + y} , tile , {0,(float)1 / 16},height});
			vertexBuffer.push_back({ {(coord.x * 16) + 1 + x,(coord.y * 16) + 0 + y} , tile , {(float)1 / 16,0},height});

			vertexBuffer.push_back({ {(coord.x * 16) + 1 + x,(coord.y * 16) + 0 + y} , tile , {(float)1 / 16,0},height});
			vertexBuffer.push_back({ {(coord.x * 16) + 0 + x,(coord.y * 16) + 1 + y} , tile , {0,(float)1 / 16},height});
			vertexBuffer.push_back({ {(coord.x * 16) + 1 + x,(coord.y * 16) + 1 + y} , tile , {(float)1 / 16,(float)1 / 16},height});
		}
	}
}

int main() {
#pragma region WindowInit
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	GLFWwindow* mWindow = glfwCreateWindow(width, height, "Grid game", NULL, NULL);
	if (!mWindow) {
		std::cerr << "Failed to create Window" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwMakeContextCurrent(mWindow);
	if (!gladLoadGL()) {
		std::cerr << "Failed to load GLAD" << std::endl;
		return -1;
	}

	glfwSetWindowSizeCallback(mWindow, windowSizeCallback);
	glfwSetKeyCallback(mWindow, keyCallback);
	glfwSetScrollCallback(mWindow, scrollCallback);
#pragma endregion

	int startRadius = 10;
	Grid<Tile>* firstGrid = new Grid<Tile>(startRadius);
	//firstGrid.SetIndexAt(0, 0, 2);
	//firstGrid.SetIndexAt(3, 0, 1);
	//firstGrid.SetIndexAt(0, 1, 2);
	//firstGrid.SetIndexAt(1, 1, 1);
	//firstGrid.SetIndexAt(1, 2, 3);
	//firstGrid.SetIndexAt(2, 2, 4);
	//firstGrid.SetIndexAt(0, 3, 1);
	//firstGrid.SetIndexAt(3, 3, 1);


	for (int y = -startRadius*16; y < startRadius * 16; y++) {
		for (int x = -startRadius * 16; x < startRadius * 16; x++) {
			float frequency = 0.48f;
			float fx = x * (frequency / 16.0f);
			float fy = y * (frequency / 16.0f);
			float height = perlin.octave2D_01(fx, fy, 10,0.5);
			float threshold = 0.35f;

			float distance = std::sqrtf((fx * fx) + (fy * fy));

			height -= fmaxf(0.0f,(distance * distance) * 0.050f);
			height *= 1.15f;

			int chosenIndex = 4;

			if (height >= 0.07f) chosenIndex = 2;
			if (height >= 0.2f) chosenIndex = 1;
			if (height >= 0.545f) chosenIndex = 3;
			if (height >= 0.78f) chosenIndex = 6;

			Tile insertedTile;
			insertedTile.height = height;
			insertedTile.tile = chosenIndex;
			firstGrid->SetElementAt(x, y, insertedTile);
		}
	}

	PolyEngine::ShaderProgram* defaultShader = new PolyEngine::ShaderProgram();
	defaultShader->AddShader("resource/default.vert", PolyEngine::VERTEX);
	defaultShader->AddShader("resource/default.frag", PolyEngine::FRAGMENT);
	defaultShader->CompileShaders();

	PolyEngine::Image tileAtlasImage = PolyEngine::Image("resource/tile_atlas.png");
	PolyEngine::Texture tileAtlasTexture = PolyEngine::Texture(tileAtlasImage);
	tileAtlasTexture.TexParameterI(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	tileAtlasTexture.TexParameterI(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//AddChunkToVertexBuffer(firstGrid, { 0,0 });
	//AddChunkToVertexBuffer(firstGrid, { -1,0 });
	//AddChunkToVertexBuffer(firstGrid, { 0,-1 });
	//AddChunkToVertexBuffer(firstGrid, { -1,-1 });

	for (int y = -startRadius; y < startRadius; y++) {
		for (int x = -startRadius; x < startRadius; x++) {
			AddChunkToVertexBuffer(*firstGrid,{ x,y });
		}
	}

#pragma region VertexInitialization
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(TileVertex), vertexBuffer.data(), GL_DYNAMIC_DRAW);
	int stride = 5 * sizeof(float) + sizeof(int);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,stride, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(1, 1, GL_INT,stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,stride, (void*)(2 * sizeof(float) + sizeof(int)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, (void*)(4 * sizeof(float) + sizeof(int)));
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
#pragma endregion

	glm::mat4 model, view, projection;
	model = glm::translate(glm::mat4(1.0f),glm::vec3(5,0,0));
	model = glm::scale(model, glm::vec3(8, 8, 1));
	view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	float aspect = (float)width / (float)height;
	projection = glm::ortho<float>(-aspect * scale, aspect * scale, -scale, scale, 0.01f, 100.0f);

	defaultShader->Activate();
	defaultShader->UniformMatrix4f("model", model);
	defaultShader->UniformMatrix4f("view", view);
	defaultShader->UniformMatrix4f("projection", projection);

	double deltaTime , oldTime;
	while (!glfwWindowShouldClose(mWindow)) {
		oldTime = glfwGetTime();
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		aspect = (float)width / (float)height;
		projection = glm::ortho<float>(-aspect * scale, aspect * scale, -scale, scale, 0.01f, 100.0f);

		if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS) {
			view[3].y += -5 * scale * deltaTime;
		}
		if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS) {
			view[3].y += 5 * scale * deltaTime;
		}
		if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS) {
			view[3].x += 5 * scale * deltaTime;
		}
		if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS) {
			view[3].x += -5 * scale * deltaTime;
		}

		defaultShader->Activate();
		defaultShader->UniformMatrix4f("view", view);
		defaultShader->UniformMatrix4f("projection", projection);
		defaultShader->Uniform1i("swap", swap);
		tileAtlasTexture.UseTexture(0);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, vertexBuffer.size());

		glfwPollEvents();
		glfwSwapBuffers(mWindow);
		deltaTime = glfwGetTime() - oldTime;
	}

	return 0;
}