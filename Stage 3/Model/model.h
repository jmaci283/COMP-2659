#define GRID_WIDTH 10
#define GRID_HEIGHT 18


struct Block
{
	char *all_Block[4];
	int total_Patterns;
	int rotation;
};

struct Shape /* x and y can be negatives */
{
	struct Block *currentShape;
    signed int x; 
    signed int y;
}; 

struct Cell  		/* holds information about a single position in the tetris grid */
{
	int isFull; 	/* has the cell been filled by the placement of a block or the presence of the current block */
};

struct Score
{
	int value;
};

struct Grid 	/* holds the array of grid cells containing the current state of the grid */
{
	struct Cell cells[GRID_WIDTH][GRID_HEIGHT];
};

struct Time
{
	int value;
};

struct Model
{
	struct Grid grid;
	struct Score score;
	struct Time time;
	struct Shape shape;
};


/*
0 = block
1 = Zig Zag Right
2 = Zig Zag Left
3 = Straight
4 = Corner Left
5 = Corner Right
6 = T-Block
*/

int lowerShape(struct Shape s);
void makeBlock(int blockNum);
int canLowerShape(); /*needs to pass a struct*/
int canLowerCell(int y);
void initShapes ();
