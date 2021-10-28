// Code adapted from www.learnopengl.com, www.glfw.org

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtc/constants.hpp>

#include "shader.h"

int main(void)
{
	//++++create a glfw window+++++++++++++++++++++++++++++++++++++++
	GLFWwindow* window;

	if (!glfwInit()) //Initialize the library
		return -1;

	window = glfwCreateWindow(640, 640, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);//Make the window's context current

	//++++Initialize GLEW to setup the OpenGL Function pointers+++++++
	glewExperimental = GL_TRUE;
	glewInit();

	//++++Define the viewport dimensions++++++++++++++++++++++++++++
	glViewport(0, 0, 640, 640);

	//++++++++++Set up vertex data (and buffer(s)) and attribute pointers+++++++++
	GLfloat firstTriangle[] = {
		// Position data,     colour data
		-0.9f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Left  
		0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Right 
		-0.45f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Top   
	};

	GLfloat secondTriangle[] = {
		// Position data,     colour data
		0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Left  
		0.9f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Right 
		0.45f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Top   
	};

	GLuint VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	// ================================
	// buffer setup
	// ===============================
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//++++++++++Build and compile shader program+++++++++++++++++++++
	GLuint shaderProgram = initShader("vert.glsl", "frag.glsl");

	//++++++++++++++++++++++++++++++++++++++++++++++
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the first Triangle
		// use shader
		glUseProgram(shaderProgram);

		//+++++++ Define transform matrix (uncomment the transform matrix you want to create)+++++++
		glm::mat4 transform;

		/* Translation */
		//transform = glm::translate(transform, glm::vec3(0.5f, 0.0f, 0.0f));

		/* Rotation */
		//transform = glm::rotate(transform, (GLfloat)(3.14), glm::vec3(0.0f, 0.0f, 1.0f));

		/* Scaling */
		//transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.0f));

		/* animation example: triangle rotation */
		//transform = glm::rotate(transform, (GLfloat)(glfwGetTime()*0.5f), glm::vec3(0.0f, 0.0f, 1.0f));

		/* animation example: triangle translation */
		/*if ((GLfloat)glfwGetTime() * 0.1f <= 0.5)
			transform = glm::translate(transform, glm::vec3((GLfloat)glfwGetTime() * 0.1f, 0.0f, 0.0f));
		else
			transform = glm::translate(transform, glm::vec3(0.5f, 0.0f, 0.0f));*/

		/* animation example:  */
		/*GLfloat dx = (GLfloat)(glm::abs(glm::sin(glfwGetTime())) * 0.5);
		transform = glm::translate(transform, glm::vec3(dx, 0.0f, 0.0f));*/


		// Get matrix's uniform location and set matrix
		GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// draw object
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//second triangle
		transform = glm::mat4();

		// Get matrix's uniform location and set matrix
		transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);

	glfwTerminate();
	return 0;
}