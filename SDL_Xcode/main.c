//
//  main.c
//  SDL_Xcode
//
//  Created by Lucius Kwok on 1/30/24.
//

#include <stdio.h>
#include <SDL2/SDL.h>


// Globals
SDL_Window* window;
SDL_Renderer* renderer;

// Constants
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

int main(int argc, const char * argv[]) {
	printf("SDL_Test is running.\n");
	
	// Set up SDL
	SDL_Init(SDL_INIT_EVERYTHING); //initialize SDL to use all subsystems
	
	// Window
	window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							  WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	// options: SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	//	WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS
	
	// Renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	
	// Clear the window
	SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	
	// Allocate frame buffer
	int pixelCount = WINDOW_WIDTH * WINDOW_HEIGHT;
	uint32_t *pixels = (uint32_t *)malloc(pixelCount * sizeof(uint32_t));
	SDL_Texture* frameBuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
												 WINDOW_WIDTH, WINDOW_HEIGHT);
	for (int i = 0; i < pixelCount; i++) {
		pixels[i] = 0x000000FF;
	}
	
	// Color palette
	uint32_t colorPalette[4] = { 0xFF0000FF, 0xFFFF00FF, 0x00FF00FF, 0x0000FFFF };
	int paletteIndex = 0;
	
	// Game loop
	int didQuit = 0;
	int y = 0;
	while (didQuit == 0) {
		SDL_Event event;
		SDL_PollEvent(&event);
		
		// Keyboard interaction
		switch (event.type) {
			case SDL_QUIT: // when 'X' button is pressed
				// do something
				didQuit = 1;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					didQuit = 1;
					
				}
				break;
		}
		
		// Update frame buffer
		for (int x = 0; x < WINDOW_WIDTH; x++) {
			pixels[y * WINDOW_WIDTH + x] = colorPalette[paletteIndex];
		}
		y++;
		if (y >= WINDOW_HEIGHT) {
			y = 0;
			paletteIndex++;
			if (paletteIndex >= 4) {
				paletteIndex = 0;
			}
		}
		
		// Render frame buffer
		SDL_UpdateTexture(frameBuffer, NULL, pixels, WINDOW_WIDTH * sizeof(uint32_t));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, frameBuffer, NULL, NULL);
		SDL_RenderPresent(renderer);
		
		// Wait for 1/60 second = (1000 / 60)
		SDL_Delay(1000/120);
	}
	
	// Clean up
	free(pixels);
	SDL_DestroyTexture(frameBuffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	printf("SDL_Test is complete.\n");
	return 0;
}

