#ifndef CPU_H
#define CPU_H

#include<string>
#include <SDL2/SDL.h>
#include"constants.h"
//timing = 700 instructions per second


class Chip{
private:
    unsigned char registers[16];
    unsigned short pc, index, sp;
    unsigned char delay_timer, sound_timer;

    unsigned short stack[16];
    unsigned char memory[MEMORY_SIZE];

    bool display[DISPLAY_HEIGHT][DISPLAY_WIDTH];
    bool last_frame_keys[16];
    
    unsigned char instruction_nibble[4];


    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    Chip(std::string);
    ~Chip();

    void draw();

    unsigned short fetch();
    void run();
    void instruction_slice(unsigned short instruction);
    void countdown();//todo

    bool check_key(unsigned char key);
    unsigned char key_press(); 

    void debug();
    void terminal_draw();



//Instructions

    // NIBBLE[0] ==  0
    void cls();
    void rts();

    // NIBBLE[0] == 1
    void jmp_nnn();

    // NIBBLE[0] ==  2
    void jrs_nnn();

    // NIBBLE[0] ==  3
    void skeq_vx_nn();
    
    // NIBBLE[0] ==  4
    void skne_vx_nn();

    // NIBBLE[0] == 5
    void skeq_vx_vy();

    // NIBBLE[0] ==  6
    void mov_vx_nn();

    // NIBBLE[0] == 7
    void add_vx_nn();

    // NIBBLE[0] == 8
    void mov_vx_vy();
    void or_vx_vy();
    void and_vx_vy();
    void xor_vx_vy();
    void add_vx_vy();
    void sub_vx_vy();
    void shr_vx();
    void rsb_vx_vy();
    void shl_vx();

    // NIBBLE[0] == 9
    void skne_vx_vy();

    // NIBBLE[0] == A
    void mvi_nnn();

    // NIBBLE[0] == B
    void jmi_nnn();

    // NIBBLE[0] == C
    void rand_vx_nn();

    // NIBBLE[0] == D
    void sprite_vx_vy_n();

    // NIBBLE[0] == E
    void skpr__x();
    void skup_x();

    // NIBBLE[0] == F
    void gdelay_vx();
    void key_vx();
    void sdelay_vx();
    void ssound_vx();
    void adivx_vx();
    void font_vx();
    void bcd_vx();
    void str_v0_vx();
    void ldr_v0_vx();

     
};

#endif
