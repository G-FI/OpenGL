#ifndef MESH_HPP_

#include<glad/glad.h>
#include<GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<string>
#include<vector>

#include"Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	unsigned int Id;
	std::string Type;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices)
	{
		this->vertices = vertices;
		this->textures = textures;
		this->indices = indices;
	}
	void Draw(Shader shader)
	{
		unsigned int diffuseIdx = 1;
		unsigned int specularIdx = 1;
		//设置纹理
		for (int i = 0; i < textures.size(); i++)
		{
			//先激活第i个理单元
			glActiveTexture(GL_TEXTURE0 + i);
			std::string index;
			if (textures[i].Type == "texture_diffuse")
				index = std::to_string(diffuseIdx++);
			else
				index = std::to_string(specularIdx++);
			shader.setInt("material." + textures[i].Type + index, i);
			//绑定第i个纹理单元
			glBindTexture(GL_TEXTURE_2D, textures[i].Id);
		}
		glActiveTexture(GL_TEXTURE0);

		//绘制网格
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
private:
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;


	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	void setup()
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		//设置顶点属性
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	};

};




#endif // !MESH_HPP_
