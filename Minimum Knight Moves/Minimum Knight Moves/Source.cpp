// Zachary Williams
// 11/2/19 was the starting date for this project, however I have been working infrequently on this
// project due to time restrictions with personal life/work/school
//
//

// Please note that the #includes that were commented out were for usage with GLEW/GLUT
// and are not currently needed for this project, I will keep them here for reference
// in case I change my mind
#include <iostream>
//#include <windows.h>
#include <chrono>
#include <queue>
//#include <GLFW/glfw3.h>
//#include <GL/GL.h>
//#include <GL/GLU.h>
//#include <GL/glut.h>
//#include "ShaderUtil.h"
#include <glad/glad.h> 

using namespace std;

struct Space
{
	int x;
	int y;
	int dist;
	Space() {}
	Space(int x, int y, int dist) : x(x), y(y), dist(dist) {}
};


bool checkPos(int x, int y, int size)
{
	// this function returns true if the position is valid
	if (x >= 1 && x <= size && y >= 1 && y <= size)
	{
		return true;
	}

	return false;
}

int minSteps(int knightPos[], int targetPos[], int size)
{
	// The following two arrays are used to illustrate the possible ways that the knight can move around the board
	int changeX[] = {-2, -1, 1, 2, -2, -1, 1, 2 };
	int changeY[] = {-1, -2, -2, -1, 1, 2, 2, 1 };
	queue<Space> knightSpot; // Queue for knight's state

	knightSpot.push(Space(knightPos[0], knightPos[1], 0)); // Push starting position

	Space temp;
	int x;
	int y;
	// visit is a 2D array of bools
	bool** visit = new bool*[size+1];
	for (int i = 0; i < size+1; ++i)
		visit[i] = new bool[size+1]; 

	/* The 2D array below was commented out because the size was determined at runtime,
	   rather than compiletime, which is not allowed for arrays in c++ 
	*/
	//bool visit[size+1][size+1];

	for (int ctr1 = 1; ctr1 <= size; ctr1++)
		for (int ctr2 = 1; ctr2 <= size; ctr2++)
			visit[ctr1][ctr2] = false;
	// visit starting state 
	visit[knightPos[0]][knightPos[1]] = true;

	// loop until we have one element in the queue 
	while (!knightSpot.empty())
	{
		temp = knightSpot.front();
		knightSpot.pop();

		// if current cell is equal to target cell, 
		// return its distance 
		if (temp.x == targetPos[0] && temp.y == targetPos[1])
			return temp.dist;

		// loop for all reachable states 
		for (int i = 0; i < 8; i++)
		{
			x = temp.x + changeX[i];
			y = temp.y + changeY[i];

			// If reachable state is not yet visited and 
			// inside board, push that state into queue 
			if (checkPos(x, y, size) && !visit[x][y]) 
			{
				visit[x][y] = true;
				knightSpot.push(Space(x, y, temp.dist + 1));
			}
		}
	}
}

/*
	Please ignore the drawMyWindow function below, this function was originally being written for GLEW, which I
	have now decided to not use in this project
*/
#if 0
int drawMyWindow()
{
	GLFWwindow* window;

	// Initialize the library 
	if (!glfwInit())
		return -1;

	// Create a windowed mode window and its OpenGL context 
	window = glfwCreateWindow(640, 480, "Board", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current 
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		// Problem: glewInit failed, something is seriously wrong. 
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	}
	else
	{
		fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

		// TODO: Create and compile shaders here (vertex and fragment shaders)
			// and finally draw something with modern OpenGL!
		ShaderUtil shaderUtil;
		shaderUtil.Load("shaders/vs.shader", "shaders/fs.shader");
		
		// Points for triangle
		/*
		float points[6] = {

			// Left
			-0.8f, -0.5f,

			// Top
			0.0f, 0.9f,

			// Right
			0.5f, -0.7f
		};
		*/

		// Points for chess board
		float points[8] = {

			// Top left
			-0.9f, 0.9f,

			// Top right
			0.9f, 0.9f,

			// Bottom right
			0.9f, -0.9f,

			// Bottom left
			-0.9f, -0.9f
		};

		unsigned int buffer;

		// Create a buffer
		glGenBuffers(1, &buffer);

		// Bind the buffer to vertx attributes
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		// Init buffer
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), points, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

		shaderUtil.Use();

		// Loop until the user closes the window 
		while (!glfwWindowShouldClose(window))
		{
			// Render here 
			glClear(GL_COLOR_BUFFER_BIT);

			// Draw triangle
			//glDrawArrays(GL_TRIANGLES, 0, 3);
			glDrawArrays(GL_LINE_LOOP, 0, 4);

			// Swap front and back buffers 
			glfwSwapBuffers(window);

			// Poll for and process events 
			glfwPollEvents();
		}
		shaderUtil.Delete();
	}

	glfwTerminate();

} 
#endif


int main(int argc, char ** argv)
{
	int knightPos[] = { 1,1 };
	int targetPos[] = { 30, 30 };
	int size = 30;


	cout << "The minimum number of steps to reach the target is " << minSteps(knightPos, targetPos, size) << endl;
	//drawMyWindow(); //was for glew, ignore.




	/*
    auto start = chrono::steady_clock::now();
	//
    //  Insert the code that will be timed
	//
    auto end = chrono::steady_clock::now();
    // Store the time difference between start and end
	auto diff = end - start;
	*/

	system("pause");
	return 0;
}