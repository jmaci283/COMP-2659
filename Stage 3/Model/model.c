#include "model.h"
/*
0 = block
1 = Zig Zag Right
2 = Zig Zag Left
3 = Straight
4 = Corner Left
5 = Corner Right
6 = T-Block
*/
struct Block block;
struct Block zigZagLeft;
struct Block zigZagRight;
struct Block straight;
struct Block cornerRight;
struct Block cornerLeft;
struct Block tBlock;
struct Model model;


const char BlockR1[4][4] =
{
	1, 1, 0, 0,
	1, 1, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0  
};

const char ZigZagRightR1[4][4] =
{
	0, 1, 1, 0,
	1, 1, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
};
 
const char ZigZagRightR2[4][4] =
{
	1, 0, 0, 0,
	1, 1, 0, 0,
	0, 1, 0, 0,
	0, 0, 0, 0
};

const char ZigZagLeftR1[4][4] =
{
	1, 1, 0, 0,
	0, 1, 1, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
};
 
const char ZigZagLeftR2[4][4] =
{
	1, 1, 0, 0,
	0, 1, 1, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
};
 
const char StraightR1[4][4] =
{
	1, 0, 0, 0,
	1, 0, 0, 0,
	1, 0, 0, 0,
	1, 0, 0, 0
};
 
const char StraightR2[4][4] =
{
	1, 1, 1, 1,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
};
 
const char CornerRightR1[4][4] =
{
	1, 0, 0, 0,
	1, 0, 0, 0,
	1, 1, 0, 0,
	0, 0, 0, 0
};

const char CornerRightR2[4][4] =
{
	1, 1, 1, 0,
	1, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
};

const char CornerRightR3[4][4] =
{
	1, 1, 0, 0,
	0, 1, 0, 0,
	0, 1, 0, 0,
	0, 0, 0, 0
};

const char CornerRightR4[4][4] =
{
	0, 0, 1, 0,
	1, 1, 1, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
};

const char CornerLeftR1[4][4] =
{
	0, 1, 0, 0,
	0, 1, 0, 0,
	1, 1, 0, 0,
	0, 0, 0, 0
};

const char CornerLeftR2[4][4] =
{
	1, 0, 0, 0,
	1, 1, 1, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
};

const char CornerLeftR3[4][4] =
{
	1, 1, 0, 0,
	1, 0, 0, 0,
	1, 0, 0, 0,
	0, 0, 0, 0
};

const char CornerLeftR4[4][4] =
{
	1, 1, 1, 0,
	0, 0, 1, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
};

const char TblockR1[4][4] =
{
	0, 1, 0, 0,
	1, 1, 1, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
};

const char TblockR2[4][4] =
{
	0, 1, 0, 0,
	0, 1, 1, 0,
	0, 1, 0, 0,
	0, 0, 0, 0
};

const char TblockR3[4][4] =
{
	0, 0, 0, 0,
	1, 1, 1, 0,
	0, 1, 0, 0,
	0, 0, 0, 0
};

const char TblockR4[4][4] =
{
	0, 1, 0, 0,
	1, 1, 0, 0,
	0, 1, 0, 0,
	0, 0, 0, 0
}; 
 
void dropShape()
{
	model.shape.y += 1;
}

void moveShapeRight()
{
	model.shape.x += 1;
}

void moveShapeLeft()
{
	model.shape.x -= 1;
}

void rotateShape()
{
	int tempRotation;
	int tempTotalPatterns;
	
	tempRotation = model.shape.currentShape -> rotation;
	tempTotalPatterns = model.shape.currentShape -> total_Patterns;
	
	tempRotation++;
	tempRotation = tempRotation % tempTotalPatterns;
	
	model.shape.currentShape -> rotation = tempRotation;
}


int canLowerShape()  /* need to figure out how to pass the struct */
{
	signed int gridX = model.shape.x;
	signed int gridY = model.shape.y + 3;
	int shapeX = 0;
	int shapeY = 3;
	signed int xLimit = gridX + 3;
	signed int yLimit = gridY - 3;
 	int curRotation = model.shape.currentShape -> rotation;
	char *currentShapeGrid = model.shape.currentShape -> all_Block[curRotation]; 
	int *tetrisGrid;
	int col0,col1,col2,col3 = 1;
	tetrisGrid = (int *) &(model.grid.cells);

	printf("X = %d, ", gridX); 
	printf("\n");
	printf("Y = %d, ", gridY);
	printf("\n");
	
	for (; shapeX <= xLimit; shapeX++)
	{
		for (; shapeY >= yLimit; shapeY--)
		{
/* 		
			printf("%d,  GX%d - GY%d", *(tetrisGrid + (GRID_HEIGHT * gridX) + gridY), gridX, gridY);
			printf("\n"); */
			
			if (*(currentShapeGrid + (4*shapeX) + shapeY) == 1) /* will need bounds checking */
			{
				if ((gridY == GRID_HEIGHT-1) || *(tetrisGrid + (GRID_HEIGHT * gridX) + (gridY + 1)) == 1) /* will need bounds checking */
				{
					switch (shapeX)
					{
					case 0:
						col0 = 0;
						break;
					case 1:
						col1 = 0;
						break;
					case 2:
						col2 = 0;
						break;
					case 3:
						col3 = 0;
						break;
					}
				}
				else
				{
					/* there was nothing below for that cell, no need to set the col value to 0 */
				}
				shapeY = yLimit; /* moves us to next col */
			}
			gridY--; 
		}
		shapeY = gridY = model.shape.y + 3;
		gridX++;
	} 
	
	return (col0 + col1 + col2 + col3); /* 	all cols are initialized to 1 (meaning true). if any fail col fail, they will be set to 0. 
											meaning that if this function returns 4, the shape can be lowered, otherwise it cant. */
}


/*
0 = block
1 = Zig Zag Left
2 = Zig Zag Right
3 = Straight
4 = Corner Right
5 = Corner Left
6 = T-Block
*/
void makeBlock (int blockNum)
{
	switch (blockNum)
	{
		case 0:
			model.shape.currentShape = &block;
			break;
		case 1:
			model.shape.currentShape = &zigZagLeft;
			break;	
		case 2:
			model.shape.currentShape = &zigZagRight;
			break;	
		case 3:
			model.shape.currentShape = &straight;
			break;
		case 4:
			model.shape.currentShape = &cornerRight;
			break;
		case 5:
			model.shape.currentShape = &cornerLeft;
			break;
		case 6:
			model.shape.currentShape = &tBlock;
			break;
	}
}

int inbounds(int x, int y)
{
	if ((x >= 0) && (x < GRID_WIDTH) && (y >= 0) && (y <= GRID_HEIGHT))
	{
		return 1;
	}
	return 0;
}

void init ()
{
	block.total_Patterns = 1;
	zigZagLeft.total_Patterns = 2;
	zigZagRight.total_Patterns = 2;
	straight.total_Patterns = 2;
	cornerRight.total_Patterns = 4;
	cornerLeft.total_Patterns = 4;
	tBlock.total_Patterns = 4;
	
	block.all_Block[0] = (char *)(&BlockR1);
    
	zigZagLeft.all_Block[0] = (char *)(&ZigZagLeftR1);
    zigZagLeft.all_Block[1] = (char *)(&ZigZagLeftR2);
    
    zigZagRight.all_Block[0] = (char *)(&ZigZagRightR1);
    zigZagRight.all_Block[1] = (char *)(&ZigZagRightR2);
    
    straight.all_Block[0] = (char *)(&StraightR1);
    straight.all_Block[1] = (char *)(&StraightR2);
    
    cornerRight.all_Block[0] = (char *)(&CornerRightR1);
    cornerRight.all_Block[1] = (char *)(&CornerRightR2);
    cornerRight.all_Block[2] = (char *)(&CornerRightR3);
    cornerRight.all_Block[3] = (char *)(&CornerRightR4);
    
    cornerLeft.all_Block[0] = (char *)(&CornerLeftR1);
    cornerLeft.all_Block[1] = (char *)(&CornerLeftR2);
    cornerLeft.all_Block[2] = (char *)(&CornerLeftR3);
    cornerLeft.all_Block[3] = (char *)(&CornerLeftR4);
	
	tBlock.all_Block[0] = (char *)(&TblockR1);
	tBlock.all_Block[1] = (char *)(&TblockR2);
	tBlock.all_Block[2] = (char *)(&TblockR3);
	tBlock.all_Block[3] = (char *)(&TblockR4);
	
	
	block.rotation = 0;
	zigZagLeft.rotation = 0;
	zigZagRight.rotation = 0;
	straight.rotation = 0;
	cornerRight.rotation = 0;
	cornerLeft.rotation = 0;
	tBlock.rotation = 0;
	
	
	/*model.grid.cells[0][0] = 1;*/
}