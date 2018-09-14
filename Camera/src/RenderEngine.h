#ifndef __RENDERENGINE
#define __RENDERENGINE

#include "GLHelper.h"
#include "WorldState.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

class RenderEngine
{
public:
	RenderEngine()
	{
		initialized = false;
	}
	
	~RenderEngine()
	{
		if(initialized)
		{
			glDeleteVertexArrays(1, &vertexArray);
		}
	}
	
	void display(WorldState const & world)
	{
		if(!initialized)
			init(world.getModel());
		
		//clear the old frame
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//use shader
		glUseProgram(shaderProg);
		world.uploadMatrix(shaderProg);
		
		//draw
		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, 3*12, GL_UNSIGNED_INT, 0);
		
		//cleanup
		glBindVertexArray(0);
		glUseProgram(0);
		checkGLError("render");
	}
	
private:
	bool initialized;
	
	GLuint shaderProg;
	GLuint vertexArray;

	
	void setupShader()
	{
		char const * vertPath = "resources/simple.vert";
		char const * redPath = "resources/simple.frag";
		
		shaderProg = ShaderManager::shaderFromFile(&vertPath, &redPath, 1, 1);
		
		checkGLError("shader");
	}
	
	void setupBuffers(Model const & model)
	{
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);
		
		GLuint positionBuffer;
		GLuint colorBuffer;
		GLuint elementBuffer;
		
		//setup position buffer
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getPositionBytes(), model.getPosition(), GL_STATIC_DRAW);
		GLint positionSlot = glGetAttribLocation(shaderProg, "pos");
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// Do the same thing for the color data
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getColorBytes(), model.getColor(), GL_STATIC_DRAW);
		GLint colorSlot =    glGetAttribLocation(shaderProg, "colorIn");
		glEnableVertexAttribArray(colorSlot);
		glVertexAttribPointer(colorSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// now the elements
		glGenBuffers(1, &elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.getElementBytes(), model.getElements(), GL_STATIC_DRAW);
		//leave the element buffer active
		
		glBindVertexArray(0);
		
		checkGLError("setup");
	}
	
	float initLoader()
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
	
	void init(Model const & model)
	{
		initialized = true;
		
		float ver = initLoader();
		if( ver < 1.0f ) {
			printf("OpenGL is not supported.\n");
			exit(1);
		}
		printf("OpenGL version %.1f is supported.\n", ver);
		
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		
		//setup all shader parts
		setupShader();
		
		//setup buffers
		setupBuffers(model);
	}
};

#endif
