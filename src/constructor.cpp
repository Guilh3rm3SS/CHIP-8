//PROGRAM START AT 0x200
//FONT IS 0x050–0x09F
#include"../libs/cpu.h"
#include<iostream>
#include<fstream>

Chip::Chip(std::string filename){
    unsigned char initial_memory[80] = {// INSERT THE FONT VALUES ON MEMORY
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // Copy the bytes to the memory array
    for (int i = 0; i < 80; ++i) {
        memory[0x050 + i] = initial_memory[i];
    }

     // Load the binary file into memory starting at 0x200
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        exit(1);
    }

    std::streamsize size = file.tellg();
    if (size > (MEMORY_SIZE - 0x200)) {
        std::cerr << "File too large to fit in memory: " << filename << std::endl;
        file.close();
        exit(1);
    }

    file.seekg(0, std::ios::beg);
    if (!file.read(reinterpret_cast<char*>(&memory[0x200]), size)) {
        std::cerr << "Failed to read file: " << filename << std::endl;
        exit(1);
    }

    file.close();

    pc = 0x200;
    sp = 0;
    delay_timer = 0;
    sound_timer = 0;
    cls();
    for(int i = 0; i < 0xF; i++){
        last_frame_keys[i] = 0;
        registers[i] = 0;
    }

    //Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Chip-8 Display", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH * 10, DISPLAY_HEIGHT * 10, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    
}

Chip::~Chip(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}