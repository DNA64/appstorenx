#if defined(NOGUI)
	#include "console/Menu.hpp"
	#include "console/Input.hpp"
#else
	#include "gui/MainDisplay.hpp"
#endif

#include "libs/get/src/Utils.hpp"
#include "libs/get/src/Get.hpp"

#if defined(SWITCH)
	#include <switch.h>
#endif

int main(int argc, char *argv[])
{
//	consoleDebugInit(debugDevice_SVC);
//	stdout = stderr; // for yuzu

#if defined(NOGUI)
	// if NOGUI variable defined, use the console's main method
	int console_main(void);
	return console_main();
#else
	init_networking();

	#if defined(SWITCH)
		socketInitializeDefault();
	#endif

	// create main get object
	Get* get = new Get("./.get/", "http://switchbru.com/appstore");

	// initialize main title screen
	MainDisplay* display = new MainDisplay(get);

	// the main inuput handler
	InputEvents* events = new InputEvents();

	bool running = true;
	while (running)
	{
		// get any new input events
		events->update();

		// wait for the remainder of one frame
		events->delay();

		// process the inputs of the supplied event
		display->process(events);

		// draw the display
		display->render(NULL);

		// quit on enter/start
		if (events->held(START_BUTTON))
			running = false;

		// if (event.type == SDL_KEYDOWN)
		// 	SDL_Delay(32); // wait for a bit if we saw key input
	}

	IMG_Quit();
	TTF_Quit();

	SDL_Delay(10);
	SDL_DestroyWindow(display->window);

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();

	#if defined(SWITCH)
		socketExit();
	#endif

	return 0;
#endif
}
