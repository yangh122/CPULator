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


int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;

    clear_screen();
	//draw_line(150, 150, 319, 0, 0x07E0); // this line is green
	
	

	draw_line(0, 0, 150, 150, 0x001F);   // this line is blue
    draw_line(150, 150, 319, 0, 0x07E0); // this line is green
    draw_line(0, 239, 319, 239, 0xF800); // this line is red
    draw_line(319, 0, 0, 239, 0xF81F);   // this line is a pink color
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