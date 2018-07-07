
#ifndef _BOARD_HEADER_
#define _BOARD_HEADER_



/************************Defines*****************/
#define _CRT_SECURE_NO_WARNINGS
#define LINECOL_SIZE 9
#define SQR_SIZE 3
#define ERROR_CODE -1
#define AMOUNT_OF_POSSIBILLITIES 9
#define NOT_FINISH 0
#define FINISH_SUCCESS 1
#define FINISH_FAILURE -3
#define EMPTY_CELL -1
#define TRUE 1
#define FALSE 0
#define FIRST_SQR_CELL(X) ((X / SQR_SIZE)*SQR_SIZE)
#define PRINT_CURRENT_MIN_CELL(X,Y) 		printf("Cell number[%d,%d] currently holds the minimum number of possible values, select one of the below :\n", X, Y)
#define PRINT_CELL_UPDATED(X,Y,NEW_VALUE) 	printf("Updating cell number [%d,%d] with value %d\n", X, Y, NEW_VALUE)
#define NAMESIZE 100
#define ERRORMSG(FUNCTION){																	\
	fprintf(stderr, "ERROR!! Failed to allocate capacity in %s", FUNCTION);					\
	exit(ERROR_CODE);																		\
}


#include "ListCord.h"
#include <stdio.h> // had to put the stdio here for the FILE strcuts
#include <stdlib.h>
#include <time.h>
/**********************************************Structs definition**************************************/


typedef struct _ARRAY
{
	short *arr;
	unsigned short size;
} ARRAY;


/****************************Functions prototypes**************************************/

/************************BoardCreation.c Prototypes***********************/



//Functions which prints the board
void sudokoPrintBoard(FILE *WinFile, short board[][9]);


//Fills the possible digits for each board cell
ARRAY ***PossibleDigits(short sudokuBoard[][LINECOL_SIZE]);
int CheckPossibillitiesBySquare(short *possibillites, short sudokuBoard[][LINECOL_SIZE], unsigned int LineNumber, unsigned int ColumnNumber);
int CheckPossibillities(short *possibillites, short sudokuBoard[][LINECOL_SIZE], unsigned int Line, unsigned int Col);
void InsertPossibillites(ARRAY **solutionCell, short* possibillities);

int OneStage(short board[][9], ARRAY ***possibilities, int *x, int *y);

//Functions which deletes possible digits from possible digit array
int SearchAndDelPossibillityInArray(unsigned int LineNumber, unsigned int ColNumber, ARRAY*** Possibillities, short OptionToFind);


void FreeSlotInPos(ARRAY *** possibilities, unsigned int row, unsigned int col);

//Fillboard functions
int FillBoard(short board[][9], ARRAY ***possibilities);
int ChooseOption(ARRAY * possibilities);

//Functions which initialize a new board

short initializationBoard(short sudokuBoard[][LINECOL_SIZE]);



#endif