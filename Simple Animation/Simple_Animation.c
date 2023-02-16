
	
	/* This files provides address values that exist in the system */

#define SDRAM_BASE            0xC0000000
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_CHAR_BASE        0xC9000000

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define TIMER_BASE            0xFF202000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030

/* VGA colors */
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00

#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

/* Constants for animation */
#define BOX_LEN 2
#define NUM_BOXES 8

#define FALSE 0
#define TRUE 1

#include <stdlib.h>
#include <stdio.h>

// Begin part3.c code for Lab 7
volatile int pixel_buffer_start; // global variable

void wait_for_vsync();
void plot_pixel(int x, int y, short int line_color);
void draw_line(int x_start, int y_start, int x_finish, int y_finish,short int line_color);

void draw_points(int x_loc, int y_loc, short int line_color,int size);

void swap(int* x, int* y);
int round(double x);
void clear_screen();


int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
	int size = 1;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)
	int x_position[8];
	int y_position[8];
	
	int x_old[8] = {0,0,0,0,0,0,0,0};
	int y_old[8] = {0,0,0,0,0,0,0,0};
	
	for(int i = 0; i < 8; i++){
		x_position[i] = rand()%(320 - size);
		y_position[i] = rand()%(240 - size);
	}                                             // random location
	
	short int colour_array[10] = {0xFFFF,0xFFE0,0xF800,0x07E0,0x001F,0x07FF,0xF81F,0xC618,0xFC18,0xFC00};
	short int colour_current[8];
	
	for(int i =0 ; i<8; i++){
		colour_current[i] = colour_array[rand()%10];
	}                                             //random colour

	int direction_array[2] = {1,-1};
	int x_direction_curr[8];
	int y_direction_curr[8];
	for(int i =0 ; i<8; i++){
		x_direction_curr[i] = direction_array[rand()%2];
		y_direction_curr[i] = direction_array[rand()%2];
	}  											//random direction
	
	
    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen(); // pixel_buffer_start points to the pixel buffer

    while (1)
    {
        /* Erase any boxes and lines that were drawn in the last iteration */
	
	          
		
        for(int i = 0; i < 7; i++){
		draw_line(x_old[i], y_old[i], x_old[i+1], y_old[i+1],0x0000);
		draw_sqr(x_old[i], y_old[i], 0x0000, size);
		}
		draw_line(x_old[7], y_old[7], x_old[0], y_old[0],0x0000);
		draw_sqr(x_old[7], y_old[7],0x0000,size);
		

		  		// code for updating the locations of boxes (not shown)
		
		for(int i = 0; i < 8; i++){
 			x_old[i] = x_position[i] - x_direction_curr[i];
			y_old[i] = y_position[i] - y_direction_curr[i];
		}
		
		 // code for drawing the boxes and lines (not shown)
		
		for(int i =0 ; i<8; i++){
			colour_current[i] = colour_array[rand()%10];
		}  
		
		for(int i = 0; i < 7; i++){
		draw_line(x_position[i], y_position[i], x_position[i+1], y_position[i+1],colour_current[i]);
		draw_sqr(x_position[i], y_position[i], colour_current[i],size);
		}
		draw_line(x_position[7], y_position[7], x_position[0], y_position[0],colour_current[7]);
		draw_sqr(x_position[7], y_position[7], colour_current[7],size);
		
		
		for(int i = 0; i<8;i++){
			if(x_position[i]+ (size - 1) == 319 || x_position[i] == 0){
				x_direction_curr[i] = -x_direction_curr[i];
			}
			if(y_position[i]+ (size - 1) == 239 || y_position[i] == 0){
				y_direction_curr[i] = -y_direction_curr[i];
			}
			x_position[i] = x_position[i] + x_direction_curr[i];
			y_position[i] = y_position[i] + y_direction_curr[i];
		}
        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
}



// code for subroutines (not shown)

void wait_for_vsync(){
	volatile int* pixel_ctrl_ptr = (int*)0xff203020;
	int status;
	*pixel_ctrl_ptr = 1;
	status = *(pixel_ctrl_ptr + 3);
	
	while((status & 0x01) != 0){
		status = *(pixel_ctrl_ptr +3);
	}
}

void plot_pixel(int x, int y, short int line_color){
	    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}


void draw_sqr(int x_loc, int y_loc, short int line_color, int size){
		for(int i = 0; i < size; i++){
			for(int j = 0; j < size;j++){
				plot_pixel(x_loc +i , y_loc + j , line_color);
			}
		}
}





void draw_line(int x_start, int y_start, int x_finish, int y_finish,short int line_color){
	double y;
	int y_pos;
	double x;
	int x_pos;
	double y_dis;
	double x_dis;
	double slope;
	if(abs(x_finish - x_start) >= abs(y_finish-y_start)){
		if(x_start > x_finish){
			swap(&x_start, &x_finish);
			swap(&y_start, &y_finish);
		}
		
		y_dis = y_finish - y_start;
		x_dis = x_finish - x_start;
		slope = y_dis/x_dis;
			for(int i = x_start; i < x_finish; i++){
				y = y_start +slope* (i - x_start);
				y_pos = round(y);
				
				plot_pixel(i, y_pos, line_color);
			}
	}

	else{
		if(y_start > y_finish){
			swap(&x_start, &x_finish);
			swap(&y_start, &y_finish);
		}
		y_dis = y_finish - y_start;
		x_dis = x_finish - x_start;
		slope = x_dis/y_dis;
			for(int i = y_start; i <= y_finish; i++){
				x = x_start +slope* (i - y_start);
				x_pos = round(x);
				plot_pixel(x_pos, i, line_color);
			}
	}
}

void swap(int* x, int*y){
	int temp = *x;
	*x = *y;
	*y = temp;
}

int round(double x){
	if(x >= 0){
		int y = (int)(x+0.5);
		return y;
	}
	else{
		int y = (int)(x- 0.5);
		return y;
	}
}

void clear_screen(){
	int x,y;
	for (x = 0; x < 320; x++ ){
		for(y = 0;y < 240; y++){
			plot_pixel(x,y,0);
		}
	}

}



