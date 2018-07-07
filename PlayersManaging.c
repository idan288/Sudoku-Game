#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "PlayersHeader.h"
#include "BoardHeader.h"
static void MovePlayersToWinnings(LISTPLAYERS *CurrPlayersList, LISTPLAYERS * WinningPlayers, PLAYERSNODE * player);
static PLAYERSNODE **AllocatePlayersArray(unsigned int size, char *function);
static PLAYERSNODE** CreateSortedPlayersArray(LISTPLAYERS *list);
static void SortPlayersArray(PLAYERSNODE **arr, unsigned int size);
static void SortPlayersArrayHelper(PLAYERSNODE **Arr1, unsigned int size1, PLAYERSNODE **Arr2, unsigned int size2, PLAYERSNODE **ArrayToFill);
static PLAYERSNODE **ExtendPlayersArray(PLAYERSNODE **ArrayToExtend, unsigned int *OldSize);
static void ScanPlayersTreeLDRHelper(PlayerTNode* root, LISTPLAYERS *CurrPlayersList, LISTPLAYERS *WinningPlayers);
static void CheckResultOfOneStage(PlayerTNode* root, LISTPLAYERS *CurrPlayersList, LISTPLAYERS *WinningPlayers, PLAYERSNODE * player);
static void sleep(unsigned int mseconds);


/****************************Functions that creates the players array, sort it, and extending it as needed****************/
static PLAYERSNODE **AllocatePlayersArray(unsigned int size, char *function)
{//Allocates a Ptrs array to list nodes
	PLAYERSNODE **result;
	result = (PLAYERSNODE **)calloc(size, sizeof(PLAYERSNODE *));
	if (result == NULL)
		ERRORMSG(function)

		return result;

}

static PLAYERSNODE** CreateSortedPlayersArray(LISTPLAYERS *list)
{//Sorts Ptr array
	PLAYERSNODE **result, *curr = list->head;
	unsigned int i;
	result = AllocatePlayersArray(list->size, "CreateSortedPlayersArray"); // First Allocate size pointers
	//Point on the list's node
	for (i = 0; i < list->size; i++)
	{
		result[i] = curr;
		curr = curr->next;
	}

	SortPlayersArray(result, list->size);
	return(result);
}

static void SortPlayersArray(PLAYERSNODE **arr, unsigned int size)
{//sorts the players list by the amount of filled cells each player has on his board (min to max), second sorting properties will be the name of the player
	//MergeSort algorithm
	PLAYERSNODE **temp;
	unsigned int i;
	if (size <= 1)
		return;
	else
	{
		SortPlayersArray(arr, size / 2);
		SortPlayersArray(arr + size / 2, size - size / 2);
		temp = AllocatePlayersArray(size, "SortPlayersArray");
		SortPlayersArrayHelper(arr, size / 2, arr + size / 2, size - size / 2,temp);
		for (i = 0; i < size; i++)
			arr[i] = temp[i];
		free(temp);
		return;

	}
}
static void SortPlayersArrayHelper(PLAYERSNODE **Arr1, unsigned int size1, PLAYERSNODE **Arr2, unsigned int size2, PLAYERSNODE **ArrayToFill)
{//SortedMerge algorithm
	unsigned int read1 = 0, read2 = 0, write = 0;
	while (read1 < size1 && read2 < size2)
	{
		if (Arr1[read1]->data.FilledCellsOnStart < Arr2[read2]->data.FilledCellsOnStart) // if the amount of filled cells that player1 has is lower than the amount of filled cells player 2 has, first add to the sorted array- plaeyr 1
			ArrayToFill[write++] = Arr1[read1++];
		else if (Arr1[read1]->data.FilledCellsOnStart > Arr2[read2]->data.FilledCellsOnStart) // if the above is otherwise, add first player 2
			ArrayToFill[write++] = Arr2[read2++];
		else
		{// else - if both of the players has the same amount of filled cells, choose the first one by their names
			if (strcmp(Arr1[read1]->data.name, Arr2[read2]->data.name) < 0)
				ArrayToFill[write++] = Arr1[read1++];
			else
				ArrayToFill[write++] = Arr2[read2++];
		}
	}
	// take all the leftovers
	while (read1<size1)
		ArrayToFill[write++] = Arr1[read1++];
	while (read2<size2)
		ArrayToFill[write++] = Arr2[read2++];
	return;
}

static PLAYERSNODE ** ExtendPlayersArray(PLAYERSNODE **ArrayToExtend, unsigned int *OldSize)
{// Extending the players array to (2^log2(x+1)) -1 and returns the new size by ref to update the players list
	PLAYERSNODE **result; // returned value
	unsigned int newSize,i;
	double power = ceil((log((double)*OldSize+1) / log((double)(BASIC_LOG)))); // calculates the power itself
	double mathExpression = pow(BASIC_LOG, power); // calulate the entire expression (2^power)


	newSize = (unsigned int)(mathExpression)-1; // get the new size and convert it to unsigned int, reduce 1
	result = AllocatePlayersArray(newSize, "ExtendPlayersArray");
	for (i = 0; i < *OldSize; i++) // copy the values at the Array needed to be extended to the new one
		result[i] = ArrayToExtend[i];

	free(ArrayToExtend);// delete the old array
	*OldSize = newSize; // update the size
	return(result);

}

/*************************************************************************************************************************/


void CreatePlayersDatabase(PlayersTree *tree, LISTPLAYERS *list, PLAYERSNODE ***arr)
{//function that creates the entire database for the players
	unsigned int size;
	CreatePlayersList(list); // get the players frm the user and put them in a singly list
	*arr = CreateSortedPlayersArray(list); // creates a sorted array of the players
	size = list->size; // save the current amount of players and send it to ExtendPlayersArray in order to build the tree correctly
	*arr = ExtendPlayersArray(*arr, &size); // Extends the sorted Ptrs array
    BuildTreeFromArray(tree, *arr, size); // Builds the tree of players
	return;
}



/*****************Functions that responsible for scanning the tree of players and act according to the scanning results*************************/
void ScanPlayersTreeLDR(PlayersTree *tree, LISTPLAYERS *CurrPlayersList, LISTPLAYERS *WinningPlayers)
{
	ScanPlayersTreeLDRHelper(tree->root,CurrPlayersList,WinningPlayers);
}
static void ScanPlayersTreeLDRHelper(PlayerTNode* root, LISTPLAYERS *CurrPlayersList, LISTPLAYERS *WinningPlayers)
{
	PLAYERSNODE * player;
	if (root == NULL)
		return; // root is empty - return
	else
	{
		if (root->left) // if left subtree exists
		ScanPlayersTreeLDRHelper(root->left,CurrPlayersList,WinningPlayers);//go left.
		
		
		player = *(root->data);//get the player node.

		if (player)//if there is data player.
		{
			printf("\n\nNow is %s Turn to Play.\n\n", player->data.name);

			CheckResultOfOneStage(root, CurrPlayersList, WinningPlayers, player);
		}
		if (root->right) // if right subtree exists
		ScanPlayersTreeLDRHelper(root->right, CurrPlayersList, WinningPlayers);
	}
}

//This function use the answer from onestage and accoding the answer...

static void CheckResultOfOneStage(PlayerTNode* root, LISTPLAYERS *CurrPlayersList, LISTPLAYERS *WinningPlayers, PLAYERSNODE * player)
{
	unsigned int x, y; // will hold the returned value from one strage
	int OptionChosen,result;

	result = OneStage(player->data.board, player->data.PossibleDigits, &x, &y);
	sudokoPrintBoard(NULL, player->data.board);
	if (result == FINISH_SUCCESS)//the board if filled for that player - he won.
	{
		PRINT_WINNER(player->data.name);
		sleep(3000); // pause the screen for 3 seconds
		MovePlayersToWinnings(CurrPlayersList, WinningPlayers, player);//move the wining player to the wining list.
		*(root->data)= NULL; // remove him from the tree
	}
	else if (result == NOT_FINISH)//The board not filled yet,the user need to chose one option.
	{
		PRINT_CURRENT_MIN_CELL(x, y); // print the current minimal cell
		OptionChosen = ChooseOption(player->data.PossibleDigits[x][y]);//return the option that the user input. 

		PRINT_CELL_UPDATED(x, y, OptionChosen);  // print the option chosen by the user
		player->data.board[x][y] = OptionChosen;//fill the board with the user input.
		
		result = SearchAndDelPossibillityInArray(x, y, player->data.PossibleDigits, OptionChosen);//update the new possibilties.

		sudokoPrintBoard(NULL,player->data.board); // print the board again

		if (!result)//found Duplicates in the board during the update of the board, players cannot continue playing.
			result = FINISH_FAILURE;
		else//not found duplicate, remove the possible digits cell in line X , col Y
			FreeSlotInPos(player->data.PossibleDigits, x, y);		   	
	}
	if (result == FINISH_FAILURE) // need to check that case again since after we update one cell in the board, the player might lose!
	{
		PRINT_LOSER(player->data.name);
		sleep(3000);// pause the screen for 3 seconds
		DeleteFromList_LISTPLAYERS(CurrPlayersList, &player->data);//delete the player from the list.
		
		*(root->data) = NULL; // delete the player and put NULL on that ptr
	}
	return;
}


static void MovePlayersToWinnings(LISTPLAYERS *CurrPlayersList, LISTPLAYERS * WinningPlayers, PLAYERSNODE * player)
{//Moves a player to the winner's list from the ucrrent player list
	PLAYER Data;

	Data = player->data; // temp hold the player's data
	Data.PossibleDigits = NULL; // this player has a filled board, no need to save a possible digits field	

	DeleteFromList_LISTPLAYERS(CurrPlayersList, &player->data);//delete the player from the cuurrently playing list.

	AddToEndOfList_LISTPLAYERS(WinningPlayers, &Data);//copy the player to the winninig list.
}

//This function print the Winning Players on file.
void PrintWinListToFile(LISTPLAYERS WinningPlayers)
{
	FILE * WinFile;
	PLAYERSNODE * cur;

	cur = WinningPlayers.head;
	short NumPlayer = 1;

	WinFile = fopen("WiningList.txt", "w");

	if (!WinFile)
		ERRORMSG("PrintWinListToFile");

	fprintf(WinFile, "And.. The Winneres Players are:\n\n");

	while (cur)
	{
		fprintf(WinFile, "The Player in Place %d is %s.\n\n", NumPlayer, cur->data.name);
		fprintf(WinFile, "And his Sudoku Board is:\n\n");
		sudokoPrintBoard(WinFile, cur->data.board);

		cur = cur->next;
		NumPlayer++;
	}



	fclose(WinFile);
}
static void sleep(unsigned int mseconds)
{
	clock_t goal = mseconds + clock();
	while (goal > clock());
}