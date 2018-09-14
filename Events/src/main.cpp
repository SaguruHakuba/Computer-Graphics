#define USER1 "put_username1_here"
#define USER2 "put_username2_here"

#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <fcntl.h>
#include <unistd.h>
#endif

#include <SFML/Window.hpp>
#include "GraphicsHelpers.h"

//TODO1: Read the capabilities of the WorldState class. You will need to call
//some of its methods.
//I strongly recommend referring to the SFML event tutorial for this project 
//http://www.sfml-dev.org/tutorials/2.5/window-events.php
#include "WorldState.h"

class GLBox
{
private:
	void handleEvents(sf::Window & window, WorldState & state)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window : exit
			if (event.type == sf::Event::Closed)
				state.setRunning(false);
			
			// Escape key : exit
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				state.setRunning(false);
			
			//TODO2: for text event with code in the range [' ', 'z'], have the state print the unicode character
			//Remember that you can use characters as numbers to test ranges (i.e. i < 'a')
			//Use the print methods in the state object to print the events
			if ((event.type == sf::Event::TextEntered) && (event.text.unicode >= ' ') && (event.text.unicode <= 'z'))
				state.printText(event.text.unicode);

			
			//TODO3: if the mouse moves, have the state print the cursor position
			if (event.type == sf::Event::MouseMoved){
				state.printMousePos(event.mouseMove.x, event.mouseMove.y);
			}

		}

		//TODO4: if any arrow keys are down, tell the state to 'move' in that direction
		//This should not use events! It should check the current state of the key each frame
		//Check the tutorial on input state:
		//http://www.sfml-dev.org/tutorials/2.4/window-inputs.php
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			state.moveDown();
		}
		

		//TODO5: do the todos in the run loop
	}
	
public:
	GLBox()
	{
		getWindowContext();
		//you can add variables here if you need
		sf::Clock clock;
		sf::Clock clock2;
		sf::Time current = clock.getElapsedTime();

		WorldState state;
		graphicsInit();
		
		// Start render loop
		while (state.isRunning())
		{
			// Handle any events that are in the queue
			this->handleEvents(*window, state);
			
			//TODO6: track the time since last render and then update the frametime in the state
			//You will want to use sf::Clock to help with this
			//A tutorial on handling SFML time:
			//http://www.sfml-dev.org/tutorials/2.4/system-time.php
			
			sf::Time oldtime = current;
			current = clock.getElapsedTime(); //get time elapsed and restart timer
			sf::Time frametime = current - oldtime; //can do math ops on time
			state.updateFrameTime(frametime.asSeconds());
			
			//TODO7: every 2.0 seconds, have the state print the fps
			// You might want to track the total runtime of the program,
			if (clock2.getElapsedTime().asSeconds() >= 2) {
				clock2.restart();
				state.printFPS();
			}
            
            
			display();
			window->display();
		}
		window->close();
	}
	
	~GLBox()
	{
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
		
		float ver = getVer();
		if( ver < 1.0f ) {
			printf("OpenGL is not supported.\n");
			exit(1);
		}
		printf("OpenGL version %.1f is supported.\n", ver);
	}
	
	sf::Window *window;
};

int main()
{
	//printf("Program by %s+%s\n", USER1, USER2);
	GLBox prog;
	
    return EXIT_SUCCESS;
}