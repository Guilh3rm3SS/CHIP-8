#include"../libs/cpu.h"
#include <random> 

// NIBBLE[0] ==  0
    void Chip::cls(){//00E0 (clear screen)
        for(int i = 0; i < DISPLAY_HEIGHT; i++){
            for(int j = 0; j < DISPLAY_WIDTH; j++){
                display[i][j] = 0;
            }
        }
    }

    void Chip::rts(){//00EE: (return from subroutine)
        pc = stack[sp-1];
        sp--;
    }

    // NIBBLE[0] == 1
    void Chip::jmp_nnn(){//1NNN: Jump
        pc = (instruction_nibble[1] << 8) | (instruction_nibble[2] << 4) | instruction_nibble[3];
    }

    // NIBBLE[0] ==  2
    void Chip::jrs_nnn(){////2NNN: (jump to subroutine at NNN)
        stack[sp] = pc;
        sp ++;
        pc = (instruction_nibble[1] << 8) | (instruction_nibble[2] << 4) | instruction_nibble[3];
    }

    // NIBBLE[0] ==  3
    void Chip::skeq_vx_nn(){// 3XNN (skip if register X = constant NN)
        if(registers[instruction_nibble[1]] == ((instruction_nibble[2] << 4) | instruction_nibble[3]))
            pc += 2;
    }
    
    // NIBBLE[0] ==  4
    void Chip::skne_vx_nn(){//4XNN (skip if register X != constant NN)
        if(registers[instruction_nibble[1]] != ((instruction_nibble[2] << 4) | instruction_nibble[3]))
            pc += 2;
    }

    // NIBBLE[0] == 5
    void Chip::skeq_vx_vy(){//5XY0 (skip if register X = register Y)
        if(registers[instruction_nibble[1]] == registers[instruction_nibble[2]])
            pc += 2;
    }

    // NIBBLE[0] ==  6
    void Chip::mov_vx_nn(){//6XNN: (set the register VX to the value NN)
        registers[instruction_nibble[1]] = (instruction_nibble[2] << 4) | instruction_nibble[3];
    }

    // NIBBLE[0] == 7
    void Chip::add_vx_nn(){//7XNN (Add the value NN to VX)
        registers[instruction_nibble[1]] += (instruction_nibble[2] << 4) | instruction_nibble[3];
    }

    // NIBBLE[0] == 8
    void Chip::mov_vx_vy(){//8XY0 (VX is set to the value of VY)
        registers[instruction_nibble[1]] = registers[instruction_nibble[2]];
    }

    void Chip::or_vx_vy(){//8XY1 (VX | VY)
        registers[instruction_nibble[1]] = registers[instruction_nibble[1]] | registers[instruction_nibble[2]];
    }

    void Chip::and_vx_vy(){//8XY2 (VX & VY)
        registers[instruction_nibble[1]] = registers[instruction_nibble[1]] & registers[instruction_nibble[2]];
    }

    void Chip::xor_vx_vy(){//8XY3 (VX ^ VY)
        registers[instruction_nibble[1]] = registers[instruction_nibble[1]] ^ registers[instruction_nibble[2]];
    }

    void Chip::add_vx_vy(){//8XY4 (VX + VY)   is used the flag register
        unsigned char sum = registers[instruction_nibble[1]] + registers[instruction_nibble[2]];
        
        if(sum > 0xFF) registers[0XF] = 1;
        else registers[0XF] = 0;

        registers[instruction_nibble[1]] = sum & 0xFF;
    }

    void Chip::sub_vx_vy(){//8XY5 (sets VX to the result of VX - VY)
        registers[0XF] = 0;
        if(registers[instruction_nibble[1]] > registers[instruction_nibble[2]]) 
            registers[0XF] = 1;
        
        registers[instruction_nibble[1]] -= registers[instruction_nibble[2]];
    }

    void Chip::shr_vx(){//8XY6 (shift right) ambiguous!!!
        //registers[instruction_nibble[1]] = registers[instruction_nibble[2]]; //versão antes de 1990s
        registers[0xF] = registers[instruction_nibble[1]] & 0x1;
        registers[instruction_nibble[1]] >>= 1;
    }

    void Chip::rsb_vx_vy(){//8XY7 (sets VX to the result of VY - VX)
        registers[0XF] = 0;
        if(registers[instruction_nibble[2]] > registers[instruction_nibble[1]]) 
            registers[0XF] = 1;
        
        registers[instruction_nibble[1]] = registers[instruction_nibble[2]] - registers[instruction_nibble[1]];
    }

    void Chip::shl_vx(){//8XYE (shift left) ambiguous!!!
        //registers[instruction_nibble[1]] = registers[instruction_nibble[2]]; //versão antes de 1990s
        registers[0xF] = (registers[instruction_nibble[1]] >> 7) & 0x1;
        registers[instruction_nibble[1]] <<= 1;
    }

    // NIBBLE[0] == 9
    void Chip::skne_vx_vy(){//9XY0 (skip if register X != register Y)
        if(registers[instruction_nibble[1]] != registers[instruction_nibble[2]])
            pc += 2;
    }

    // NIBBLE[0] == A
    void Chip::mvi_nnn(){//ANNN (set index register I)
        index = (instruction_nibble[1] << 8) | (instruction_nibble[2] << 4) | instruction_nibble[3];
    }

    // NIBBLE[0] == B
    void Chip::jmi_nnn(){//BXNN (Jump with offset) old: BNNN
        pc = registers[instruction_nibble[1]] + ((instruction_nibble[1] << 8) | (instruction_nibble[2] << 4) | instruction_nibble[3]);
    }

    // NIBBLE[0] == C
    void Chip::rand_vx_nn(){//CXNN (VX = random & NN)
        srand(SDL_GetTicks());
        registers[instruction_nibble[1]] = (rand() % 0xFF) & ((instruction_nibble[2] << 4) | instruction_nibble[3]);
    }

    // NIBBLE[0] == D
    void Chip::sprite_vx_vy_n(){//DXYN (display/draw)
        registers[0xF] = 0;
        unsigned char x_pos = registers[instruction_nibble[1]] % 64;
        unsigned char y_pos = registers[instruction_nibble[2]] % 32;

        for(int i = 0; i < instruction_nibble[3]; i++){
            unsigned char sprite_byte = memory[index + i];
            for (int j = 0; j < 8; j++){
                if(x_pos + j < DISPLAY_WIDTH && y_pos + i < DISPLAY_HEIGHT){
                    bool sprite_bit = (sprite_byte >> (7-j)) & 0x1;
                    
                    if(sprite_bit && display[y_pos + i][x_pos + j])
                        registers[0xF] = 1;
                    
                    display[y_pos + i][x_pos + j] = sprite_bit ^ display[y_pos + i][x_pos + j];
                }
            }
            
        }
        
    }

    // NIBBLE[0] == E
    void Chip::skpr__x(){//EX9E (Skip next instruction if key with the value of Vx is pressed.)
        if(check_key(registers[instruction_nibble[1]]) == 1){
            printf("key %c pressed \n", registers[instruction_nibble[1]]);
            pc += 2;

        }
    }


    void Chip::skup_x(){// EXA1 (Skip next instruction if key with the value of Vx is not pressed.)
        if(check_key(registers[instruction_nibble[1]]) == 0){
            printf("key %c not pressed \n", registers[instruction_nibble[1]]);
            pc += 2;

        }
    }

    // NIBBLE[0] == F
    void Chip::gdelay_vx(){//FX07 (sets VX to the current value of the delay timer)
        registers[instruction_nibble[1]] = delay_timer;
    }

    void Chip::key_vx(){//FX0A (Wait for a key press, store the value of the key in Vx.)
        unsigned char key_pressed = key_press();

        if(key_pressed <= 0xF)registers[instruction_nibble[1]] = key_pressed;
        else pc -=2;
    }

    void Chip::sdelay_vx(){//FX15 (Set delay timer = Vx)
        delay_timer = registers[instruction_nibble[1]];
    }

    void Chip::ssound_vx(){//FX18 (Set sound timer = Vx)
        sound_timer = registers[instruction_nibble[1]];
    }

    void Chip::adivx_vx(){//fX1E (Set I = I + Vx)
        index += registers[instruction_nibble[1]];
    }

    void Chip::font_vx(){//FX29 (Set I = location of sprite for digit Vx)
        index = (registers[instruction_nibble[1]] * 0x5) + 0x50;
        //!!!!!cosmac vip used only the last Vx nibble as the character identifier
    }

    void Chip::bcd_vx(){//FX33 (Store BCD representation of Vx in memory locations I, I+1, and I+2)
        memory[index] = registers[instruction_nibble[1]] / 100;
        memory[index + 1] = (registers[instruction_nibble[1]] % 100) / 10;  
        memory[index + 2] = (registers[instruction_nibble[1]] % 10);
        
    }

    void Chip::str_v0_vx(){//FX55 (Store registers V0 through Vx in memory starting at location I)
        for(int i = 0; i < 0xF;i++){
            memory[index + i] = registers[i];
        }

        //in the old Version, index was modified

    }

    void Chip::ldr_v0_vx(){//FX65 (Read registers V0 through Vx from memory starting at location I)
        for(int i = 0; i < 0xF;i++){
            registers[i] = memory[index + i];
        }

        //in the old Version, index was modified
    }
