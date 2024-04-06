#include <stdio.h>
#include <stdbool.h> // Required for using bool type
#include <stdlib.h>
#include <string.h>
#include <time.h>

int xStep;
int yStep;
int pressedKey;
int previousKey;
void read_keyboard() {
    //volatile int * PS2_ptr = (int *) 0xFF200100;

	volatile int * PS2_ptr = (int *) 0xFF200100;
	int data = *PS2_ptr;
    int VALID = (data & 0x8000);
    pressedKey = data & 0xFF;

    if (VALID !=0){
        pressedKey = data & 0xFF;
        if(pressedKey == 0xF0){
            printf("break");
            xStep = 0;
            yStep = 0;
            previousKey = pressedKey;
        }
        else if(pressedKey == 0x1C && previousKey != 0xF0){ //A
            printf("left");
            xStep = -1;
            previousKey = pressedKey;
        }
        else if(pressedKey == 0x1D && previousKey != 0xF0){ //W
            printf("up");
            yStep = -1;
            pressedKey = pressedKey;

        }
        else if(pressedKey == 0x1B && previousKey != 0xF0){ //S
            printf("down");
            yStep = 1;
            previousKey = pressedKey;

        }   
        else if(pressedKey == 0x23 && previousKey != 0xF0){ //D
            printf("right");
            xStep = 1;
            previousKey = pressedKey;
        }   
        else if(pressedKey == 0x5A && previousKey != 0xF0){ //D
            printf("enter");
            previousKey = pressedKey;
        }   
        else{
            previousKey = 0;
        }
    }
}

int pixel_buffer_start; // global variable
#include <stdbool.h>
#include <stdlib.h>

short int Buffer1[240][512]; // 240 rows, 512 (320 + padding) columns
short int Buffer2[240][512];
int boxSize = 8;
short int line_colours[10] = {0xFFFF,0x0001,0x07E0, 0x3456, 0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF};

void plot_pixel(int x, int y, short int line_color);

void getRand(int xCoord[8], int yCoord[8], int xStep[8], int yStep[8], short int line_colour[10]){
    for (int i = 0; i < 8; i++){
        xCoord[i] = rand() % 317;
        yCoord[i] = rand() % 237;
        xStep[i] = ((rand() % 2) * 2) - 1;
        yStep[i] = ((rand() % 2) * 2) - 1;

        line_colour[i] = line_colours[rand() % 10];
    }
}

void swap( int * x, int * y){
    int i = *x;
    *x = *y;
    *y = i;
}




void plot_box(int startX, int startY, short int boxColour) {
    for (int i = 0; i < boxSize; i++){
        for(int j =0; j < boxSize; j++){
            plot_pixel(startX, startY, boxColour);
            startX = startX + 1;
        }
        startY = startY + 1;
    }
}

void wait_for_vsync()
{
    volatile int * pixel_ctrl_ptr = (int *) 0xff203020; // base address
    int status;
    *pixel_ctrl_ptr = 1; // start the synchronization process
    // write 1 into front buffer address register
    status = *(pixel_ctrl_ptr + 3); // read the status register
    while ((status & 0x01) != 0) // polling loop waiting for S bit to go to 0
    {
        status = *(pixel_ctrl_ptr + 3);
    } // loop/function exits when status bit goes to 0
}

void plot_pixel(int x, int y, short int line_color)
{
    volatile short int *one_pixel_address;

        one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);

        *one_pixel_address = line_color;
}


void clear_screen() {
    int x, y;
    for (x = 0; x < 320; x++) {
        for (y = 0; y < 240; y++) {
            plot_pixel(x, y, 0); // Set pixel color to black (0x0000)
        }
    }
}

void draw_box(int xCoord, int yCoord, int line_colour){
    plot_pixel(xCoord, yCoord, line_colour);
    plot_pixel(xCoord + 1, yCoord, line_colour);
    plot_pixel(xCoord, yCoord + 1, line_colour);
    plot_pixel(xCoord + 1, yCoord + 1, line_colour);
}
int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables(not shown)
    int xCoord;
    int yCoord;

    short int line_colour = 0xFFE0;
    // initialize location and direction of rectangles(not shown)

    /* set front pixel buffer to Buffer 1 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer1; // first store the address in the  back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer

    /* set back pixel buffer to Buffer 2 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer2;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen(); // pixel_buffer_start points to the pixel buffer

    xCoord = 0;
    yCoord = 0;
    xStep = 0;
    yStep = 0;

    while (1)
    {
        clear_screen();
        read_keyboard();
        draw_box(xCoord, yCoord, line_colour);

        //printf("%d", xStep);
        xCoord = xCoord + xStep;
        yCoord = yCoord + yStep;

        if(yCoord == 239){
            yCoord = 237;
            yStep = 0;

        }
        else if(yCoord == 0){
            yCoord = 0;
            yStep = 0;
        }
        if(xCoord == 239){
            xCoord = 237;
            xStep = 0;

        }
        else if(xCoord == 0){
            xCoord = 0;
            xStep = 0;
        }

        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
}