#pragma once
#ifndef MYFRUSTUM_H
#define MYFRUSTUM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>

#include "light.h"

#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 normal;
	glm::vec2 textureCo;
	Vertex():Position(0.0f), normal(0.0f), textureCo(0.0f) {}
};

class Frustum {
protected:
	float upperEdge;
	float lowerEdge;
	float height;
	Shader shader;
	glm::vec3 position; // 初始位置，默认为原点

public:
	std::vector<Vertex> vertexarray;
	std::vector<glm::vec3> vertices;
	std::vector<glm::ivec3> indices;
	glm::vec3 color;
	unsigned int VAO, VBO;

public:
	Frustum(float upperEdge = 1.0f, float lowerEdge = 1.0f,
		float height = 1.0f, 
		glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3 position = glm::vec3(0.0f))
		:shader("frustum.vs", "frustum.fs") {

		this->upperEdge = upperEdge;
		this->lowerEdge = lowerEdge;
		this->height = height;
		this->color = color;
		this->position = position;

		generateVertex();
		generateIndex();
		generate_vertexarray();
		bind_to_gpu();
	}

	~Frustum() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

public:
	// 返回起始地址
	float* ptrVec() { return &vertices[0].x; }
	int* ptrIndex() { return &indices[0].x; }

	// 高度
	float getHeight() { return height; }
	float getUpperEdge() { return upperEdge; }
	float getLowerEdge() { return lowerEdge; }
public:
	// 使用指定的shader绘制图形
	void draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const Light& light, const Camera& camera) {
		
		shader.use();

		// 配置颜色，变化矩阵
		shader.setVec3("color", color);
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVec3("lightPos", light.lightPos);
		shader.setVec3("lightColor", light.lightColor);
		shader.setVec3("viewPos", camera.Position);

		glBindVertexArray(VAO);
		// glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, vertexarray.size() * sizeof(Vertex));
	}

private:
	// 产生索引
	void generateVertex() {
		// 首先重新清零
		vertices.clear();

		vertices.push_back(glm::vec3(-0.5f * upperEdge, 0.5f * height,  0.5f * upperEdge));
		vertices.push_back(glm::vec3( 0.5f * upperEdge, 0.5f * height,  0.5f * upperEdge));
		vertices.push_back(glm::vec3( 0.5f * upperEdge, 0.5f * height, -0.5f * upperEdge));
		vertices.push_back(glm::vec3(-0.5f * upperEdge, 0.5f * height, -0.5f * upperEdge));

		
		vertices.push_back(glm::vec3(-0.5f * lowerEdge, -0.5f * height,  0.5f * lowerEdge));
		vertices.push_back(glm::vec3( 0.5f * lowerEdge, -0.5f * height,  0.5f * lowerEdge));
		vertices.push_back(glm::vec3( 0.5f * lowerEdge, -0.5f * height, -0.5f * lowerEdge));
		vertices.push_back(glm::vec3(-0.5f * lowerEdge, -0.5f * height, -0.5f * lowerEdge));

		for (auto& pos : vertices) {
			pos += this->position;
		}
	}

	// 产生索引
	void generateIndex() {
		// 首先重新清零
		indices.clear();

		indices.push_back(glm::ivec3(0, 1, 2));
		indices.push_back(glm::ivec3(0, 3, 2));

		indices.push_back(glm::ivec3(0, 1, 5));
		indices.push_back(glm::ivec3(0, 4, 5));

		indices.push_back(glm::ivec3(1, 6, 5));
		indices.push_back(glm::ivec3(1, 6, 2));

		indices.push_back(glm::ivec3(2, 7, 3));
		indices.push_back(glm::ivec3(2, 7, 6));

		indices.push_back(glm::ivec3(0, 7, 3));
		indices.push_back(glm::ivec3(0, 7, 4));

		indices.push_back(glm::ivec3(4, 6, 5));
		indices.push_back(glm::ivec3(4, 6, 7));
	}

	void bind_to_gpu() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO); 

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexarray.size() * sizeof(Vertex), &(vertexarray[0].Position.x), GL_STATIC_DRAW);
		// position atribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
 
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(offsetof(Vertex, Position)));
		glEnableVertexAttribArray(1);

		/*
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(2);
		*/
	}

	void generate_vertexarray() {
		vertexarray.clear();
		// 初始化位置
		vertexarray = std::vector<Vertex>(3 * indices.size());
		for (int i = 0; i < indices.size(); ++i) {
			vertexarray[3 * i].Position = vertices[indices[i].x];
			vertexarray[3 * i + 1].Position = vertices[indices[i].y];
			vertexarray[3 * i + 2].Position = vertices[indices[i].z];
		}
		// 初始化法向量
		std::vector<glm::vec3> normals;
		normals.push_back(glm::normalize(glm::cross(vertices[1] - vertices[0], vertices[3] - vertices[0])));
		normals.push_back(glm::normalize(glm::cross(vertices[5] - vertices[0], vertices[1] - vertices[0])));
		normals.push_back(glm::normalize(glm::cross(vertices[5] - vertices[1], vertices[6] - vertices[1])));
		normals.push_back(glm::normalize(glm::cross(vertices[6] - vertices[2], vertices[3] - vertices[2])));
		normals.push_back(glm::normalize(glm::cross(vertices[3] - vertices[0], vertices[4] - vertices[0])));
		normals.push_back(glm::normalize(glm::cross(vertices[7] - vertices[4], vertices[5] - vertices[4])));
		for (int i = 0; i < 6; ++i) {
			for (int j = 0; j < 6; ++j) {
				vertexarray[6 * i + j].normal = normals[i];
			}
		}
	}

};
#if 0
void draw(Frustum * obj, glm::mat4 model, Shader& shader) {
	// 使用指定的shader
	shader.setMat4("model", model);
	
	// 绘制
	obj->draw(shader);
}
#endif

#endif
