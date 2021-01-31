
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>


#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC 27
#define EXIT 0
#define COL_OUT_BOARD 26
#define ROW_OUT_BOARD 81
#define MID_COL 38
#define MID_ROW 13

#define MENU_ROW_SIZE 55
#define BOARD_ROW_SIZE 25
#define BOARD_COL_SIZE 75

#define ASTRIK '*'
#define BORDER '#'
#define SPACE ' '
#define SNAKE_SIGN '@'
#define FOOD_SIGN '$'

#define SNAKE_MAX_SIZE 20
#define SNAKE_INIT_SIZE 10
#define INIT_HEAD 9
#define UNINIT -1
#define LAST_ELEMENT 0
#define WIN_INDEX 19
#define INCREASE 1
#define DECREASE -1
#define NO_CHANGE 0

#define TRUE 1
#define FALSE 0


typedef struct SnakeElement {
	char sign;
	int colNum;
	int rowNum;
} SnakeElement;



typedef struct {

	char sign;
	int colNum;
	int rowNum;

}Food;


typedef struct {

	int headIndex;
	int isCollision;
	Food currentFood;

} MoveInfo;



void gotoxy(int y, int x);
int getKey();
int selectGameMode();
int initGameMode(int userModeSelection);
void printBoardFrame(int rowSize, int colSize, char sign);
void printRow(int colSize, char sign);
void printCols(int rowSize, int colSize, char sign);
void playGame(SnakeElement snake[]);
MoveInfo initMoveInfo(SnakeElement snake[]);
MoveInfo makeMove(SnakeElement snake[], MoveInfo moveInfo, int isInit, int colChange, int rowChange);
void initSnakeHead(SnakeElement[], int initColNum, int initRowNum);
void completeInitSnake(SnakeElement snake[], int newColNum, int newRowNum, int headIndex);
void moveSnake(SnakeElement snake[], int newColNum, int newRowNum, int headIndex);
int raiseSnake(SnakeElement snake[], int headIndex, int newColNum, int newRowNum, Food food);
int randomNumber(int size);
Food randomFood(SnakeElement snake[], int headIndex);
int checkCollision(SnakeElement snake[], int headIndex, int colTarget, int rowTarget);
void gameWin();
void gameOver();
char receiveMoveArrow();
char checkNextMoveArrow(char moveArrow);
void printSpecificColRow(SnakeElement snake[], MoveInfo moveInfo);
MoveInfo moveByArrow(char moveArrow, SnakeElement snake[], MoveInfo moveInfo, int isInit);
int checkWinOrLoss(MoveInfo moveInfo);
int checkIfSankeInit(int* initMovesCounter, int isInit);






/**********************************************************
This program implements Snake game.

User enetrs arrows in keyboard in order to control snake
movenment. In start, snake consists of 10 elements.
Food will be randonly shown on board. Snake has to eat food
in order to increase until he will consists of 20 elements.
If snake arrives to 20 elements- user wins.
If snake collides in one of his elements or in borders,
user loses.
**********************************************************/


/*
Main function initializes snake array.
According to user mode selection, the game level is chosen.
At this phase, only Basic Level is impelemented.
In case that user chooses from menu valid input,
There is printing of game board by printBoardFrame() function.
Then there is first initializing of snake head by  initSnakeHead().
And then the game is started by playGame() function.
*/
void main()
{

	srand(time(NULL));

	int userModeSelection = -1;
	int isInvalidInput = FALSE;
	SnakeElement snake[SNAKE_MAX_SIZE];

	while (userModeSelection != EXIT) {

		userModeSelection = selectGameMode();
		isInvalidInput = initGameMode(userModeSelection);

		if (!isInvalidInput && userModeSelection != EXIT) {
			printBoardFrame(BOARD_ROW_SIZE, BOARD_COL_SIZE + 2, BORDER);
			initSnakeHead(snake, MID_COL, MID_ROW);
			playGame(snake);
		}

	}


}


/*
This function prints menu and allows user to select game mode.
At this phase, only Basic Level is impelemented.
Function returns user selection.
*/
int selectGameMode() {

	int userModeSelection = 0;

	printRow(MENU_ROW_SIZE, ASTRIK);
	printf("* Pick Your Level, to exit press 0\n");
	printf("* 0 -Exit\n");
	printf("* 1 - Basic Level\n");
	printf("* 2 - Intermidiate Level\n");
	printf("* 3 - Advanced Level\n");
	printRow(MENU_ROW_SIZE, ASTRIK);

	scanf("%d", &userModeSelection);

	return userModeSelection;
}


/*
This function check if user selection is valid,
and in case it is not valid, return isInvalid flag.
At this phase only Basic Level is impelemented so only
in case user selected 1, game will be started.
In case that user enterted 0, it means that he
asked to exit.
*/
int initGameMode(int userModeSelection) {

	int isInvalidInput = 0;

	switch (userModeSelection) {
	case 1:break;
	case 2: isInvalidInput = 1; break;
	case 3: isInvalidInput = 1; break;
	case 0:break;
	default: isInvalidInput = 1; break;
	}

	return isInvalidInput;

}


/*
This function prints board frame.
Function receives row size, col size and sign.
*/
void printBoardFrame(int rowSize, int colSize, char sign) {
	system("cls");
	printRow(colSize, sign);
	printCols(rowSize, colSize, sign);
	printRow(colSize, sign);
}

/*
This function used to print row according to column size
and sign which received.
*/
void printRow(int colSize, char sign) {
	int i;

	for (i = 0; i < colSize; i++) {
		printf("%c", sign);
	}

	printf("\n");
}


/*
This function used to print columns.
Function receives row size, column size and sign.
Only in first column and last column the sign is printed.
In other columns there is space printing.
*/
void printCols(int rowSize, int colSize, char sign) {
	int i, j;

	for (i = 0; i < rowSize; i++) {

		for (j = 0; j < colSize; j++) {

			if (j == 0 || j == colSize - 1) {
				printf("%c", sign);
			}
			else {
				printf("%c", SPACE);
			}
		}
		printf("\n");
	}


}

/*
This function is used to init snake head.
Function receives SnakeElement array, init column num and init
row num. SnakeElement is defined with the row and column
which received. Then head is located in array in index INIT_HEAD (9).
And the array for indexs 1 to 8, is filled by uninitialized
SnakeElement structs.
*/
void initSnakeHead(SnakeElement snake[], int initColNum, int initRowNum) {

	int i;
	SnakeElement head;
	SnakeElement snakeElement;

	head.sign = SNAKE_SIGN;
	head.colNum = initColNum;
	head.rowNum = initRowNum;

	snakeElement.sign = SNAKE_SIGN;
	snakeElement.colNum = UNINIT;
	snakeElement.rowNum = UNINIT;

	snake[INIT_HEAD] = head;

	for (i = 0; i < SNAKE_INIT_SIZE - 1; i++) {
		snake[i] = snakeElement;
	}

}


/*
This function manages play mode.

Function started the movement of the snake from head row and column.
Then function is waiting from another key from user by
function checkNextMoveArrow().
According to arrow received, there is movement of snake by
moveByArrow() function. At first 10 move, the snake
is still in init mode and it raised in any move,
there is checking if snake is already inited fully by
function checkIfSankeInit().Then there is printing of new
snake head by printSpecificColRow() function.
And then checking if game is over in Loss or Win by
function checkWinOrLoss().
In case customer pressed ESC - game is over.
In case still there is no win or loss, snake movement still
proceeds.
*/
void playGame(SnakeElement snake[]) {

	int isInit = TRUE;
	int isGameOver = FALSE;
	char moveArrow;
	char moveArrowRecevied;
	int initMovesCounter = 0;
	MoveInfo moveInfo = initMoveInfo(snake);
	Food currentFood = moveInfo.currentFood;

	gotoxy(snake[moveInfo.headIndex].colNum, snake[moveInfo.headIndex].rowNum);
	moveArrow = receiveMoveArrow();

	while (moveArrow != ESC && !isGameOver)
	{
		Sleep(1000);

		if (_kbhit()) {
			moveArrow = checkNextMoveArrow(moveArrow);
		}
		moveInfo = moveByArrow(moveArrow, snake, moveInfo, isInit);
		if (isInit){
			isInit = checkIfSankeInit(&initMovesCounter, isInit);
		}
		printSpecificColRow(snake, moveInfo);
		isGameOver = checkWinOrLoss(moveInfo);
	}
	if (moveArrow == ESC) {
		gameOver();
	}

}


/*
This function receives the first move arrow.
Any key which is not arrow or ESC. User has to press on another key.
Function return the moveArrow which selected.
*/
char receiveMoveArrow() {

	char moveArrow;

	do {
		moveArrow = getKey();
	} while (moveArrow != RIGHT && moveArrow != LEFT && moveArrow != UP && moveArrow != DOWN);

	return moveArrow;
}


/*
This function is used to check next move arrow which received
from user. If the new move is one of arrows keys or ESC.
the new move is returned. else, the last arrow is returned,
which means that snake will keep movement as last arrow received.
Function returns move arrow.
*/
char checkNextMoveArrow(char moveArrow) {

	char moveArrowRecevied;

	moveArrowRecevied = getKey();
	if (moveArrowRecevied == RIGHT || moveArrowRecevied == LEFT || moveArrowRecevied == UP || moveArrowRecevied == DOWN || moveArrowRecevied == ESC) {
		moveArrow = moveArrowRecevied;
	}

	return moveArrow;
}


/*
This function is used to make move according to arrow received and
to make the move with relevant values for row and column.
Function recevies move arrow, SnakeElement array, MoveInfo struct
and isInit flag.
According to move arrow, function calls to makeMove() function with
the relevant values to add to row and column. For any arrow, if need
to increase\decrease\no change the row and column.
Function returns MoveInfo struct.
*/
MoveInfo moveByArrow(char moveArrow, SnakeElement snake[], MoveInfo moveInfo, int isInit) {

	switch (moveArrow)
	{
	case RIGHT:
		moveInfo = makeMove(snake, moveInfo, isInit, INCREASE, NO_CHANGE);
		break;
	case DOWN:
		moveInfo = makeMove(snake, moveInfo, isInit, NO_CHANGE, INCREASE);
		break;
	case LEFT:
		moveInfo = makeMove(snake, moveInfo, isInit, DECREASE, NO_CHANGE);
		break;
	case UP:
		moveInfo = makeMove(snake, moveInfo, isInit, NO_CHANGE, DECREASE);
		break;
	}

	return moveInfo;
}


/*
This function is used to check if snake is still in init mode.
At the start of the game, snake in any move is increased in 1 element.
after snake consists of 10 elements, init mode is stopped and
snake is increased only when eat.
Function receivs pointer to initMovesCounter from playGame() function,
and increases the counter as long as it still init mode.
If snake consists of SNAKE_INIT_SIZE(10), isInit flag is changed to false.
Function returns the isInit flag.
*/
int checkIfSankeInit(int* initMovesCounter, int isInit) {

	if (isInit) {
		(*initMovesCounter)++;
	}
	if (*initMovesCounter == SNAKE_INIT_SIZE) {
		isInit = FALSE;
	}

	return isInit;
}


/*
This function is used to move the cursor to spesific row and column
on board and to print the new snake head element which added.
And then, move the cursor out of board.
*/
void printSpecificColRow(SnakeElement snake[], MoveInfo moveInfo) {

	gotoxy(snake[moveInfo.headIndex].colNum, snake[moveInfo.headIndex].rowNum);
	printf("%c", snake[moveInfo.headIndex].sign);
	gotoxy(COL_OUT_BOARD, ROW_OUT_BOARD);

}

/*
This function used to init the first MoveInfo struct.
Function receivs SnakeElement array and returns MoveInfo.
*/
MoveInfo initMoveInfo(SnakeElement snake[]) {

	MoveInfo moveInfo;
	moveInfo.headIndex = INIT_HEAD;
	moveInfo.isCollision = FALSE;
	moveInfo.currentFood = randomFood(snake, INIT_HEAD);

	return moveInfo;
}


/*
This function manages the process of making move.
Function receives SnakeElement array, MoveInfo struct, isInit flag,
row change and column change.
Function predicted the requested move by column change and row change,
and according to new values checks the following:
(1) function checks if the requested move casued to collision by
checkCollision() function.
In case there is no collision:
(2) If the requested move is to location that food located,
increase snake size by raiseSnake() function.
And then create another random food on board by randomFood() function.
(3) If is init mode of snake, increase the snake in one element by using
completeInitSnake() function.
(4) In other case which is not eating ot not init mode - only proceed
with snake movement by moveSnake() function.
At the end, all the updated infromation is definded in MoveInfo struct
and it returned.
*/
MoveInfo makeMove(SnakeElement snake[], MoveInfo moveInfo, int isInit, int colChange, int rowChange) {

	int headIndex = moveInfo.headIndex;
	int isCollision = moveInfo.isCollision;
	Food currentFood = moveInfo.currentFood;

	isCollision = checkCollision(snake, headIndex, snake[headIndex].colNum + colChange, snake[headIndex].rowNum + rowChange);

	if (!isCollision) {

		if (snake[headIndex].colNum + colChange == currentFood.colNum && snake[headIndex].rowNum + rowChange == currentFood.rowNum) {
			headIndex = raiseSnake(snake, headIndex, snake[headIndex].colNum + colChange, snake[headIndex].rowNum + rowChange, currentFood);
			currentFood = randomFood(snake, headIndex);
		}

		if (isInit) {
			completeInitSnake(snake, snake[headIndex].colNum + colChange, snake[headIndex].rowNum + rowChange, headIndex);
		}

		else {
			moveSnake(snake, snake[headIndex].colNum + colChange, snake[headIndex].rowNum + rowChange, headIndex);
		}
	}

	moveInfo.headIndex = headIndex;
	moveInfo.isCollision = isCollision;
	moveInfo.currentFood = currentFood;

	return moveInfo;

}


/*
This function checks if there is win or loss.
Function receives MoveInfo.
calls to gameWin() or gameOver() as needed.
and returns isGameOver flag.
*/
int checkWinOrLoss(MoveInfo moveInfo) {

	int isGameOver = FALSE;

	if (moveInfo.headIndex == WIN_INDEX) {
		gameWin();
		isGameOver = TRUE;
	}

	if (moveInfo.isCollision) {
		gameOver();
		isGameOver = TRUE;
	}

	return isGameOver;
}


/*
This function used to increase snake in init mode.
Function recieves SnakeElement array, new column num, new row num
and headIndex.
At first 10 moves, the snake is still in init mode and
snake is raised in any move by one element.
When snake is first initialized, index 0 to 8 was initialized
with uninit SnakeElement structs.
Function copies the following indexs in array to
previous indexs, and in this way it delete the last element of snake body.
At the end, function inserts to array in headInedx the new column num and
new row num.
*/
void completeInitSnake(SnakeElement snake[], int newColNum, int newRowNum, int headIndex) {

	int i;

	for (i = 0; i < headIndex; i++) {
		snake[i] = snake[i + 1];
	}

	snake[headIndex].sign = SNAKE_SIGN;
	snake[headIndex].colNum = newColNum;
	snake[headIndex].rowNum = newRowNum;


}


/*
This function is used to create snake movement.
Function recieves SnakeElement array, new column num, new row num
and headIndex.
Function copies the following indexs in array to
previous indexs, and in this way it delete the last element of snake body.
Function inserts to array in headInedx the new column num and
new row num.
At the end, using gotoxy function goes with cursor to last elemnent
location and print space.
*/
void moveSnake(SnakeElement snake[], int newColNum, int newRowNum, int headIndex) {

	int i;

	int colToRemove = snake[LAST_ELEMENT].colNum;
	int rowToRemove = snake[LAST_ELEMENT].rowNum;


	for (i = 0; i < headIndex; i++) {
		snake[i] = snake[i + 1];
	}

	snake[headIndex].colNum = newColNum;
	snake[headIndex].rowNum = newRowNum;

	gotoxy(colToRemove, rowToRemove);
	printf(" ");

}

/*
This function is used to raise snake.
Function recieves SnakeElement array, new column num, new row num
and headIndex, and Food struct.
Function uses gotoxy function in order to replace print of Food sign to
Snake sign. Then, initializng new SnakeElement which represent the new head.
The SnakeElement of new head is defined with new row column num and new row column num. And in the end, inserts the new head to the new head index
in snake array.
At the end, returns the newHeadIndex.
*/
int raiseSnake(SnakeElement snake[], int headIndex, int newColNum, int newRowNum, Food food) {

	int newHeadIndex = headIndex + 1;
	SnakeElement newHead;

	gotoxy(food.colNum, food.rowNum);
	printf("%c", SNAKE_SIGN);

	newHead.sign = SNAKE_SIGN;
	newHead.colNum = newColNum;
	newHead.rowNum = newRowNum;

	snake[newHeadIndex] = newHead;

	return newHeadIndex;

}


/*
This function is used to random food on board.
Function receives SnakeElement array and headIndex.
Using randomNumber() function to receive location.
If this location cause to collision with snake, create another random.
Using gotoxy to print the snake sign.
At the end, retruns Food.
*/
Food randomFood(SnakeElement snake[], int headIndex) {

	Food food;


	food.sign = FOOD_SIGN;

	do
	{
		food.colNum = randomNumber(BOARD_COL_SIZE);
		food.rowNum = randomNumber(BOARD_ROW_SIZE);

	} while (checkCollision(snake, headIndex, food.colNum, food.rowNum));


	gotoxy(food.colNum, food.rowNum);
	printf("%c", food.sign);

	return food;
}

/*
This function is used to generate random number.
This function receives int size, the random number using rand() fucntion
from stdlib.h library. and function returns the random number.
*/
int randomNumber(int size) {

	int randomNum;
	do {
		randomNum = rand() % size;
	} while (randomNum == 0);
	return randomNum;

}

/*
This function is used to check collision.
Function receivs SnakeElement array, headIndex, col traget, row target.
(1) If col target or row target is on borders - isCollision flag changed to true.
(2) If col target and row target is identical to snake element which existed -
isCollision flag changed to true.
At the end, retruns isCollision flag.
*/
int checkCollision(SnakeElement snake[], int headIndex, int colTarget, int rowTarget) {

	int i;
	int isCollision = FALSE;

	if (colTarget == 0 || colTarget == BOARD_COL_SIZE + 1 || rowTarget == 0 || rowTarget == BOARD_ROW_SIZE + 1) {
		isCollision = TRUE;
	}
	else {
		for (i = 0; i < headIndex; i++) {
			if (colTarget == snake[i].colNum && rowTarget == snake[i].rowNum) {
				isCollision = TRUE;
			}
		}
	}

	return isCollision;

}

/*
Function used to print message of game win.
*/
void gameWin() {
	system("cls");
	printf("##################################\n");
	printf("	    YOU WIN\n");
	printf("##################################\n");
}

/*
Function used to print message of game over.
*/
void gameOver() {
	system("cls");
	printf("##################################\n");
	printf("	    GAME OVER\n");
	printf("##################################\n");
}



// This function moves the cursor to the line y and column x.
// (0,0) is the upper left corner of the screen.
void gotoxy(int y, int x) {
	printf("\x1b[%d;%df", x + 1, y + 1);
}

// This function gets keyCodes for special keys such as arrows , escape and others
// It returns the Key code such as 72 for arrow up, 27 for Escape...
int getKey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}
