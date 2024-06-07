#include <iostream>
#include "../libs/cpu.h"

using namespace std;

int main(int argc, char** argv){
    if(argc != 2){
        cout << "Incorrect entry \n Try: <PROGRAM_NAME> <ROM_NAME> instead" << endl;
        exit(1);
    }

    Chip chip(argv[1]);
    unsigned int cpu_start, timers_start;
    int cpu_time, timers_time;

    SDL_Event event;
    bool quit = 0;
    //int count = 0;
    const unsigned char *key_state;


    timers_start = SDL_GetTicks();
    while(quit == 0){
    //while(count < 20){
        //count++;
        SDL_PollEvent(&event);
        key_state = SDL_GetKeyboardState(NULL);
        if(key_state[SDL_SCANCODE_ESCAPE])
            quit = 1;

        cpu_start = SDL_GetTicks();
        chip.run();

        timers_time = SDL_GetTicks() - timers_start;
        chip.draw();

        if(timers_time > TIMERS_DELAY){
            //chip.terminal_draw();
            timers_start = SDL_GetTicks();
            chip.countdown();
        }
        
        cpu_time = SDL_GetTicks() - cpu_start;
        if(CPU_DELAY > cpu_time) SDL_Delay(CPU_DELAY - cpu_time);
    }



    return 0;
}