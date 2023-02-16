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

// Begin part1.s for Lab 7

	
volatile int pixel_buffer_start; // global variable
void plot_pixel(int x, int y, short int line_color);
void draw_line(int x_start, int y_start, int x_finish, int y_finish,short int line_color);
void swap(int* x, int* y);
int round(double x);
void clear_screen();
void delay();


int main(void)
{

    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;

    clear_screen();
	int x_start = 100;
	int y_start = 0;
	int x_finish = 200;
	int y_finish = 0;
	int direction = 0; // if direction == 0, goes diwn. Otherwise goes up
	
	
	//draw_line(x_start, y_start, x_finish, y_finish, 0xFFFF);   // this line is blue
	while(1){
		if(direction == 0){
			while(y_start < 239){
				delay();
				draw_line(x_start, y_start, x_finish, y_finish, 0x0000);//erase the old line
				
				y_start = y_start+1;
				y_finish = y_finish+1;
				draw_line(x_start, y_start, x_finish, y_finish, 0xFFFF); // draw the new line
			}
		} 
			
		if(y_start == 239){
			direction = 1;
		}
			
		if(direction == 1){
			while(y_start > 0){
				delay();
				draw_line(x_start, y_start, x_finish, y_finish, 0x0000);//erase the old line
				y_start = y_start-1;
				y_finish = y_finish-1;
				draw_line(x_start, y_start, x_finish, y_finish, 0xFFFF);
			}
		}
			
			if(y_start == 0){
				direction =0;
			}
	
	}
}

// code not shown for clear_screen() and draw_line() subroutines

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
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

void delay(){
	volatile int* pixel_ctrl_ptr = (int*)0xff203020;
	int status;
	*pixel_ctrl_ptr = 1;
	status = *(pixel_ctrl_ptr + 3);
	
	while((status & 0x01) != 0){
		status = *(pixel_ctrl_ptr +3);
	}
}