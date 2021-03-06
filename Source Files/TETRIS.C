/*************************************************************************
FILE:
AUTHORS:	James MacIsaac & Brad Ritten
PURPOSE:
*************************************************************************/
#include "tetris.h"

UINT8 buffer[32255]; 

/*************************************************************************
FUNCTION:
PARAMETERS:
RETURNS:
PURPOSE:
*************************************************************************/
int main()
{
	void *base = get_vb(); /* frame buffer pointer */
	/*kbd_read();*/
	run_menu(base);
	return 0;
}

/*************************************************************************
FUNCTION:
PARAMETERS:
RETURNS:
PURPOSE:
*************************************************************************/
long getCurTime()
{
	long *timer = (long *)0x462; /* address of longword auto-inc’ed 70 x per s */
	long timeNow;
	long old_ssp;
	enter_super(); /* enter privileged mode */
	timeNow = *timer;
	exit_super(); /* exit privileged mode as soon as possible */
	return timeNow;
}

/*************************************************************************
FUNCTION:
PARAMETERS:
RETURNS:
PURPOSE:
*************************************************************************/
void init_frame(char *base)
{
	disable_cursor();
	clear_screen(base);
/* 	render_interface(base); */
}

/*************************************************************************
FUNCTION:
PARAMETERS:
RETURNS:
PURPOSE:
*************************************************************************/
int doubleBuffer(struct Model *model, void *base, UINT8 *back, int isBase)
{

	if (isBase == 1)
	{
		clear_model_elements(base);
		render_model_elements(base, &model);
		isBase = 0;

		set_vb(base);
	}
	else
	{
		clear_model_elements(back);
		render_model_elements(back, &model);
		isBase = 1;

		set_vb(back);
	}

	return isBase;
}

/*************************************************************************
FUNCTION:
PARAMETERS:
RETURNS:
PURPOSE:
*************************************************************************/
void run_menu(char *base)
{
	int keyPress;				
	int currentIndex = 1;		/*menu button index*/
	int selection = 0;

	init_frame(base);
	fill_screen(base);
	plot_menu_display(base);
	plot_menu_selection(base, currentIndex);
	

	while (selection == 0)
	{
		keyPress = menuSelect();

		if (keyPress == 1) /* up arrow */
		{
			
			if (currentIndex == 1)
			{
				playClearEffect();
			}
			else if (currentIndex == 2)
			{
				clear_menu_selection(base, currentIndex);
				currentIndex = 1;
				plot_menu_selection(base, currentIndex);
			}
			else if (currentIndex == 3)
			{
				clear_menu_selection(base, currentIndex);
				currentIndex = 2;
				plot_menu_selection(base, currentIndex);
			}
		}
		else if (keyPress == 2) /* down arrow */
		{
			
			if (currentIndex == 1)
			{
				clear_menu_selection(base, currentIndex);
				currentIndex = 2;
				plot_menu_selection(base, currentIndex);
			}
			else if (currentIndex == 2)
			{
				clear_menu_selection(base, currentIndex);
				currentIndex = 3;
				plot_menu_selection(base, currentIndex);
			}
			else if (currentIndex == 3)
			{
				playDropEffect();
			}
			
		}
		else if (keyPress == 3)
		{
			if (currentIndex = 1)
			{
				selection = 1;
				main_game_loop(base);
			}
			else if (currentIndex == 2)
			{
				selection = 1;
				/* two player game */
			}
			else if (currentIndex == 3)
			{
				selection = 1;
				/* quit */
			}
		}
		
	}
}

/*************************************************************************
FUNCTION:
PARAMETERS:
RETURNS:
PURPOSE:
*************************************************************************/
int main_game_loop(char *base)
{
	UINT8 *back = (UINT8*)(((UINT32)(buffer)+256) & 0xFFFFFF00);
	int isBase = 1;
	int x;
	int y;
	int canLower;
	int canRight;
	int canLeft;
	int canRot;
	int blockNum;
	int result = 0;
	struct Model model;
	struct Block blocks[7];
	long timeNow;
	long timeElapsed;
	long timeThen = 0;
	int keyPress;
	int isGameLost;
	int points;

	blockNum = getCurTime() % 7;
	clear_screen(base);
	clear_screen(back);
	init_frame(base);
	init_frame(back);
	init(&model, blocks);
	makeBlock(blockNum, &model, blocks);
	
	isGameLost = gameLost(&model);
	
	render_frame(base, &model);
	render_frame(back, &model);
	
	timeNow = getCurTime();

	start_music(timeNow);

	while (isGameLost == 0)
	{

		detectKeyPress(&model);
		keyPress = model.userMovement;

		if (keyPress == 1)
		{
			clearShape(&model);
			canLeft = canMoveShapeLeft(&model);
			placeShape(&model);
			model.userMovement = 0;
			if (canLeft == 4)
			{
				moveShapeLeft(&model);

			}
		}
		if (keyPress == 2)
		{
			clearShape(&model);
			canRight = canMoveShapeRight(&model);
			model.userMovement = 0;
			placeShape(&model);
			if (canRight == 4)
			{
				moveShapeRight(&model);
			}
		}
		if (keyPress == 3)
		{
			clearShape(&model);
			canRot = canRotate(&model);
			if (canRot == 4)
			{
				rotateShape(&model);
			}
			placeShape(&model);
			model.userMovement = 0;
		}
		if (keyPress == 4)
		{
			clearShape(&model);
			canLower = canLowerShape(&model);
			if (canLower == 4)
			{
				dropShape(&model);
			}
			placeShape(&model);
			model.userMovement = 0;
		}

		timeNow = getCurTime();
		timeElapsed = timeNow - timeThen;

		if (timeElapsed > 0)
		{
			clearShape(&model);
			canLower = canLowerShape(&model);
			if (canLower != 4) 
			{
				playDropEffect();
				placeShape(&model);
				points = clearRows(&model);
				incr_score(&model, points);
				isGameLost = gameLost(&model);
				blockNum = rand() % 7;
				makeBlock(blockNum, &model, blocks);
			}
			dropShape(&model);
			incr_time(&model);
			timeThen = timeNow + 70;
		}

		if (isBase == 1)
		{
			clear_model_elements(base);
			render_model_elements(base, &model);
			isBase = 0;

			set_vb(base);
		}
		else
		{
			clear_model_elements(back);
			render_model_elements(back, &model);
			isBase = 1;

			set_vb(back);
		}
		update_music(timeNow);

	}

	set_vb(base);
	stop_sound();
	for (y = GRID_HEIGHT - 1; y >= 0; y--) /* game lost */
	{
		for (x = 0; x < GRID_WIDTH; x++)
		{
			model.grid[x][y] = 1;
		}
		x = 0;
		render_frame(base, &model);
	}
	
}


