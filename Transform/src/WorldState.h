#ifndef __WORLDSTATE_H
#define __WORLDSTATE_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

#define NUM_TRACKED_FRAMES 10

class WorldState
{
private:
	float frameTimes[NUM_TRACKED_FRAMES];
	bool running;
	glm::mat4 currentMatrix;
	glm::mat4 ident;
	int activeMatrix;


	glm::mat4 identity = glm::mat4(1.0f);

	glm::mat4 getMatrix1()
	{
		//TODO4 Set the current matrix to scale by 1.5
		glm::vec3 doubleSize = glm::vec3(1.5, 1.5, 1.5);
		glm::mat4 scale = glm::scale(identity, doubleSize);
		return scale;
	}
	
	glm::mat4 getMatrix2()
	{
		//TODO5 Set the current matrix to rotate by 45 degrees.
		// Hint: angle must be a float.
		float angle = 0.25 * M_PI;
		glm::vec3 zAxis = glm::vec3(0, 0, 1);
		glm::mat4 rotation = glm::rotate(identity, angle, zAxis);
		return rotation;
	}
	
	glm::mat4 getMatrix3()
	{
		//TODO6 Set the current matrix to translate along X by -0.5 and along Y by 0.5
		glm::vec3 moveXY = glm::vec3(-0.5, 0.5, 0);
		glm::mat4 translation = glm::translate(identity, moveXY);
		return translation;
	}
	
	glm::mat4 getMatrix4()
	{
		//This just returns the identity
		glm::mat4 identity = glm::mat4(1.0f);
		return identity;
	}
	
public:
	WorldState()
	{
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = 0.0f;
		running = true;
		currentMatrix = glm::mat4(1.0);
		ident = glm::mat4(1.0);
		activeMatrix = -1;
	}

	void printText(unsigned char c)
	{ printf("key %c\n", c); }
        
	void printMousePos(int x, int y)
	{ printf("move %d %d\n", x, y); }
	
	void moveUp()
	{ printf("^"); }
	
	void moveDown()
	{ printf("v"); }
	
	void moveLeft()
	{ printf("<"); }
	
	void moveRight()
	{ printf(">"); }
	
	void updateFrameTime(float timeAsSeconds)
	{
		for(size_t i=1; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = frameTimes[i-1];
		frameTimes[0] = timeAsSeconds;
	}
	
	void printFPS() const
	{
		float sum = 0.0f;
		float avg = 0.0f;
		float fps = 0.0f;
		
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			sum += frameTimes[i];
		
		avg = sum / NUM_TRACKED_FRAMES;
		fps = 1.0f / avg;
		printf("fps %f\n", fps);
	}

	void setRunning(bool r)
	{ running = r; }

	bool isRunning() const
	{ return running; }
	
	void nextMatrix()
	{
		activeMatrix++;
		if(activeMatrix > 3) activeMatrix = 0;
		
		switch(activeMatrix) {
			case 0: currentMatrix = getMatrix1(); break;
			case 1: currentMatrix = getMatrix2(); break;
			case 2: currentMatrix = getMatrix3(); break;
			case 3: currentMatrix = getMatrix4(); break;
			default: currentMatrix = glm::mat4(1.0);
		}
		
		printf("matrix: %i\n", activeMatrix);
	}
	
	glm::mat4 getCurrentMatrix()
	{ return currentMatrix; }
};

#endif
