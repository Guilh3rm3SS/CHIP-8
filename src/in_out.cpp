#include<iostream>
#include "../libs/cpu.h"

//DISPLAY
void Chip::draw() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White

    int pixelWidth = 10;
    int pixelHeight = 10;

    for (int i = 0; i < DISPLAY_HEIGHT; i++) {
        for (int j = 0; j < DISPLAY_WIDTH; j++) {
            if (display[i][j]) {
                SDL_Rect rect;
                rect.x = j * pixelWidth;
                rect.y = i * pixelHeight;
                rect.w = pixelWidth;
                rect.h = pixelHeight;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}


//KEYPAD
bool Chip::check_key(unsigned char key){
    const unsigned char* keyboard_state = SDL_GetKeyboardState(NULL);
    switch(key){
        case 0x0: return keyboard_state[SDL_SCANCODE_X];
        case 0x1: return keyboard_state[SDL_SCANCODE_1];
        case 0x2: return keyboard_state[SDL_SCANCODE_2];
        case 0x3: return keyboard_state[SDL_SCANCODE_3];
        case 0x4: return keyboard_state[SDL_SCANCODE_Q];
        case 0x5: return keyboard_state[SDL_SCANCODE_W];
        case 0x6: return keyboard_state[SDL_SCANCODE_E];
        case 0x7: return keyboard_state[SDL_SCANCODE_A];
        case 0x8: return keyboard_state[SDL_SCANCODE_S];
        case 0x9: return keyboard_state[SDL_SCANCODE_D];
        case 0xA: return keyboard_state[SDL_SCANCODE_Z];
        case 0xB: return keyboard_state[SDL_SCANCODE_C];
        case 0xC: return keyboard_state[SDL_SCANCODE_4];
        case 0xD: return keyboard_state[SDL_SCANCODE_R];
        case 0xE: return keyboard_state[SDL_SCANCODE_F];
        case 0xF: return keyboard_state[SDL_SCANCODE_V];
    }
    return 0;
}

unsigned char Chip::key_press() {// return the value of the key pressed in the keyboard converted to the chip-8 key
    //SDL_Event event;
    unsigned char key_pressed = 0x10;
    const unsigned char* keyboard_state = SDL_GetKeyboardState(NULL);
    //SDL_PollEvent(&event);

   // if(event.type == SDL_KEYUP){
      if(keyboard_state[SDL_SCANCODE_1] && (last_frame_keys[0x1] == 0)) key_pressed = 0x1;
      if(keyboard_state[SDL_SCANCODE_2] && (last_frame_keys[0x2] == 0)) key_pressed = 0x2;
      if(keyboard_state[SDL_SCANCODE_3] && (last_frame_keys[0x3] == 0)) key_pressed = 0x3;
      if(keyboard_state[SDL_SCANCODE_4] && (last_frame_keys[0xC] == 0)) key_pressed = 0xC;
      if(keyboard_state[SDL_SCANCODE_Q] && (last_frame_keys[0x4] == 0)) key_pressed = 0x4;
      if(keyboard_state[SDL_SCANCODE_W] && (last_frame_keys[0x5] == 0)) key_pressed = 0x5;
      if(keyboard_state[SDL_SCANCODE_E] && (last_frame_keys[0x6] == 0)) key_pressed = 0x6;
      if(keyboard_state[SDL_SCANCODE_R] && (last_frame_keys[0xD] == 0)) key_pressed = 0xD;
      if(keyboard_state[SDL_SCANCODE_A] && (last_frame_keys[0x7] == 0)) key_pressed = 0x7;
      if(keyboard_state[SDL_SCANCODE_S] && (last_frame_keys[0x8] == 0)) key_pressed = 0x8;
      if(keyboard_state[SDL_SCANCODE_D] && (last_frame_keys[0x9] == 0)) key_pressed = 0x9;
      if(keyboard_state[SDL_SCANCODE_F] && (last_frame_keys[0xE] == 0)) key_pressed = 0xE;
      if(keyboard_state[SDL_SCANCODE_Z] && (last_frame_keys[0xA] == 0)) key_pressed = 0xA;
      if(keyboard_state[SDL_SCANCODE_X] && (last_frame_keys[0x0] == 0)) key_pressed = 0x0;
      if(keyboard_state[SDL_SCANCODE_C] && (last_frame_keys[0xB] == 0)) key_pressed = 0xB;
      if(keyboard_state[SDL_SCANCODE_V] && (last_frame_keys[0xF] == 0)) key_pressed = 0xF;


    //}
    
    last_frame_keys[0x0] = keyboard_state[SDL_SCANCODE_X];
    last_frame_keys[0x1] = keyboard_state[SDL_SCANCODE_1];
    last_frame_keys[0x2] = keyboard_state[SDL_SCANCODE_2];
    last_frame_keys[0x3] = keyboard_state[SDL_SCANCODE_3];
    last_frame_keys[0x4] = keyboard_state[SDL_SCANCODE_Q];
    last_frame_keys[0x5] = keyboard_state[SDL_SCANCODE_W];
    last_frame_keys[0x6] = keyboard_state[SDL_SCANCODE_E];
    last_frame_keys[0x7] = keyboard_state[SDL_SCANCODE_A];
    last_frame_keys[0x8] = keyboard_state[SDL_SCANCODE_S];
    last_frame_keys[0x9] = keyboard_state[SDL_SCANCODE_D];
    last_frame_keys[0xA] = keyboard_state[SDL_SCANCODE_Z];
    last_frame_keys[0xB] = keyboard_state[SDL_SCANCODE_C];
    last_frame_keys[0xC] = keyboard_state[SDL_SCANCODE_4];
    last_frame_keys[0xD] = keyboard_state[SDL_SCANCODE_R];
    last_frame_keys[0xE] = keyboard_state[SDL_SCANCODE_F];
    last_frame_keys[0xF] = keyboard_state[SDL_SCANCODE_V];
    
    return key_pressed;
}


//AUDIO
