#include "BoardHeader.h"
#include <stdio.h>
#include <stdlib.h>

static void AddToEmptyList_ListCord(ListCord *list, LCNODE* NodeToAdd);
static void DeleteFromTheBegginningOfList_ListCord(ListCord *list);
static void DeleteFromTheEndOfList_ListCord(ListCord *list, LCNODE *p);
static void DeleteFromInnerPlaceOfList_ListCord(LCNODE *PlaceToDelete);
static LCNODE *AllocateListNode_ListCord(CORDINATE DataToAdd);


void makeEmptyList_ListCord(ListCord* result)
{// creates an empty list
	result->head = result->tail = NULL;
	result->size = 0;
}

int ListIsEmptyList_ListCord(ListCord *list)
{//Checks if a List is empty and returns 0 or 1
	if (list->head == NULL)
		return (TRUE);
	else
		return (FALSE);
}
void AddToEndOfList_ListCord(ListCord *list, CORDINATE DataToAdd)
{//Adds the data on the end of the list only
	LCNODE* NodeToAdd = AllocateListNode_ListCord(DataToAdd);
	if (ListIsEmptyList_ListCord(list))
		AddToEmptyList_ListCord(list, NodeToAdd);
	else
	{
		list->tail->next = NodeToAdd;
		list->tail = NodeToAdd;
	}
	(list->size)++;

}
static void AddToEmptyList_ListCord(ListCord *list, LCNODE* NodeToAdd)
{// adds data to an empty list
	list->head = list->tail = NodeToAdd;
	return;
}
static LCNODE *AllocateListNode_ListCord(CORDINATE DataToAdd)
{//allocates a list cord cell
	LCNODE *result;
	result = (LCNODE*)malloc(sizeof(LCNODE));
	if (!result)
		ERRORMSG("AllocateListNode")
		result->next = NULL;
	result->data = DataToAdd;
	return(result);

}
void FreeList_ListCord(ListCord *list)
{// free the entire list 
	LCNODE*curr = list->head, *temp;
	while (curr)
	{
		temp = curr->next;
		free(curr);
		curr = temp;
	}
	list->head = list->tail = NULL;
	return;

}

void DeleteFromList_ListCord(ListCord *list, LCNODE *PlaceToDelete)
{//finds the cell to delete and deletes it
	//Now we will check which case fits PlaceToInsert
	if (PlaceToDelete == NULL) // delete is in the head
		DeleteFromTheBegginningOfList_ListCord(list);
	else if (PlaceToDelete->next == list->tail) // deletion in the end
		DeleteFromTheEndOfList_ListCord(list, PlaceToDelete);
	else
		DeleteFromInnerPlaceOfList_ListCord(PlaceToDelete); // deletion in the middle
	(list->size)--;
}

static void DeleteFromTheBegginningOfList_ListCord(ListCord *list)
{
	LCNODE *temp = list->head;
	list->head = list->head->next;
	free(temp);
	if (list->head == NULL)
	{
		list->tail = NULL;
	}

}
static void DeleteFromTheEndOfList_ListCord(ListCord *list, LCNODE *p)
{
	free(list->tail);
	list->tail = p;
	p->next = NULL;

}
static void DeleteFromInnerPlaceOfList_ListCord(LCNODE *PlaceToDelete)
{
	LCNODE *del_cell = PlaceToDelete->next;
	PlaceToDelete->next = del_cell->next;
	free(del_cell);
}
ListCord CreateCordinateList()
{//Creates a list of the cordinates in the soduko board, each cell in the soduko board has a cell in this list, presentings its coordinates
	ListCord result;
	CORDINATE cordinate;
	short LineNumber, ColNumber;
	makeEmptyList_ListCord(&result);

	//2 loops to get the coordinates X ,Y for each cell in the sudoko board
	for (LineNumber = 0; LineNumber < LINECOL_SIZE; LineNumber++)
	{
		for (ColNumber = 0; ColNumber < LINECOL_SIZE; ColNumber++)
		{
			cordinate.Row = LineNumber;
			cordinate.Col = ColNumber;
			AddToEndOfList_ListCord(&result, cordinate);
		}
	}
	return (result);
}