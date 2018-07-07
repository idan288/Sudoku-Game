#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PlayersHeader.h"

static void AddToEmptyList_LISTPLAYERS(LISTPLAYERS *list, PLAYERSNODE* NodeToAdd);
static void DeleteFromTheBegginningOfList_LISTPLAYERS(LISTPLAYERS *list);
static void DeleteFromTheEndOfList_LISTPLAYERS(LISTPLAYERS *list, PLAYERSNODE *p);
static void DeleteFromInnerPlaceOfList_LISTPLAYERS(PLAYERSNODE *PlaceToDelete);
static void DeallocateListNode_PLAYERSLIST(PLAYERSNODE *CellToDelete);


void makeEmptyList_LISTPLAYERS(LISTPLAYERS* result)
{// creates an empty list
	result->head = result->tail = NULL;
	result->size = 0;
}

int ListIsEmptyList_LISTPLAYERS(LISTPLAYERS *list)
{//Checks if a List is empty and returns 0 or 1
	if (list->head == NULL)
		return (TRUE);
	else
		return (FALSE);
}

void AddToEndOfList_LISTPLAYERS(LISTPLAYERS *list, PLAYER* DataToAdd)
{//Adds the data on the end of the list only
	PLAYERSNODE* NodeToAdd = AllocateListNode_LISTPLAYERS(DataToAdd);

	if (ListIsEmptyList_LISTPLAYERS(list)) // list is empty - private case
		AddToEmptyList_LISTPLAYERS(list, NodeToAdd);
	else
	{// add the node after the tail
		list->tail->next = NodeToAdd;
		list->tail = NodeToAdd;
	}
	(list->size)++;

}
static void AddToEmptyList_LISTPLAYERS(LISTPLAYERS *list, PLAYERSNODE* NodeToAdd)
{// adds data to an empty list
	list->head = list->tail = NodeToAdd;
	return;
}
PLAYERSNODE *AllocateListNode_LISTPLAYERS(PLAYER* DataToAdd)
{//allocates a player's list cell
	PLAYERSNODE *result;
	result = (PLAYERSNODE*)malloc(sizeof(PLAYERSNODE));
	if (!result)
		ERRORMSG("AllocateListNode_LISTPLAYERS")
		result->next = NULL;
	result->data = *DataToAdd;
	return(result);

}
void FreeList_LISTPLAYERS(LISTPLAYERS *list) 
{// free the entire list 
	PLAYERSNODE*curr = list->head, *temp;
	while (curr)
	{
		temp = curr->next;
		DeallocateListNode_PLAYERSLIST(curr);
		curr = temp;
	}
	list->head = list->tail = NULL;
	return;

}

void DeleteFromList_LISTPLAYERS(LISTPLAYERS *list, PLAYER *DataToDelete)
{//finds the cell to delete and deletes it
	
	PLAYERSNODE *PlaceToDelete = FindPlaceToDelete_LISTPLAYERS(list, DataToDelete);
	//Now we will check which case fits PlaceToInsert
	if (PlaceToDelete == NULL) // delete is in the head
		DeleteFromTheBegginningOfList_LISTPLAYERS(list);
	else if (PlaceToDelete->next == list->tail) // deletion in the end
		DeleteFromTheEndOfList_LISTPLAYERS(list, PlaceToDelete);
	else
		DeleteFromInnerPlaceOfList_LISTPLAYERS(PlaceToDelete); // deletion in the middle
	(list->size)--;
}

static void DeleteFromTheBegginningOfList_LISTPLAYERS(LISTPLAYERS *list)
{//deletes a cell in the begin of list
	PLAYERSNODE *temp = list->head;
	list->head = list->head->next;
	DeallocateListNode_PLAYERSLIST(temp);
	if (list->head == NULL)
	{
		list->tail = NULL;
	}

}
static void DeleteFromTheEndOfList_LISTPLAYERS(LISTPLAYERS *list, PLAYERSNODE *p)
{// deletes a cell in the end of list
	DeallocateListNode_PLAYERSLIST(list->tail);
	list->tail = p;
	p->next = NULL;

}
static void DeleteFromInnerPlaceOfList_LISTPLAYERS(PLAYERSNODE *PlaceToDelete)
{//deletes a cell in the middle of the list
	PLAYERSNODE *del_cell = PlaceToDelete->next;
	PlaceToDelete->next = del_cell->next;
	DeallocateListNode_PLAYERSLIST(del_cell);
}
void CreatePlayersList(LISTPLAYERS *list)
{//Creates a list of players
	short board[9][9] =
	{ 5, -1, 4, -1, 7, -1, -1, 1, -1,
	6, -1, 2, 1, -1, -1, 3, -1, -1,
	1, -1, 8, -1, 4, -1, -1, 6, -1,
	-1, 5, -1, -1, 6, -1, -1, 2, -1,
	-1, 2, -1, 8, -1, 3, -1, -1, -1,
	-1, -1, -1, -1, -1, 4, -1, 5, 6,
	-1, 6, 1, 5, 3, 7, 2, 8, 4,
	-1, 8, 7, -1, 1, 9, -1, 3, -1,
	-1, -1, -1, 2, 8, -1, -1, -1, 9 };
	PLAYER PlayerToAdd;
	unsigned int i, size = 0;
	makeEmptyList_LISTPLAYERS(list);
	
	printf("Please enter the amount of players you would like to add\n");
	scanf("%u", &size);
	fflush(stdin);
	for (i = 0; i < size; i++)
	{
		printf("Please enter your name:\n");
		gets(PlayerToAdd.name); // get the player name
		PlayerToAdd.FilledCellsOnStart = 10;
			//initializationBoard(PlayerToAdd.board); // get the initialized board after it is filled with initial values, will return the amount of cells on the board which has been filled

		
		for (int k = 0; k < 9; k++)
			for (int p = 0; p < 9; p++)
			PlayerToAdd.board[k][p] = board[k][p];
		
	
		PlayerToAdd.PossibleDigits = PossibleDigits(PlayerToAdd.board); // will return an array of possible digits for each cell in the board
		AddToEndOfList_LISTPLAYERS(list, &PlayerToAdd); // add the new player to the end of list
	}
	printf("The game has added %u players successfully\n", list->size);
	return;
}
PLAYERSNODE *FindPlaceToDelete_LISTPLAYERS(LISTPLAYERS *list, PLAYER* DataToDelete)
{//finds the cell needed to be deleted, according to its data
	PLAYERSNODE *curr = list->head, *prev = NULL;
	while (curr && (&(curr->data)!=DataToDelete)) // as long as the address of the cells we would like to delete is different from the address we are currently reading
	{
		prev = curr;
		curr = curr->next;
	}
	return (prev);
}
static void DeallocateListNode_PLAYERSLIST(PLAYERSNODE *CellToDelete)
{//removes a list from the list
	
	if (CellToDelete->data.PossibleDigits!=NULL) // if this user has a possible digits array, need to free it first
	   FreePossibillitiesArray(CellToDelete->data.PossibleDigits);
	free(CellToDelete); // all other fields are static, just free the player now

}

void FreePossibillitiesArray(ARRAY *** PossibleDigits)
{//This function delete whole PossibleDigits array for a certain player
	short row, col;
	

	for (row = 0; row < LINECOL_SIZE; row++)
	{
		for (col = 0; col < LINECOL_SIZE; col++)
			if (PossibleDigits[row][col] != NULL) // free the array on that adress
			FreeSlotInPos(PossibleDigits, row, col);
		free(PossibleDigits[row]); // free the pointer itself
		PossibleDigits[row] = NULL;
	}
		
	free(PossibleDigits);
	
	PossibleDigits = NULL;

}