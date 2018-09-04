
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
// TODO1: You will need to use vectors. Include the vector header here.
// Vectors are defined in <vector> header
#include <vector>


// TODO2: You will need to use math functions. Include the math header here.
// Math functions are defined in <math.h> header
#include <math.h>


#ifdef __APPLE__
#include <fcntl.h>
#include <unistd.h>
#endif

using namespace std;

#include "gl3w/gl3w.h"
#include <SFML/Window.hpp>
#include "simplePNG.h"
#include "GLHelper.h"
#define RESOLUTION 512


GLfloat positions[10] = {-1,-1, 1,1, -0.5,-0.366, 0,0.5, 0.5,-0.366};
GLfloat colors[15] = {1,1,1, 0.5,0.5,0.5, 1,0,0, 0,1,0, 0,0,1};

// TODO3: declare a vector that can hold GLFloats.
// It should look like: vector<TYPE TO HOLD> name_of_variable;
// Name this vector 'myPositions'
vector<GLfloat> myPositions;

// TODO4: declare another vector that can hold GLFloats.
// Name this vector 'myColors'
vector<GLfloat> myColors;


GLuint shaderProg;

GLuint simpleVoa = 0;

GLuint positionBuffer = 0;
GLuint colorBuffer = 0;
//TODO7: Add GLuints to hold your position and color buffers
// Call them 'myPositionBuffer' and 'myColorBuffer'
GLuint myPositionBuffer = 0;
GLuint myColorBuffer = 0;


GLuint positionSlot = 0;
GLuint colorSlot = 0;

float getVer();
void printProgramLinkLog(GLuint obj);
void printProgramCompileLog(GLuint obj);

/*
 Draw a single frame
 */
void display()
{
	// Clear the color bits in the display buffer
	glClear(GL_COLOR_BUFFER_BIT);
	
	// Use a simple shader to render the line
	glUseProgram(shaderProg);
	
	// Render using vertex attributes (data already on GPU) (~2008, 3.0)
	// https://web.archive.org/web/20150225192608/http://www.arcsynthesis.org/gltut/Basics/Tut01%20Following%20the%20Data.html
	
	// Tell OpenGL we want to use a buffer on the GPU
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	checkGLError("bind");
	
	// Tell OpenGL what shader data slot we want to use
	glEnableVertexAttribArray(positionSlot);
	checkGLError("attrib");
	
	// Tell OpenGL how to interpret the data
	glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, 0, 0);
	checkGLError("ptr");
	
	// Do the same thing for colors
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glEnableVertexAttribArray(colorSlot);
	glVertexAttribPointer(colorSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
	checkGLError("ptr2");
	
	// Draw some primitives as: glDrawArrays(type, first, count)
	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_TRIANGLES, 2, 3);
	checkGLError("draw");
	
	// Tell OpenGL we are done with the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// TODO10: set pointers for your position data
	// First, bind the buffer object: glBindBuffer(GL_ARRAY_BUFFER, BUFFER_ID);
	//    You probably will use 'myPositionBuffer'
	// Next, enable the data slot on the shader program: glEnableVertexAttribArray(SLOT_ID);
	//    You probably want 'positionSlot' for position, and 'colorSlot' for color
	// Last, link the slot and a buffer: glVertexAttribPointer(SLOT_ID, PRIM_COUNT, TYPE, GL_FALSE, 0, 0);
	//    You probably want 2 for the position count (x,y) and 3 for the color count (r,g,b)
	//    The type for both should be 'GL_FLOAT'
	//    We won't be using the other options for now
	glBindBuffer(GL_ARRAY_BUFFER, myPositionBuffer);
	glEnableVertexAttribArray(positionSlot);
	glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, 0, 0);

	
	// TODO11: repeat the same process for the color data
	glBindBuffer(GL_ARRAY_BUFFER, myColorBuffer);
	glEnableVertexAttribArray(colorSlot);
	glVertexAttribPointer(colorSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// TODO12: draw all 360 points as lines
	// Draw the circle with: glDrawArrays(PRIM_TYPE, START_ID, COUNT);
	//    You should draw 360 GL_LINES, starting at index 0
	glDrawArrays(GL_LINES, 0, 360);
	
	checkGLError("my positions");
	
	// Tell OpenGL we are done with the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Tell OpenGL we are done with the shader slot
	glDisableVertexAttribArray(positionSlot);
	glDisableVertexAttribArray(colorSlot);
	
	// Tell OpenGL we are done with the shader
	glUseProgram(0);
	checkGLError();
}

/*
 Initialize the graphics state
 */
void graphicsInit()
{
	// Shaders are programs that do the actual rendering on the GPU
	// We will discuss these in detail later, for now, just set them up
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	char const * vertSource =
"#version 330\n \
in vec2 pos; \
in vec3 color; \
out vec4 smoothColor; \
void main() { \
	gl_Position=vec4(pos.xy, 0, 1); \
	smoothColor=vec4(color.xyz, 1); \
}";
	char const * fragSource =
"#version 330\n \
in vec4 smoothColor; \
out vec4 fragColor; \
void main() { \
	fragColor = smoothColor; \
}";
	
	glShaderSource(vertShader, 1, (char const **)&vertSource, NULL);
	glShaderSource(fragShader, 1, (char const **)&fragSource, NULL);
	
	glCompileShader(vertShader);	
	printProgramCompileLog(vertShader);
	glCompileShader(fragShader);
	printProgramCompileLog(fragShader);
	
	shaderProg = glCreateProgram();
	glAttachShader(shaderProg, vertShader);
	glAttachShader(shaderProg, fragShader);
	
	glLinkProgram(shaderProg);
	printProgramLinkLog(shaderProg);
	
	// The data we will render needs to be on the GPU
	// These commands upload the data
	
	// Find out where the shader expects the data
	positionSlot = glGetAttribLocation(shaderProg, "pos");
	colorSlot = glGetAttribLocation(shaderProg, "color");
	//printf("posSlot %i\ncolorSlot %i\n", positionSlot, colorSlot);

	// Vertex attibute objects store array bindings for easy use
	glGenVertexArrays(1, &simpleVoa);
	glBindVertexArray(simpleVoa);

	// Generate a GPU side buffer
	glGenBuffers(1, &positionBuffer);
	
	// Tell OpenGL we want to work with the buffer we just made
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	
	// Allocate and upload data to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	
	// Debind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Do the same thing for the color data
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	checkGLError("init done");
	
	//Create data for 360 vertices
	size_t count = 360;
	for(int i=0; i<count; i++) {
		// TODO5: Create a position for each vertex.
		// Put the positions at the end of your position vector.
		// Push them in the order x, y, x, y, x, y, ...
		//    Use the vector.push_back() method to add the positions
		// The x position should be cos(deg*pi/180)
		// The y position should be sin(deg*pi/180)
		//    math.h defines the value of pi as the constant M_PI
		myPositions.push_back(cos(i * M_PI / 180.0f));
		myPositions.push_back(sin(i * M_PI / 180.0f));
		
		// TODO6: Add an RGB triple for each vertex
		// Add in the order r, g, b, r, g, b, r, g, b, ...
		// You can use any bright color you like.
		// OpenGL colors range from 0.0 to 1.0
		myColors.push_back(1.0);
		myColors.push_back(1.0);
		myColors.push_back(1.0);

	}
	
	// TODO8: Upload your position buffer to the GPU
	// Create the buffer object: glGenBuffers(objCount, pointer_to_id_handle);
	//    You probably want a count of 1, and a pointer to 'myPositionBuffer'
	// Activate the new buffer object: glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	//    Use the buffer id you just got back from GenBuffer
	// Upload the data: glBufferData(GL_ARRAY_BUFFER, size_in_bytes, pointer_to_data, GL_STATIC_DRAW);
	//    You can get a vectors size with the .size() method
	//    You should use sizeof(..) to get the byte size of GLfloats
	//    You can get a pointer to the data with &someVector[0]
	// Deactivate the buffer object with: glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenBuffers(1, &myPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, myPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, myPositions.size() * sizeof(GLfloat), &myPositions[0], GL_STATIC_DRAW);

	
	// TODO9: Repeat the process for the color data
	glGenBuffers(1, &myColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, myColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, myColors.size() * sizeof(GLfloat), &myColors[0], GL_STATIC_DRAW);
	
	
	checkGLError("my init done");
}

void saveBuffer(sf::Window const & window)
{
    unsigned char *dest;
    unsigned int w = window.getSize().x;
    unsigned int h = window.getSize().y;
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    dest = (unsigned char *) malloc( sizeof(unsigned char)*w*h*3);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, dest);
    
    simplePNG_write("_program1.png", w, h, dest);
    free(dest);
}

class GLBox
{
public:
	GLBox()
	{
		// Create the main window
		//sf::Window window(sf::VideoMode(RESOLUTION, RESOLUTION, 32), "program1");
		getWindowContext();
		float ver = getVer();
		if( ver < 1.0f ) {
			printf("OpenGL is not supported.\n");
			exit(1);
		}
		printf("OpenGL version %.1f is supported.\n", ver);
		
		graphicsInit();
        unsigned int frameCount = 0;
		
		// Start render loop
		while (window->isOpen())
		{			
			// Set the active window before using OpenGL commands
			// It's not needed here because the active window is always the same,
			// but don't forget it if you use multiple windows or controls
            window->setActive();
			
			// Handle any events that are in the queue
			sf::Event event;
			while (window->pollEvent(event))
			{
				// Close window : exit
				//if (event.type == sf::Event::Closed)
				//	window.close();
				
				// Escape key : exit
				if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
					window->close();
				
				// This is for grading your code. DO NOT REMOVE
				if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Equal)
				{
					saveBuffer(*window);
				}
			}
			
			// Render the scene
			display();
            frameCount++;
            if(frameCount == 100)
                saveBuffer(*window);
			
			// Finally, display rendered frame on screen
			window->display();
		}
	}
	
	~GLBox()
	{
		// Clean up the buffer
		glDeleteBuffers(1, &positionBuffer);
	}
private:
	
	float getVer()
	{
		float ver = 0.0f;
		
		if (gl3wInit()) {
			fprintf(stderr, "failed to initialize OpenGL\n");
		}
		
		if (gl3wIsSupported(1, 1)) { ver = 1.1f; }
		if (gl3wIsSupported(1, 2)) { ver = 1.2f; }
		if (gl3wIsSupported(1, 3)) { ver = 1.3f; }
		if (gl3wIsSupported(1, 4)) { ver = 1.4f; }
		if (gl3wIsSupported(1, 5)) { ver = 1.5f; }
		
		if (gl3wIsSupported(2, 0)) { ver = 2.0f; }
		if (gl3wIsSupported(2, 1)) { ver = 2.1f; }
		
		if (gl3wIsSupported(3, 0)) { ver = 3.0f; }
		if (gl3wIsSupported(3, 1)) { ver = 3.1f; }
		if (gl3wIsSupported(3, 2)) { ver = 3.2f; }
		if (gl3wIsSupported(3, 3)) { ver = 3.3f; }
		
		if (gl3wIsSupported(4, 0)) { ver = 4.0f; }
		if (gl3wIsSupported(4, 1)) { ver = 4.1f; }
		if (gl3wIsSupported(4, 2)) { ver = 4.2f; }
		if (gl3wIsSupported(4, 3)) { ver = 4.3f; }
		if (gl3wIsSupported(4, 4)) { ver = 4.4f; }
		if (gl3wIsSupported(4, 5)) { ver = 4.5f; }
		
		return ver;
	}
	
	void getWindowContext()
	{
		sf::err().rdbuf(NULL); //hide errors
		
#ifdef __APPLE__
		int nullFD = open("/dev/null", O_WRONLY);
		int oldFD = dup(2); // Duplicate the old file descriptor, so it can be restored
		dup2(nullFD, 2); // Redirect
#endif
		
		sf::VideoMode mode(RESOLUTION, RESOLUTION, 32);
#ifdef __linux__
		sf::ContextSettings settings(32, 0, 0, 3, 3);
#else
		sf::ContextSettings settings(32, 0, 0, 3, 3, sf::ContextSettings::Core);
#endif
		window = new sf::Window(mode, "SFML application", sf::Style::Default, settings);
		
#ifdef __APPLE__
		dup2(oldFD, 2); // Redirect back
		close(oldFD); // Not needed anymore
#endif
	}
	
	sf::Window *window;
};

void printProgramCompileLog(GLuint obj)
{
	GLint infologLength;
	GLint status;
	int charsWritten = 0;
	char *infoLog;
	
	glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE)
		return;
	
	printf("Error compiling shader: ");
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	
	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s",infoLog);
		free(infoLog);
	}
	printf("\n");
}

void printProgramLinkLog(GLuint obj)
{
	GLint infologLength;
	GLint status;
	char *infoLog;
	int charsWritten  = 0;
	
	glGetProgramiv(obj, GL_LINK_STATUS, &status);
	if (status == GL_TRUE)
		return;
	
	printf("Error linking shader: ");
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	
	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s",infoLog);
		free(infoLog);
	}
	printf("\n");
}

int main()
{
	GLBox prog;
	
    return EXIT_SUCCESS;
}
