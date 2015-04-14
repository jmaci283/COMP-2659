#include "raster.h"


#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	400
#define GRID_START_X	48
#define GRID_START_Y    40

void plot_pixel(char *base, int x, int y)
{
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
	{
		*(base + y * 80 + (x >> 3)) |= 1 << 7 - (x & 7);
	}
}

void plot_vertical_line(char *base, int x, int yFirst, int ySecond)
{
	int xVal = x;
	int yVal1 = yFirst;
	int yVal2 = ySecond;

	if (yVal1 > yVal2){
		while (!(yVal1 == yVal2)){
			plot_pixel(base, xVal, yVal1);
			yVal1 = yVal1 - 1;
		}
		plot_pixel(base, xVal, yVal1);
	}
	else if (yVal1 < yVal2){
		while (!(yVal1 == yVal2)){
			plot_pixel(base, xVal, yVal1);
			yVal1 = yVal1 + 1;
		}
		plot_pixel(base, xVal, yVal1);
	}
	else{	/* yFirst == ySecond */
		plot_pixel(base, xVal, yVal1);
	}
}

void plot_horizontal_line(char *base, int xFirst, int xSecond, int y)
{
	int counter;
	char* tempBase = base;
	
	tempBase += (80*y);									/* 80 bytes per line */
	tempBase += (xFirst >> 3);  						/* divide by 8 */
	*tempBase = ((0xFF >> (xFirst & 7)));  				/* mod by 8 */
	tempBase += 1; 										/* Does left side of horizontal line */
	for (counter = 0; counter < (((xSecond - xFirst)/8)); counter ++)
	{
		*tempBase = 0xFF;
		tempBase += 1;
	}
	*tempBase = ((0xFF << (7 - (xSecond & 7))));  			/* mod by 8 */
															/* Does right side of horizontal line */
}

void plot_tetris_display(int *base)
{
	plot_horizontal_line(base, 268, 362, 8);
	plot_vertical_line(base, 268, 8, 20);
	plot_vertical_line(base, 362, 8, 20);
	plot_horizontal_line(base, 268, 362, 20);
	
	plotChar(base, 272, 10,'T');
	plotChar(base, 288, 10, 'E');
	plotChar(base, 304, 10, 'T');
	plotChar(base, 320, 10, 'R');
	plotChar(base, 336, 10, 'I');
	plotChar(base, 352, 10, 'S');

	plotChar(base, 320, 100, 'S');
	plotChar(base, 336, 100, 'C');
	plotChar(base, 352, 100, 'O');
	plotChar(base, 368, 100, 'R');
	plotChar(base, 384, 100, 'E');

	plot_horizontal_line(base, 300, 410,112);
	plot_vertical_line(base, 300, 112, 140);
	plot_vertical_line(base, 410, 112, 140);
	plot_horizontal_line(base, 300, 410, 140);

	plotChar(base, 304, 122, '0'); /* keep track of these locations for when */
	plotChar(base, 320, 122, '0'); /* printing the number's digits*/
	plotChar(base, 336, 122, '0');
	plotChar(base, 352, 122, '0');
	plotChar(base, 368, 122, '0');
	plotChar(base, 384, 122, '0');
	plotChar(base, 400, 122, '0');

	plotChar(base, 328, 200, 'T');
	plotChar(base, 344, 200, 'I');
	plotChar(base, 360, 200, 'M');
	plotChar(base, 376, 200, 'E');

	plot_horizontal_line(base, 300, 410, 212);
	plot_vertical_line(base, 300, 212, 240);
	plot_vertical_line(base, 410, 212, 240);
	plot_horizontal_line(base, 300, 410, 240);

	plotChar(base, 320, 222, '0');
	plotChar(base, 336, 222, '0');
	plotChar(base, 352, 222, ':');
	plotChar(base, 368, 222, '0');
	plotChar(base, 384, 222, '0');

}

void plot_cell_grid(char *base) /* 10w x 18h */
{
	int startX = GRID_START_X;
	int startY = GRID_START_Y;
	int endX = startX + (10 * 16); /* grid width  * num grids */
	int endY = startY + (18 * 16); /* grid height * num grids */
	
	plot_horizontal_line(base, startX-2, endX+1, startY-3);
	plot_horizontal_line(base, startX-2, endX+1, startY-2);
	
	plot_horizontal_line(base, startX-2, endX+1, endY);
	plot_horizontal_line(base, startX-2, endX+1, endY+1);
	
	plot_vertical_line(base, startX-2, startY-3, endY);
	plot_vertical_line(base, startX-1, startY-3, endY);
	
	plot_vertical_line(base, endX+2, startY-3, endY+1);
	plot_vertical_line(base, endX+1, startY-3, endY+1);
}

void fill_cell(int *base, int cellX, int cellY)
{
	int* tempBase = base;
	int startX = GRID_START_X + (cellX * 16);
	int startY = GRID_START_Y + (cellY * 16);
	int i;
	
	tempBase += (40*startY);
	tempBase += (startX >> 4); 
	
	for (i = 0; i < 15; i++)
	{
		*tempBase = 0x7FFF;
		tempBase += 40;
	}
}

void clear_cell(int *base, int cellX, int cellY)
{
	int* tempBase = base;
	int startX = GRID_START_X + (cellX * 16);
	int startY = GRID_START_Y + (cellY * 16);
	int i;
	
	tempBase += (40*startY);
	tempBase += (startX >> 4); 
	
	for (i = 0; i < 15; i++)
	{
		*tempBase = 0x0000;
		tempBase += 40;
	}
}


void plotChar(UINT8 *base, int x, int y, char c)
{
	int counter;
	int index;
	UINT8* tempBase = base;
    UINT8* ptr;
	tempBase += (80*y);		 
	tempBase += (x >> 3); 	
	ptr = GLYPH_START(c);
	if (IS_PRINTABLE(c) && (charInBounds(x,y) == 1))
	{
		for(counter = 0; counter < FONT_HEIGHT; counter++)
		{
			*tempBase = (*(ptr+counter) << (7 - (x+7 & 7)));
			tempBase += 80;
		}
	}
}

char charInBounds(int x, int y)
{
	if ((x >= 0) && (x <= 632) && (y >= 0) && ( y <= 400))
	{
		return 1;
	}
	return 0;
}
/* stubbed method bodies go here */