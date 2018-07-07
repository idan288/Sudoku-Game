#include "BoardHeader.h"
#include "PlayersHeader.h"

static ARRAY ***AllocateBoard(char *function);
static void AllocateSolutionArray(short **result, unsigned int size, char *function);
static void AllocateCellBoard(ARRAY ** result, char *function);
static void PrintUnderLine(FILE *WinFile, unsigned int HowMuch);
static int ShouldPrintSeperators(unsigned int num);
static SearchAndUpdateOptionInArray(ARRAY* Possibillities, short OptionToFind);
static SearchAndDelByRowsAndCols(unsigned int LineNumber, unsigned int ColNumber, ARRAY*** Possibillities, short OptionToFind);
static SearchAndDelBySquare(unsigned int LineNumber, unsigned int ColNumber, ARRAY*** Possibillities, short OptionToFind);
static void UpdatePossibillityForCell(ARRAY *CellToUpdate, short OptionToDel);
static short LotteryNumber(ARRAY * solutioncell);
static LCNODE * GetTheCurPossition(ListCord ListCoordinate, LCNODE * prev);
static LCNODE *  FindSlotInList(ListCord lst, unsigned int numberslot);
static void CreatEmptySoduko(short sudokuBoard[][LINECOL_SIZE]);



/*******************************************Data allocation*************************************/


static ARRAY ***AllocateBoard(char *function)
{//Allocates matrix of pointers to ARRAY struct
	ARRAY ***	result;// allocate the pointer to the matrix
	unsigned int LineNumber;//will run on the line of the matrix, in order to allocate LINECOL_SIZE cells at each line
	result = (ARRAY ***)malloc(LINECOL_SIZE*sizeof(ARRAY **));
	if (!result)
		ERRORMSG(function);

	for (LineNumber = 0; LineNumber < LINECOL_SIZE; LineNumber++)
	{
		result[LineNumber] = (ARRAY **)malloc(LINECOL_SIZE*sizeof(ARRAY *));
		if (!result[LineNumber])
			ERRORMSG(function);
	}
	return (result);
}
static void AllocateCellBoard(ARRAY ** result, char *function)
{
	*result = (ARRAY *)malloc(sizeof(ARRAY));

	if (!result)
		ERRORMSG(function);
}
static void AllocateSolutionArray(short **result, unsigned int size, char *function)
{
	*result = (short *)malloc(size*sizeof(short));
	if (!result)
		ERRORMSG(function)
}

/******************************************Board printing*******************************************************************/

//This function print the sodoku board.

void sudokoPrintBoard(FILE *WinFile, short board[][9])
{
	unsigned int Line, Col, counter = 0;// counter will count how much underlines shall be printed
	char chToPrint;
	FILE *WhereTo;
	if (WinFile == NULL)
		WhereTo = stdout;
	else
		WhereTo = WinFile;

	counter += fprintf(WhereTo, "  | ");

	/**Prints the Line numbers**/
	for (Line = 0; Line < LINECOL_SIZE; Line++)
	{
		if (ShouldPrintSeperators(Line))
			counter += fprintf(WhereTo, "%d | ", Line);
		else
			counter += fprintf(WhereTo, "%d ", Line);
	}

	fprintf(WhereTo, "\n");

	/*Prints the underline*/
	PrintUnderLine(WhereTo, counter);

	/**Print the board data**/
	for (Line = 0; Line < LINECOL_SIZE; Line++)
	{
		fprintf(WhereTo, " %d| ", Line);

		for (Col = 0; Col < LINECOL_SIZE; Col++)
		{
			if (board[Line][Col] == EMPTY_CELL)
				chToPrint = ' '; // if its an empty cell, print just one space
			else
				chToPrint = (char)('0' + board[Line][Col]);

			if (ShouldPrintSeperators(Col)) // check if need to print a pipe or not
				fprintf(WhereTo, "%c | ", chToPrint);
			else
				fprintf(WhereTo, "%c ", chToPrint);
		}
		fprintf(WhereTo, "\n");
		if (ShouldPrintSeperators(Line))
			PrintUnderLine(WhereTo, counter);
	}
	PrintUnderLine(WhereTo, counter);
	fprintf(WhereTo, "\n\n");
}

static void PrintUnderLine(FILE *WinFile, unsigned int HowMuch)
{
	unsigned int i;

	for (i = 0; i < HowMuch; i++)
	{
			fprintf(WinFile, "-");
	}

		fprintf(WinFile, "\n");
	return;
}
static int ShouldPrintSeperators(unsigned int num)
{
	if ((num + 1) % SQR_SIZE == 0 && num + 1 != LINECOL_SIZE)
		return TRUE;
	else
		return FALSE;

}


/******************************************Scanning Possibillities*******************************************************************/
ARRAY *** PossibleDigits(short sudokuBoard[][LINECOL_SIZE])
{
	ARRAY ***result;
	short possibillities[AMOUNT_OF_POSSIBILLITIES + 1] = { 0 }; // will hold the disqualified options for each cell
	unsigned int LineNumber, ColumnNumber, disOptions; // disOptions will count the disqualified options for each cell 
	result = AllocateBoard("PossibleDigits"); // allocate the board

	for (LineNumber = 0; LineNumber < LINECOL_SIZE; LineNumber++)
	{

		for (ColumnNumber = 0; ColumnNumber < LINECOL_SIZE; ColumnNumber++)
		{
			if (sudokuBoard[LineNumber][ColumnNumber] == EMPTY_CELL) // cell is not filled
			{
				disOptions = CheckPossibillities(possibillities, sudokuBoard, LineNumber, ColumnNumber); // check the possibilities in a line and colum, will return the amount of disqualified possibillities
				disOptions += CheckPossibillitiesBySquare(possibillities, sudokuBoard, FIRST_SQR_CELL(LineNumber), FIRST_SQR_CELL(ColumnNumber)); // check the possibilities in a square

				if (disOptions == AMOUNT_OF_POSSIBILLITIES) // if the disQualified options equals to the amount of possibillities, there is no options for this cell, hence put NULL
					result[LineNumber][ColumnNumber] = NULL;
				else
				{
					InsertPossibillites(&result[LineNumber][ColumnNumber], possibillities); // insert all available possibilities found
				}
			}
			else
				result[LineNumber][ColumnNumber] = NULL; // this cell is already filled
		}

	}
	return(result);

}
int CheckPossibillitiesBySquare(short *possibillites, short sudokuBoard[][LINECOL_SIZE], unsigned int LineNumber, unsigned int ColumnNumber)
{//The function will check which numbers has been already used on a specific line and return the amount of numbers
	unsigned int i, j, takenoptions;

	takenoptions = 0;// count the disqualified possibillities.

	for (i = LineNumber; i < LineNumber + SQR_SIZE; i++)
	{
		for (j = ColumnNumber; j < ColumnNumber + SQR_SIZE; j++)
		{
			if (sudokuBoard[i][j] != EMPTY_CELL && possibillites[sudokuBoard[i][j]] == 0) //if the cell in that line & coulmn is different from -1, it means that it has some value inside 
			{// if the value on that cell is for eample: 5 , so it will change possibillities[5]=1
				possibillites[sudokuBoard[i][j]] = 1;
				takenoptions++;

			}
		}

	}
	return takenoptions;
}

int CheckPossibillities(short *possibillites, short sudokuBoard[][LINECOL_SIZE], unsigned int Line, unsigned int Col)
{//The function will check which numbers has been already used on a specific line and return the amount of numbers

	unsigned int i, takenoptions = 0;// count the disqualified possibillities.


	//Run on the rows..
	for (i = 0; i < LINECOL_SIZE; i++)
	{
		if (sudokuBoard[i][Col] != EMPTY_CELL)//if the cell in that line & coulmn is different from -1, it means that it has some value inside 
			possibillites[sudokuBoard[i][Col]] = 1; // if the value on that cell is for example: 5 , so it will change possibillities[5]=11
	}

	//Run on the cols..
	for (i = 0; i < LINECOL_SIZE; i++)
	{
		if (sudokuBoard[Line][i] != EMPTY_CELL && possibillites[sudokuBoard[Line][i]] == 0)
		{
			possibillites[sudokuBoard[Line][i]] = 1;
			takenoptions++;
		}
	}

	return takenoptions;
}

void InsertPossibillites(ARRAY **solutionCell, short* possibillities)
{
	unsigned int i, write = 0;
	ARRAY * Cell;


	AllocateCellBoard(&(*solutionCell), "InsertPossbillities"); // allocate cell address
	Cell = *solutionCell;

	Cell->size = 0; // initialize the size

	//First get the size of the array need to be allocated 
	for (i = 1; i <= AMOUNT_OF_POSSIBILLITIES; i++)
	{
		if (possibillities[i] == 0)//because if possibilities[i]==1 it means that this posibilitie is not an option.
			(Cell->size)++;
	}

	AllocateSolutionArray(&(Cell->arr), Cell->size, "InsertPossbillities"); // allocate the solution array according to the needed size

	//Got the memory allocation for the array, start adding the data
	for (i = 1; i <= AMOUNT_OF_POSSIBILLITIES; i++)
	{
		if (possibillities[i] == 0)
		{//if the number hasnt been found, add it to the solution cell and promote write in 1
			Cell->arr[write++] = i;
		}
		else
			possibillities[i] = 0; // intialize possibilities array after you finish read each cell
	}
}


/******************************************One Stage*********************************************************************************************************************/

int OneStage(short board[][9], ARRAY *** possibilities, int *x, int *y)
{
	unsigned int LineNumber, ColumnNumber, MinimalPos, result = FINISH_SUCCESS;
	char FoundCell;
	short OptionToAdd, CheckIfLegal;

	MinimalPos = AMOUNT_OF_POSSIBILLITIES + 1; // MinimalPos will tell us which cell has the minimum amount of possibillities
	FoundCell = TRUE;//check if found at least one cell with one option.
	//possibilities is a array of 81 cells.

	while (FoundCell)
	{
		FoundCell = FALSE; // if it remains false after the loops below, means that we ran on the entire board and there is no arrays with size of 1
		MinimalPos = AMOUNT_OF_POSSIBILLITIES + 1; // Initialize MinimalPos for the next run on the loops
		result = FINISH_SUCCESS; // if there is no NULL cells, this will remain FINISH SUCCESS until the end of the entire loop

		for (LineNumber = 0; LineNumber < LINECOL_SIZE; LineNumber++)
		{//Run on the rows

			for (ColumnNumber = 0; ColumnNumber < LINECOL_SIZE; ColumnNumber++)
			{// Run on the cols
				if (board[LineNumber][ColumnNumber] == EMPTY_CELL)//if this cell is empty.
				{
					result = NOT_FINISH; // found a cell with -1 , board is not filled yet
					if (possibilities[LineNumber][ColumnNumber]->size == 1)//if there is one possibility.
					{
						OptionToAdd = possibilities[LineNumber][ColumnNumber]->arr[0]; // first take the possibillity

						FoundCell = TRUE;//found a new cell with one option, need to run the loop for another round
						CheckIfLegal = SearchAndDelPossibillityInArray(LineNumber, ColumnNumber, possibilities, OptionToAdd); // if SearchAndDel returns FALSE, it means that there is at least one array with a size of 1, that need to delete an option from it, hence it will be empty array (FINISH_FAILURE case)

						if (CheckIfLegal == TRUE)//check if there is no duplicates, if CheckIfLegal is FALSE, stop the program as it means FINSIH FAILURE.
						{
							board[LineNumber][ColumnNumber] = OptionToAdd; // update that cell in the board with the option available found


							FreeSlotInPos(possibilities, LineNumber, ColumnNumber);//free the cell in pos.
						}
						else//duplicate.
							return FINISH_FAILURE;

					}
					else
					{// if the size of the possibillities array on that cell is not 1 but it is exists, check if its the minimal one

						if (MinimalPos > possibilities[LineNumber][ColumnNumber]->size)
						{
							MinimalPos = possibilities[LineNumber][ColumnNumber]->size;
							*x = LineNumber;
							*y = ColumnNumber;
						}
					}
				}

			}
		}
	}
	return (result);

}





/*********************************************Function responsible for detecting the options on different cells, deleting them and free them*****************/
//free slot in possibilities.  
void FreeSlotInPos(ARRAY *** possibilities, unsigned int row, unsigned int col)
{
	free(possibilities[row][col]->arr);

	possibilities[row][col] = NULL;
}


int SearchAndDelPossibillityInArray(unsigned int LineNumber, unsigned int ColNumber, ARRAY*** Possibillities, short OptionToFind)
{
	int DupFound; // Dupfound will tell if a duplicated option has been found, meaning it found an array with size of one (if Dupfound==false), with the same possibilities (FINISH FAILURE CASE)
	DupFound = SearchAndDelByRowsAndCols(LineNumber, ColNumber, Possibillities, OptionToFind);
	if (DupFound) // if no errors on the rows and cols, move on to the square
		DupFound = SearchAndDelBySquare(LineNumber, ColNumber, Possibillities, OptionToFind);
	return DupFound;

}

int static SearchAndDelByRowsAndCols(unsigned int LineNumber, unsigned int ColNumber, ARRAY*** Possibillities, short OptionToFind)
{
	unsigned int i;

	for (i = 0; i < LINECOL_SIZE; i++)
	{
		if (ColNumber != i && Possibillities[LineNumber][i] != NULL) // avoid checking your own array
		{
			if (!SearchAndUpdateOptionInArray(Possibillities[LineNumber][i], OptionToFind)) // found duplicate!
				return FALSE;
		}
	}
	//Searching on the Cols..
	for (i = 0; i < LINECOL_SIZE; i++)
	{
		if (LineNumber != i && Possibillities[i][ColNumber] != NULL) // avoid checking your own array
		{
			if (!SearchAndUpdateOptionInArray(Possibillities[i][ColNumber], OptionToFind)) // found duplicate!
				return FALSE;
		}
	}
	return TRUE;
}

int static SearchAndDelBySquare(unsigned int LineNumber, unsigned int ColNumber, ARRAY*** Possibillities, short OptionToFind)
{
	unsigned int i, j;
	unsigned int Line = FIRST_SQR_CELL(LineNumber);		// get the most upper left cell on the square
	unsigned int Col = FIRST_SQR_CELL(ColNumber);
	for (i = Line; i < Line + SQR_SIZE; i++)
	{// will run on 3 lines in the board
		for (j = Col; j < Col + SQR_SIZE; j++)
		{//will on 3 cells on each line

			if (Possibillities[i][j] != NULL && (j != ColNumber || i != LineNumber))
			{// if there is an array onthat option and I am not checking myself, check if that array also has the same option we are looking for

				if (!SearchAndUpdateOptionInArray(Possibillities[i][j], OptionToFind)) // found duplicate!
					return FALSE;

			}

		}

	}
	return TRUE;
}
static int SearchAndUpdateOptionInArray(ARRAY* Possibillities, short OptionToFind)
{
	int matchFound = FALSE;
	unsigned short i;
	for (i = 0; i < Possibillities->size && !matchFound; i++)
	{
		if (Possibillities->arr[i] == OptionToFind)
		{
			if (Possibillities->size == 1)
				return FALSE; // if the size was one, it means FINISH FAILURE (because if we will delete this cell, there will be no more options left)
			matchFound = TRUE; // matchFound will detect if we found an array that contains the option we are looking for
			UpdatePossibillityForCell(Possibillities, OptionToFind);
		}
	}
	return TRUE;
}
static void UpdatePossibillityForCell(ARRAY *CellToUpdate, short OptionToDel)
{
	short * newarr, *oldarr;
	unsigned int read, write;
	oldarr = CellToUpdate->arr;

	AllocateSolutionArray(&newarr, (CellToUpdate->size) - 1, "UpdatePossibillityForCell");
	for (write = 0, read = 0; read < CellToUpdate->size; read++) //copy the possiblities wihout num.
	{
		if (oldarr[read] != OptionToDel)
			newarr[write++] = oldarr[read];
	}
	free(oldarr);
	CellToUpdate->arr = newarr;
	(CellToUpdate->size)--;

	return;

}


/******************************************FillBoard*********************************************************************************************************************/
int FillBoard(short board[][9], ARRAY ***possibilities)
{
	int x, y, result, NotDup;
	short option;

	result = NOT_FINISH;
	NotDup = TRUE;
	while (result == NOT_FINISH)
	{
		sudokoPrintBoard(NULL, board);

		result = OneStage(board, possibilities, &x, &y);

		if (result == NOT_FINISH)
		{
			PRINT_CURRENT_MIN_CELL(x, y);
			option = ChooseOption(possibilities[x][y]);//return the option that the user input. 

			PRINT_CELL_UPDATED(x, y, option);
			board[x][y] = option;

			NotDup = SearchAndDelPossibillityInArray(x, y, possibilities, option);
			if (!NotDup)
				result = FINISH_FAILURE;

			FreeSlotInPos(possibilities, x, y);
		}
	}


	return result;

}


//This function print options to the user and return his legal chosen option.
int ChooseOption(ARRAY * possibilities)
{
	unsigned int  index;
	int num;

	char Legal = FALSE;

	for (index = 0; index < possibilities->size; index++)
		printf("%d.%d\n", index + 1, possibilities->arr[index]);


	while (!Legal)
	{

		scanf("%d", &num);

		for (index = 0; index < possibilities->size; index++)
		if (num == possibilities->arr[index])
			Legal = TRUE;


		if (!Legal)
			printf("You Input a ILegal Option Please Try Again\n");

	}

	return num;
}



/**********************************************initializationBoard*****************************************************************/

//This function Lottery a Number for the solutioncell.
static short LotteryNumber(ARRAY * solutioncell)
{
	short i;

	srand((unsigned int)time(NULL));

	i = rand() % solutioncell->size + 1;

	return solutioncell->arr[i - 1];

}

//This fuction return the currect possion of the cell.
static LCNODE * GetTheCurPossition(ListCord ListCoordinate, LCNODE * prev)
{
	LCNODE * cur;

	if (!prev)//check if the cell he is the head of the list.
		cur = ListCoordinate.head;
	else
		cur = prev->next;

	return cur;
}

//This function get a list of coordinate and index of cell and return the prev cell on that index.
static LCNODE *  FindSlotInList(ListCord lst, unsigned int numberslot)
{
	unsigned int index;
	LCNODE * cur, *prev;

	prev = NULL;
	cur = lst.head;
	index = 1;

	while (cur)
	{
		if (index == numberslot)
			return prev;
		prev = cur;
		cur = cur->next;
		index++;
	}

	return prev;
}


//This function reset the sudoku board to -1, so the sudoku is empty.
static void CreatEmptySoduko(short sudokuBoard[][LINECOL_SIZE])
{
	short row, col;

	for (row = 0; row < LINECOL_SIZE; row++)

	for (col = 0; col < LINECOL_SIZE; col++)
		sudokuBoard[row][col] = EMPTY_CELL;

}

short initializationBoard(short sudokuBoard[][LINECOL_SIZE])
{
	short Notpossibillities[AMOUNT_OF_POSSIBILLITIES + 1] = { 0 }, RowNumber, ColNumber, ChosenNumber;
	ARRAY * solutioncell;
	short HowManySlots, Slot, SlotIndex, disOptions;
	ListCord ListCoordinate;
	LCNODE * PrevRandomCoordinate, *CurRandomCoordinate;

	srand((unsigned int)time(NULL));

	CreatEmptySoduko(sudokuBoard);//reset the soduko to empty.

	ListCoordinate = CreateCordinateList();//create new coordinate list.

	HowManySlots = rand() % 20 + 1;//get random number between 1-20.how many cells we need to fill.



	for (SlotIndex = 0; SlotIndex < HowManySlots; SlotIndex++)
	{
		Slot = rand() % ListCoordinate.size + 1;//get random number between 1-size of the list.

		PrevRandomCoordinate = FindSlotInList(ListCoordinate, Slot);//get the prev of that index.

		if (ListCoordinate.tail == PrevRandomCoordinate)//check if not found the cell.
		{
			printf("Error not found cell in the list!!\n");
			exit(ERROR_CODE);
		}

		CurRandomCoordinate = GetTheCurPossition(ListCoordinate, PrevRandomCoordinate);//get the currect possition of the cell in the list.

		ColNumber = CurRandomCoordinate->data.Col;//get the row.
		RowNumber = CurRandomCoordinate->data.Row;//get the col.

		DeleteFromList_ListCord(&ListCoordinate, PrevRandomCoordinate);//delete this cell from the list of coordinate.

		//get what not the options of this cell.
		disOptions = CheckPossibillities(Notpossibillities, sudokuBoard, RowNumber, ColNumber); // check the possibilities in a line and colum.
		disOptions += CheckPossibillitiesBySquare(Notpossibillities, sudokuBoard, FIRST_SQR_CELL(RowNumber), FIRST_SQR_CELL(ColNumber)); // check the possibilities in a square

		if (disOptions == AMOUNT_OF_POSSIBILLITIES)//check if there is no POSSIBILLITIES at all.
		{
			printf("Error There is no POSSIBILLITIES for this cell!?!\n");
			exit(ERROR_CODE);
		}


		InsertPossibillites(&solutioncell, Notpossibillities);//insert into solutioncell the possibilities of this cell.

		ChosenNumber = LotteryNumber(solutioncell);//chose Randomly one of the possibilities.

		sudokuBoard[RowNumber][ColNumber] = ChosenNumber;
	}

	free(solutioncell);//free soultion array.
	FreeList_ListCord(&ListCoordinate);//free coordinate list.


	return HowManySlots;
}
