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


GLuint VBOs[7], VAOs[7], EBOs[2];

void makeTriangle(GLfloat arr[], int index) {
	GLfloat newArr[18];
	for (int i = 0; i < 18; i++) {
		newArr[i] = arr[i];
	}

	glBindVertexArray(VAOs[index]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[index]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(newArr), newArr, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void makeSquare(GLfloat arr[], GLuint indices[], int index) {
	GLfloat newArr[24];
	for (int i = 0; i < 24; i++) {
		newArr[i] = arr[i];
	}

	GLuint newIndices[6];
	for (int j = 0; j < 6; j++) {
		newIndices[j] = indices[j];
	}

	int eboIndex;
	if (index == 5) {
		eboIndex = 0;
	}
	else {
		eboIndex = 1;
	}

	glBindVertexArray(VAOs[index]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[index]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(newArr), newArr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[eboIndex]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(newIndices), newIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void drawTriangle(glm::mat4 transform, int index) {
	GLuint shaderProgram = initShader("vert.glsl", "frag.glsl");
	glUseProgram(shaderProgram);

	// Get matrix's uniform location and set matrix
	GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// draw object
	glBindVertexArray(VAOs[index]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void drawSquare(glm::mat4 transform, int index) {
	GLuint shaderProgram = initShader("vert.glsl", "frag.glsl");
	glUseProgram(shaderProgram);

	// Get matrix's uniform location and set matrix
	GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// draw object
	glBindVertexArray(VAOs[index]);
	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
}

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
	GLfloat largeTriangle1[] = {
		// Position data,     colour data
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Left  
		0.2f, 0.2f, 0.0f, 1.0f, 0.0f, 0.0f, // Right 
		0.0f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f, // Top   
	};

	GLfloat largeTriangle2[] = {
		// Position data,     colour data
		0.2f, 0.2f, 0.0f, 0.0f, 0.0f, 1.0f, // Left  
		0.4f, 0.4f, 0.0f, 0.0f, 0.0f, 1.0f, // Right 
		0.0f,  0.4f, 0.0f, 0.0f, 0.0f, 1.0f, // Top   
	};

	GLfloat mediumTriangle[] = {
		// Position data,     colour data
		0.2f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Left  
		0.4f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Right 
		0.4f,  0.2f, 0.0f, 0.0f, 1.0f, 0.0f, // Top   
	};

	GLfloat smallTriangle1[] = {
		// Position data,     colour data
		0.1f, 0.1f, 0.0f, 1.0f, 1.0f, 0.0f, // Left  
		0.3f, 0.1f, 0.0f, 1.0f, 1.0f, 0.0f, // Right 
		0.2f,  0.2f, 0.0f, 1.0f, 1.0f, 0.0f, // Top   
	};

	GLfloat smallTriangle2[] = {
		// Position data,     colour data
		0.3f, 0.3f, 0.0f, 1.0f, 0.0f, 1.0f, // Left  
		0.4f, 0.2f, 0.0f, 1.0f, 0.0f, 1.0f, // Right 
		0.4f,  0.4f, 0.0f, 1.0f, 0.0f, 1.0f, // Top   
	};

	GLfloat square[] = {
		// Position data,    colour data
		0.3f, 0.3f, 0.0f, 1.0f, 0.5f, 0.0f,  // Top
		0.2f, 0.2f, 0.0f, 1.0f, 0.5f, 0.0f, // Left
		0.4f, 0.2f, 0.0f, 1.0f, 0.5f, 0.0f, // Right
		0.3f, 0.1f, 0.0f, 1.0f, 0.5f, 0.0f, //Bottom
	};

	GLuint indices[] = {
		0, 1, 2, // First triangle
		1, 2, 3, // Second triangle
	};

	GLfloat parallelogram[] = {
		// Position data,    colour data
		0.1f, 0.1f, 0.0f, 0.5f, 0.0f, 1.0f,  // Top
		0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 1.0f, // Left
		0.3f, 0.1f, 0.0f, 0.5f, 0.0f, 1.0f, // Right
		0.2f, 0.0f, 0.0f, 0.5f, 0.0f, 1.0f, //Bottom
	};
 

	glGenVertexArrays(7, VAOs);
	glGenBuffers(7, VBOs);
	glGenBuffers(2, EBOs);

	// ================================
	// buffer setup
	// ===============================

	makeTriangle(largeTriangle1, 0);
	makeTriangle(largeTriangle2, 1);
	makeTriangle(mediumTriangle, 2);
	makeTriangle(smallTriangle1, 3);
	makeTriangle(smallTriangle2, 4);
	makeSquare(square, indices, 5);
	makeSquare(parallelogram, indices, 6);


	//++++++++++Build and compile shader program+++++++++++++++++++++
	GLuint shaderProgram = initShader("vert.glsl", "frag.glsl");

	float CenterX = largeTriangle1[0] + largeTriangle1[6] + largeTriangle1[12] / 3;
	float CenterY = largeTriangle1[1] + largeTriangle1[7] + largeTriangle1[13] / 3;
	glm::vec4 center(CenterX, CenterY, 0.0f, 1.0f);
	//++++++++++++++++++++++++++++++++++++++++++++++
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
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




		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// use shader
		glUseProgram(shaderProgram);
		
		glm::mat4 transform;
		glm::mat4 transform0;
		glm::mat4 transform1;
		glm::mat4 transform2;


		// Get matrix's uniform location and set matrix
		GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");

		drawTriangle(transform, 0);
		/* animation example: triangle translation */
		GLfloat time = (GLfloat)glfwGetTime();
		float delay = 0.5f;


		glm::mat4 iden;
		if (time * 0.1f >= delay && time * 0.1f <= 3.14f) {
			glm::mat4 rotate;

			//glm::mat4 translate = glm::translate(transform, glm::vec3((time * 0.1f)-delay, (time * 0.1f)-delay, 0.0f));
			//rotate = glm::translate(transform, glm::vec3(center.x, center.y, 0.0f));
			//rotate = glm::rotate(rotate, (GLfloat)(time * 1.0f)-delay*10, glm::vec3(0.0f, 0.0f, -1.0f));
			//rotate = glm::translate(rotate, glm::vec3(0 - center.x, 0 - center.y, 0.0f));

			rotate = glm::translate(rotate, glm::vec3((time * 0.1f) - delay, (time * 0.1f) - delay, 0.0f));

			glm::mat4 matrix = glm::translate(glm::mat4(), glm::vec3((time * 0.1f)-delay, (time * 0.1f)-delay, 0.0f));
			center = matrix * center;

			/*transform0 = glm::translate(transform0, glm::vec3(center.x, center.y, 0.0f));
			glm::mat4 rotate = glm::rotate(transform0, (GLfloat)(glfwGetTime() * 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			transform0 = glm::translate(transform0, glm::vec3(-center.x, -center.y, 0.0f));*/

			//std::cout << center.x << "\n" << center.y << "\n";

			std::cout << rotate[0].x  << " " << rotate[0].y << "\n";
			std::cout << rotate[1].x << " " << rotate[1].y << "\n";
			std::cout << rotate[2].x << " " << rotate[2].y << "\n";
			std::cout << rotate[3].x << " " << rotate[3].y << "\n";
			
			transform0 = rotate;
			//transform0 = glm::mix(iden, transform0, time * 0.1f);
		}
		else if (time * 0.1f < 0.5){
			transform0 = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
		}
		//else {
			//transform0 = glm::translate(transform0, glm::vec3(0.2, 0.2, 0.0f));
			//transform0 = glm::translate(transform0, glm::vec3(0.7333f, 0.4667f, 0.0f)); // translate to origin
			//transform0 = glm::rotate(transform0, (GLfloat)(glfwGetTime() * 1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
			//transform0 = glm::translate(transform0, glm::vec3(-0.7333f, -0.4667f, 0.0f));
		//}

		drawTriangle(transform0, 1);

		transform2 = glm::translate(transform2, glm::vec3(0.3333f, 0.0667f, 0.0f)); // translate to origin
		transform2 = glm::rotate(transform2, (GLfloat)(glfwGetTime() * 1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		transform2 = glm::translate(transform2, glm::vec3(-0.3333f, -0.0667f, 0.0f));
		drawTriangle(transform2, 2);
		drawTriangle(transform, 3);
		drawTriangle(transform, 4);
		drawSquare(transform, 5);
		drawSquare(transform, 6);
		
		//GLuint shaderProgram = initShader("vert.glsl", "frag.glsl");
		//glUseProgram(shaderProgram);

		//// Get matrix's uniform location and set matrix
		//transformLoc = glGetUniformLocation(shaderProgram, "transform");
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		//// draw object
		//glBindVertexArray(VAOs[6]);
		//glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		//glBindVertexArray(0);

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