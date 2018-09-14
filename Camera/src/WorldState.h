#ifndef __WORLDSTATE_H
#define __WORLDSTATE_H
#include "Model.h"
#include "glm/gtc/matrix_transform.hpp"

#define NUM_TRACKED_FRAMES 10

class WorldState
{
private:
	float frameTimes[NUM_TRACKED_FRAMES];
	float currentTime;
	bool running;
	Model model;
	
	bool useGlmCam;
	
	glm::mat4 cameraMatrix;

public:
	WorldState()
	{
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = 0.0f;
		running = true;
		model = Model();
		cameraMatrix = glm::mat4(1.0f);

		useGlmCam = 0;
	}
	
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
	
	Model const & getModel() const
	{ return model; }
	
	void setRunning(bool r)
	{ running = r; }

	bool isRunning() const
	{ return running; }

	float getCurrentTime() const
	{ return this->currentTime; }

	void timeStep(float t)
	{
		this->currentTime = t;
	}
	
	void uploadMatrix(GLint shaderProg) const
	{
		float time = getCurrentTime();
		GLint matSlot = glGetUniformLocation(shaderProg, "m");
		glm::mat4 p = glm::perspective(1.2f, 1.0f, 0.01f, 2.0f);
		glm::mat4 T = p * glm::translate(glm::rotate(glm::translate(cameraMatrix, getModel().getCentroid()), time, glm::vec3(0,1,0)), -getModel().getCentroid());
		glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);
	}
	
	void buildCameraMatrix()
	{
		using namespace glm;
		
		//TODO2 In the TODOs below, you will build the camera matrix
		// Your camera has these properties:
		vec3 position = vec3(0.05f, 0.15f, 0.15);
		vec3 up = vec3(0, 1, 0);
		vec3 lookTarget = vec3(0.05f, 0.05f, -0.15f);
		
		//TODO3 This is the direction the camera is looking.
		// Update this to be correct. You can use - and + as natural operators on vec3s
		vec3 lookDir = vec3(1.0f);
		lookDir = lookTarget - position;

		// These will be used to build the basis matrices
		vec3 u = vec3(1,0,0);
		vec3 v = vec3(0,1,0);
		vec3 w = vec3(0,0,1);
		vec3 t = vec3(0,0,0);

		//TODO4 Update the UVW basis vectors based on the camera configuration
		// GLM commands that might be useful: cross(), normalize()
		//ADD YOUR CODE HERE
		w = normalize(-lookDir);
		u = normalize(cross(up, w));
		v = normalize(cross(w, u));

		//TODO5 Update t to translate based on camera position
		//ADD YOUR CODE HERE
		t = position;

		// Prepare the columns of the orientation matrix using the UVW vectors
		vec4 r1 = vec4(u,0);
		vec4 r2 = vec4(v,0);
		vec4 r3 = vec4(w,0);
		vec4 r4 = vec4(0,0,0,1);
		
		//TODO6 Prepare the columns of the translation matrix
		// You can negate vectors with natural notation: '-'
		vec4 t1 = vec4(1,0,0,0);
		vec4 t2 = vec4(0,1,0,0);
		vec4 t3 = vec4(0,0,1,0);
		vec4 t4 = vec4(-t,1);
		
		// Create the final camera matrix
		mat4 ident = mat4(1.0f);
		mat4 camOrient = mat4(r1, r2, r3, r4);
		mat4 camTrans   = mat4(t1, t2, t3, t4);

		//TODO7 This is the basic idea, but it is not correct.
		// The glm transpose() function may be helpful
		mat4 camMatrix = transpose(camOrient) * camTrans;
		
		if(!useGlmCam)
		{
			this->cameraMatrix = camMatrix;
		}
		else
		{
			//TODO1 Replace this with a call to the glm function that creates a camera matrix
			this->cameraMatrix = glm::mat4(1.0f);
			this->cameraMatrix = glm::lookAt(position, lookTarget, up);
		}
	}
	
	void toggleCam()
	{
		useGlmCam = !useGlmCam;
	}
};

#endif
