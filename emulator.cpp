#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "cpu.h"
#include <SDL.h>

void drawGraphics(SDL_Renderer* renderer){
    //goal of this function is to read the graphics[] array and display it with SDL

    SDL_SetRenderDrawColor(renderer,0,0,0,255); //set the draw color to black. the 255 controls opacity
    SDL_RenderClear(renderer); //clear the frame with black

    SDL_SetRenderDrawColor(renderer,255,255,255,255); //set it to white
    
    for(int i=0; i<32;i++){
        for(int j=0; j<64;j++){  //this nested loop traverses the array from left to right, then next row down.
            int pixIndex = i * 64 + j; //gets column, * by 64 to find what row its in, then + the j which is the x value.
            
            if(graphics[pixIndex] == 1){
                SDL_Rect pixel = {j*10,i*10,10,10}; // creates a rectangle size 10 by 10 pixels.
                                                    //starts at j*10 and i*10 because pixels will overlap if not evenly space by 10.
                SDL_RenderFillRect(renderer, &pixel); //& gives the address of pixel, so it can read its pos and dimensions
            } 

        }
    }
    SDL_RenderPresent(renderer); //presents/displays the frame

}

SDL_Keycode keyMap[16] = { //keymap for the key pad. refer to the keypad layout if confused
    SDLK_x, SDLK_1, SDLK_2, SDLK_3,
    SDLK_q, SDLK_w, SDLK_e, SDLK_a,
    SDLK_s, SDLK_d, SDLK_z, SDLK_c,
    SDLK_4, SDLK_r, SDLK_f, SDLK_v
};

int main(int argc, char **argv){
    
    //setup cpu and input
    initCpu();

    //if no ROM file is provided.
    if(argc < 2){
        printf("Please provide a file to run. \n");
        return 1;
    }

    //setup SDL video
    SDL_SetMainReady();
    
    //SDL_Init(SDL_INIT_VIDEO) setsup video subsystem
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Couldn't Initialize SDL Video %s\n", SDL_GetError());
        return 1; //0 means success but return 1 means failure.
    }

    SDL_Window* window1 = SDL_CreateWindow( //creates window with parameters
    "Chip-8 Emulator",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    640,
    320,
    SDL_WINDOW_SHOWN);

    if(window1 == nullptr){
        printf("Window could not initialize: %s\n", SDL_GetError());
        return 1;
    }
    //this is the renderer referenced in the render graphics function parameter.
    SDL_Renderer* renderer = SDL_CreateRenderer(window1, -1, SDL_RENDERER_ACCELERATED); 
    if(renderer == nullptr){
        printf("Renderer could not initialize: %s\n", SDL_GetError());
        return 1;
    }

    //load the file
    loadGame(argv[1]);

    bool running = true;
    SDL_Event event;

    //emulate the file
    const double cpuFrequency = 500.0;
    const double cycleTime = 1000/cpuFrequency;
    
    uint32_t lastTimerUpd = SDL_GetTicks(); //initialize a timer variable

    while(running){ 
        uint32_t start = SDL_GetTicks(); //gets the ms since start up.
        
        while(SDL_PollEvent(&event)){ //gives memory address of the event
            if(event.type == SDL_QUIT){
                running = false;
            }else if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){
                for(int i=0; i<16; i++){
                    if(event.key.keysym.sym == keyMap[i]){
                        key[i] = (event.type == SDL_KEYDOWN); //this argument returns key[i] to 1

                        if(event.type == SDL_KEYDOWN){
                            printf("CHIP-8 key %X pressed\n", i);
                        }
                        break;
                    }

                    
                }
            }

        }

        emulateCycle();
        uint32_t currentTime = SDL_GetTicks(); //gets current time in ms
        if(currentTime - lastTimerUpd >=16){ //16ms converts to 60 hz
            if(dT > 0){
                dT--;
            }
            if(sT > 0){
                sT--;
            }
            lastTimerUpd = currentTime;
        }
        

        drawGraphics(renderer);

        uint32_t elapsed = SDL_GetTicks() - start;

        if(elapsed < cycleTime){ //if the time that has passed is less than the set cycle time then delay until the cycle time
            SDL_Delay(cycleTime - elapsed);
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window1);
    SDL_Quit();
    return 0;
}