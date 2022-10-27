#pragma once
#include <glad/glad.h>
#include<GLFW/glfw3.h>

class VAO {
public:
	unsigned int ID;
	VAO(){
		glGenVertexArrays(1, &ID);
	}
	void BindVAO(){
		glBindVertexArray(ID);
	}
	void UnBindVAO() {
		glBindVertexArray(0);
	}
	//…Ë÷√∂•µ„ Ù–‘
	void SetVertexAttribPointer(int location, GLint size, GLsizei stride,  const void *pointer) {
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride, pointer);
	}
	~VAO() {
		glDeleteVertexArrays(1, &ID);
	}
};

class VBO {
public:
	unsigned int ID;
	VBO() {
		glGenBuffers(1, &ID);
	}
	void BindVBO() {
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}

	void UnBindVBO() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void BufferData(GLsizeiptr size, const void* data) {
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	~VBO() {
		glDeleteBuffers(1, &ID);
	}
};

class Texture2D {
public:
	unsigned int ID;
	Texture2D() {
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void BindTexture() {
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	~Texture2D() {
		glDeleteTextures(1, &ID);
	}
};

class FBO {
public:
	unsigned int ID;
	FBO() {
		glGenFramebuffers(1, &ID);
	}
	void BindFBO() {
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
	}
	void UnBindFBO() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	~FBO() {
		glDeleteFramebuffers(1, &ID);
	}
};