#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include "chip8.h"

using namespace std;

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
int modifier = 16;
int display_width = SCREEN_WIDTH * modifier;
int display_height = SCREEN_HEIGHT * modifier;

chip8 myChip8;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

// Chip8 Keypad
uint8_t keymap[16] = {
	SDLK_x,
	SDLK_1,
	SDLK_2,
	SDLK_3,
	SDLK_q,
	SDLK_w,
	SDLK_e,
	SDLK_a,
	SDLK_s,
	SDLK_d,
	SDLK_z,
	SDLK_c,
	SDLK_4,
	SDLK_r,
	SDLK_f,
	SDLK_v,
};

void init_SDL();
void SDL_Key(SDL_Event windowEvent);


int main(int argc, char **argv){
    if(argc<2){
        fprintf(stderr,"Error: Incorrect File Usage\nUsage: Chip8.exe Filename\n");
        exit(0);
    }

    // Set up render system and register input callbacks
    init_SDL();
    SDL_Event windowEvent;

    // Initialize the Chip8 system and load the game into the memory
    if(myChip8.loadApplication(argv[1]) == 0){
        fprintf(stderr,"Error: Could not process File\n");
        exit(0);
    }

    // Emulation loop
    for(;;){
        // Emulate one cycle
        myChip8.emulateCycle();

        // If the draw flag is set, update the screen
        if(myChip8.drawFlag){
            // Clear screen
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Set the draw color to white
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            // Calculate the width and height of each pixel on the screen
            int pixelWidth = 16;
            int pixelHeight = 16;


            // Draw each pixel on the screen based on the graphics array
            for (int y = 0; y < 32; y++){

                for (int x = 0; x < 64; x++){
                    if (myChip8.gfx[x+y*64]){

                        // Render a filled rectangle for an "on" pixel
                        SDL_Rect pixelRect = {x * pixelWidth, y * pixelHeight, pixelWidth, pixelHeight};
                        SDL_RenderFillRect(renderer, &pixelRect);
                    }
                }
            }

            // Update the screen
            SDL_RenderPresent(renderer);
            myChip8.drawFlag = false;
        }

        while(SDL_PollEvent(&windowEvent)){//proccess keypress
            SDL_Key(windowEvent);
        } 
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit( );
    return 0;
}

/// @brief Setup SDL Environment
void init_SDL(){
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, display_width, display_height, 0);

    if (window == NULL){
        std::cout << "Could not create window: " << SDL_GetError( ) << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    if (renderer == NULL){
        std::cout << "Could not create rednerer: " << SDL_GetError( ) << std::endl;
        return;
    }
}

/// @brief Process keypress and store id of key into Chip8 object
/// @param windowEvent 
void SDL_Key(SDL_Event windowEvent){
    if (windowEvent.type == SDL_QUIT){
        exit(0);
    }
    if (windowEvent.type == SDL_KEYDOWN) {
        if (windowEvent.key.keysym.sym == SDLK_ESCAPE){
            exit(0);
        }
        if (windowEvent.key.keysym.sym == SDLK_SPACE){
            myChip8.emulateCycle();
        }

        //check for keypress and associated ID in keymap
        for(int i=0;i<16;i++){
            if(windowEvent.key.keysym.sym == keymap[i]){
                myChip8.key[i] = 1;
            }
        }
    }
    if (windowEvent.type == SDL_KEYUP) {
        //check for keypress and associated ID in keymap
        for(int i=0;i<16;i++){
            if(windowEvent.key.keysym.sym == keymap[i]){
                myChip8.key[i] = 0;
            }
        }
    }
}

