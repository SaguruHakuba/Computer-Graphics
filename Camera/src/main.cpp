#define USER1 "put_username1_here"
#define USER2 "put_username2_here"

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <fcntl.h>
#include <unistd.h>
#endif

#include "gl3w/gl3w.h"

#include <SFML/Window.hpp>
#include "simplePNG.h"
#include "ShaderManager.h"
#include "WorldState.h"
#include "GLHelper.h"
#include "RenderEngine.h"

#define RESOLUTION 512

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
private:
	RenderEngine engine;
	
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

			if ((event.type == sf::Event::TextEntered) && (event.text.unicode == ' '))
				state.toggleCam();
		}
	}
	
	void renderLoop(sf::Window & window)
	{
		sf::Clock time;
		WorldState state;
		
		while (state.isRunning())
		{
			this->handleEvents(window, state);
			state.timeStep( time.getElapsedTime().asSeconds() );
			state.buildCameraMatrix();
			engine.display(state);
			window.display();
		}
		window.close();
	}
	
public:
	GLBox()
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
		sf::ContextSettings settings(0, 0, 0, 3, 3, sf::ContextSettings::Core);
#endif
		sf::Window window(mode, "glver", sf::Style::Default, settings);
		
#ifdef __APPLE__
		dup2(oldFD, 2); // Redirect back
		close(oldFD); // Not needed anymore
#endif
		
		renderLoop(window);
	}
};

int main()
{
	GLBox prog;
	
    return EXIT_SUCCESS;
}
