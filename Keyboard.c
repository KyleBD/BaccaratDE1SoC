#include <stdio.h>
#include <stdbool.h> // Required for using bool type
#include <stdlib.h>
#include <string.h>
#include <time.h>


int pressedKey;
void read_keyboard() {
    volatile int * PS2_ptr = (int *) 0xFF200100;
	while(1){
	int data = *PS2_ptr;
    int VALID = (data & 0x8000);
    pressedKey = data & 0xFF;

    if (VALID !=0){
        pressedKey = data & 0xFF;
         if(pressedKey == 0x1C){ //A
            printf("left");
        }
        if(pressedKey == 0x1D){ //W
            printf("up");

        }
        if(pressedKey == 0x1B){ //S
            printf("down");

        }   
        if(pressedKey == 0x23){ //D
            printf("right");
        }   
    }
  }
}

int main(){
    read_keyboard();
    return 0;
}