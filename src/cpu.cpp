#include<iostream>
#include "../libs/cpu.h"


unsigned short Chip::fetch(){
    unsigned short instruction;
    instruction = (memory[pc] << 8) | memory[pc + 1];
    pc += 2;
    return instruction;
}

void Chip::instruction_slice(unsigned short instruction){
    instruction_nibble[0] = (instruction >> 12) & 0xF;
    instruction_nibble[1] = (instruction >> 8) & 0xF;
    instruction_nibble[2] = (instruction >> 4) & 0xF;
    instruction_nibble[3] = instruction & 0xF;
}


void Chip::run(){
    unsigned short instruction;
    instruction = fetch();

    instruction_slice(instruction);
    //debug();

    switch(instruction_nibble[0]){
        case 0x0:
            if(instruction == 0x00E0)cls();
            
            else if(instruction == 0x00EE)rts();
            break;

        case 0x1:
            jmp_nnn();
            break;

        case 0x2:
            jrs_nnn();
            break;

        case 0x3:
            skeq_vx_nn();
            break;

        case 0x4:
            skne_vx_nn();
            break;

        case 0x5:
            skeq_vx_vy();
            break;

        case 0x6:
            mov_vx_nn();
            break;

        case 0x7:
            add_vx_nn();
            break;

        case 0x8:
            switch(instruction_nibble[3]){
                case 0x0:
                    mov_vx_vy();
                    break;
                case 0x1:
                    or_vx_vy();
                    break;
                case 0x2:
                    and_vx_vy();
                    break;
                case 0x3:
                    xor_vx_vy();
                    break;
                case 0x4:
                    add_vx_vy();
                    break;
                case 0x5:
                    sub_vx_vy();
                    break;
                case 0x6:
                    shr_vx();
                    break;
                case 0x7:
                    rsb_vx_vy();
                    break;
                case 0xE:
                    shl_vx();
                    break;
            }
            break;

        case 0x9:
            skne_vx_vy();
            break;

        case 0xA:
            mvi_nnn();
            break;

        case 0xB:
            jmi_nnn();
            break;

        case 0xC:
            rand_vx_nn();
            break;

        case 0xD:
            sprite_vx_vy_n();
            break;

        case 0xE:
            switch(instruction_nibble[3]){
                case 0xE:
                    skpr__x();
                    break;
                case 0xA:
                    skup_x();
                    break;
            }
            break;

        case 0xF:
            switch((instruction_nibble[2] << 4) | instruction_nibble[3]){
                case 0x07:
                    gdelay_vx();
                    break;
                case 0x0A:
                    key_vx();
                    break;
                case 0x15:
                    sdelay_vx();
                    break;
                case 0x18:
                    ssound_vx();
                    break;
                case 0x1E:
                    adivx_vx();
                    break;
                case 0x29:
                    font_vx();
                    break;
                case 0x33:
                    bcd_vx();
                    break;
                case 0x55:
                    str_v0_vx();
                    break;
                case 0x65:
                    ldr_v0_vx();
                    break;
            }
            break;
    }
}

void Chip::countdown(){
    if(delay_timer > 0) delay_timer --;
    if(sound_timer > 0) sound_timer --;
}



void Chip::debug(){
    std::cout << "\ninstruction: "<< std::hex << static_cast<unsigned>(instruction_nibble[0]) 
    << std::hex << static_cast<unsigned>(instruction_nibble[1]) << std::hex << static_cast<unsigned>(instruction_nibble[2]) 
    << std::hex << static_cast<unsigned>(instruction_nibble[3]) << " index: " << std::hex << index << " pc: " << std::hex << (pc - 2) << std::endl;
    
    for(int i = 0; i < 0x10; i++)
        std::cout << std::hex << i << ": "  << std::hex << static_cast<unsigned>(registers[i])<< "| ";
    std::cout << std::endl << std::endl;
}

void Chip::terminal_draw(){
    //system("clear");
    for(int i = 0; i < DISPLAY_HEIGHT;i++){
            for(int j = 0; j < DISPLAY_WIDTH; j++){
                switch(display[i][j]){
                    case 0:
                        printf(".");
                        break;
                    case 1:
                        printf("■");
                        break;
                }
            }
            printf("\n");
        }
}