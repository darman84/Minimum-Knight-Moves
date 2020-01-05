// Zachary Williams
// 11/2/19 was the starting date for this project, however I have been working infrequently on this
// project due to time restrictions with personal life/work/school
// Note that some of the following code is HEAVILY documented for educational purposes
// TODO: if possible, allow the user to CLICK where they would like to have the target and knight 
// position on the OpenGL window


#include <iostream>
#include <chrono>
#include <queue>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

////////////////// ----shaders template----///////////////
//#version version_number
//in type in_variable_name;
//in type in_variable_name;
//
//out type out_variable_name;
//
//uniform type uniform_name;
//
//void main()
//{
//	// process input(s) and do some weird graphics stuff
//	...
//		// output processed stuff to output variable
//		out_variable_name = weird_stuff_we_processed;
//}
////////////////////////////////////////////////////////
// shaders(R,G,B,OPACITY)
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
// calculating the color output of the pixels
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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
	return 0;
}




// The function below is called whenever the window changes in size by GLFW and fills in the proper arguments
// for you to process
// framebuffer_size_callback args below
// framebuffer_size_callback(GLFWwindow, new window width, new window height)
void framebuffer_size_callback(GLFWwindow* myWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

// processInput allows the user to close the window using the escape key
void processInput(GLFWwindow *myWindow)
{
	// check if user has pressed the escape key
	if (glfwGetKey(myWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(myWindow, true); //set to true if escape is pressed
}

// checkMaxAttributes allows the user to check for the maximum number of vertex attributes that is allowed by their hardware.
// there are always at least 16 4-component vertex attributes available,
// but some hardware might allow for more which you can retrieve by querying GL_MAX_VERTEX_ATTRIBS: 
/*
void checkMaxAttributes()
{
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << endl;
}
*/

int main()
{
	int knightPos[] = { 1,1 };
	int targetPos[] = { 30, 30 };
	int size = 30;



	cout << "The minimum number of steps to reach the target is " << minSteps(knightPos, targetPos, size) << endl;
	//checkMaxAttributes(); // getting an access violation error, will fix later
	cout << "Initializing GLFW... " << endl;
	do {
		cout << '\n' << "Press the Enter key to open the OpenGL window.";
	} while (cin.get() != '\n');
	glfwInit(); // Intitializing glfw

	// glfwWindowHint args below
	// glfwWindowHint(What option we want to configure, integer that sets the value of our option)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Telling GLFW that 3.3 is version of OpenGl we want to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Using core profile explicitly
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //needed for OS X
	
	// glfwCreateWindow args below
	// glfwCreateWindow(width of window, height of window, name for the window, ignore, ignore)
	GLFWwindow* myWindow = glfwCreateWindow(800, 600, "Board", NULL, NULL);
	if (myWindow == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(myWindow);
	glfwSetFramebufferSizeCallback(myWindow, framebuffer_size_callback);

	// Initializing GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// Telling OpenGL the size of the rendering window
	// glViewport args below
	// glViewport(x location of the lower left corner of the window,
	//			  y location of the lower left corner of the window,				
	//			  width of the window,	
	//			  height of the window)
	// Note: We could actually set the viewport dimensions at values smaller than GLFW's dimensions;
	// then all the OpenGL rendering would be displayed in a smaller window and we could for example display
	// other elements outside the OpenGL viewport. 
	// glViewport(0, 0, 800, 600);

		// x,y,z, 3 vertices in this example, z coordinate is zero so that the object remains 2D
	/* for triangle
	float vertices[] = 
	{
	   -0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	*/

	// For rectangle
	float vertices[] = 
	{
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = 
	{  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	// Creation of the element buffer object for the creation of a rectangle
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Generation of a vertex buffer object with a buffer ID
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Generate a Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Binding the newly created buffer to the GL_ARRAY_BUFFER target with the glBindBuffer function
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData Copies the previously defined vertex data into the buffer's memory
	// glBufferData args below
	// glBufferData(the type of buffer we want to copy data into, size of the data in bytes we want to pass to the buffer,
	//				the actual data we want to send, how we want the graphics card to manage the given data)
	// 4th param options - 
	// GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
	// GL_DYNAMIC_DRAW : the data is likely to change a lot.
	// GL_STREAM_DRAW : the data will change every time it is drawn.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Dynamically compile the shader at run-time from it's source code
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// glShaderSource args below
	// glShaderSource(shader object, how many strings we're passing as source code, the actual source code of the vertex shader, NULL)
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checking for compile time errors
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Creating a program object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); // Creates a program and returns the ID reference to the newly created program object
	// Attaching the previously compiled shaders to the program object
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Linking the shaders to the program object
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	glUseProgram(shaderProgram); // Activating the program object
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Telling OpenGL how to interpret the vertex data per vertex attribute
	// glVertexAttribPointer args below
	// glVertexAtrribPointer( 
	//	1. which vertex attribute we want to configure, we specified the location of the position vertex attribute
	//	in the vertex shader with layout (location = 0). This sets the location of the vertex attribute to 0 and since
	//	we want to pass data to this vertex attribute, we pass in 0
	//	2. size of the vertex attribute, vertex attribute is a vec3 so it's composed of 3 values
	//  3. the type of the data which is GL_FLOAT (a vec* in GLSL consists of floating point values)
	//	4. specifies if we want the data to be normalized. If we're inputting integer data types (int, byte)
	//	and we've set this to GL_TRUE, the integer data is normalized to 0 (or -1 for signed data) and 1 when converted
	//	to float. This is not relevant for us so we'll leave this at GL_FALSE.
	//	5. the "stride"; tells us the space between consecutive vertex attributes. Since the next set of position data
	//	is located exactly 3 times the size of a float away we specify that value as the stride. Note that since we know
	//	that the array is tightly packed (there is no space between the next vertex attribute value) we could've also
	//	specified the stride as 0 to let OpenGL determine the stride (this only works when values are tightly packed).
	//	Whenever we have more vertex attributes we have to carefully define the spacing between each vertex attribute
	//	6. of type void*; the offset of where the position data begins in the buffer. Because the position data is at
	//	the start of the data array the value is 0 here.
	//		)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// Render loop that keeps the program open
	// checks at the start of each loop iteration if GLFW has been instructed to close. If so, the function 
	// returns true and the game loop stops running, after which we can close the application
	while (!glfwWindowShouldClose(myWindow))
	{
		// Input
		processInput(myWindow);

		// All rendering commands should go HERE
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // set background color to light green
		glClear(GL_COLOR_BUFFER_BIT);

		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		// drawing a triangle
		// glDrawArrays args below
		// glDrawArrays(OpenGL primitive type that we want to draw, starting index of the vertex array we want to draw, how many vertices)
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		// drawing a rectangle
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Check and call events and swap the buffers
		// glfwSwapBuffers will swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window)
		// that is used to render to during this render iteration and show it as output to the screen. 
		glfwSwapBuffers(myWindow);
		// glfwPollEvents checks if any events are triggered(like keyboard input or mouse movement events), updates the window state, 
		//and calls the corresponding functions(which we can register via callback methods)
		glfwPollEvents();
	}


	// Function below intended for usage involving performance, will be implemented later
	/*
    auto start = chrono::steady_clock::now();
	//
    //  Insert the code that will be timed
	//
    auto end = chrono::steady_clock::now();
    // Store the time difference between start and end
	auto diff = end - start;
	*/
	
	glfwTerminate();
	do {
		cout << '\n' << "Press the Enter key to continue.";
	} while (cin.get() != '\n');
	return 0;
}