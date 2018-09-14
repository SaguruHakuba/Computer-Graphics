#ifndef __GRAPHICS_HELPERS
#define __GRAPHICS_HELPERS

#include "gl3w/gl3w.h"
#include "GLHelper.h"
#include "simplePNG.h"

using namespace std;

#define RESOLUTION 512

GLuint shaderProg;

GLuint simpleVoa = 0;

GLuint positionBuffer = 0;
GLuint colorBuffer = 0;

GLuint positionSlot = 0;
GLuint colorSlot = 0;

GLfloat positions[10] = {-0.5,-0.366, 0,0.5, 0.5,-0.366};
GLfloat colors[15] = {0,1,1, 1,0,1, 1,1,0};

void printProgramLinkLog(GLuint obj);
void printProgramCompileLog(GLuint obj);
float getVer();
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
	glDrawArrays(GL_TRIANGLES, 0, 3);
	checkGLError("draw");
	
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

	// Container VOA for now
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
	//printf("posBuf %i\ncolorBuf %i\n", positionBuffer, colorBuffer);
	
	checkGLError("init done");
}

void saveBuffer(sf::Window const & window)
{
    unsigned char *dest;
    unsigned int w = window.getSize().x;
    unsigned int h = window.getSize().y;
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    dest = (unsigned char *) malloc( sizeof(unsigned char)*w*h*3);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, dest);
    
    simplePNG_write("_image.png", w, h, dest);
    free(dest);
}

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

#endif
