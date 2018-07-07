#include <stdio.h>
#include <stdlib.h>
#include "PlayersHeader.h"

static PlayerTNode *BuildTreeFromArrayHelper(PLAYERSNODE **arr, unsigned int size);
static PlayerTNode *AllocatePlayerTNode(PLAYERSNODE **Data, char *function);
static void FreeTreePlayersHelper(PlayerTNode * tr);


static PlayerTNode *AllocatePlayerTNode(PLAYERSNODE **Data, char *function)
{//Allocates a node for the players tree
	PlayerTNode *result;
	result = (PlayerTNode *)malloc(sizeof(PlayerTNode));
	if (result == NULL)
		ERRORMSG(function);
	result->data = Data;
	result->left = result->right = NULL;

	return result;
}
void BuildTreeFromArray(PlayersTree* tree, PLAYERSNODE **arr,unsigned int size)
{// Builds a tree from an array
	tree->root = BuildTreeFromArrayHelper(arr, size);
	return;
}
static PlayerTNode *BuildTreeFromArrayHelper(PLAYERSNODE **arr, unsigned int size)
{
	{
		PlayerTNode *res;
		if (size <= 1) // if size is 1 or less,check if the last cell is a valid cell as well 
		{
			if (size == 1 && arr[INITIAL_SIZE] != NULL)
			{
				res = AllocatePlayerTNode(&arr[INITIAL_SIZE], "BuildTreeFromArrayHelper");
				return(res);
			}
			else
				return NULL;
		}
		else
		{
			if (arr[size / 2] != NULL)
			{//if the value on that cell is not NULL , add it to the tree 
				res = AllocatePlayerTNode(&arr[size / 2], "BuildTreeFromArrayHelper");
				res->left = BuildTreeFromArrayHelper(arr, size / 2); // send the left side of the array to the recursion
				res->right = BuildTreeFromArrayHelper(arr + (size / 2) + 1, size / 2); // send the right side of the array to the recursion
				return(res);
			}
			else
				return NULL;
		}
	}
}

void freeTreeNode(PlayerTNode * node)
{
	free(node);
}

void FreeTreePlayers(PlayersTree* tr)
{
	FreeTreePlayersHelper(tr->root);
}

static void FreeTreePlayersHelper(PlayerTNode * tr)
{
	if (tr == NULL)
		return;
	if (tr->left)
		FreeTreePlayersHelper(tr->left);

	
	if (tr->right)
		FreeTreePlayersHelper(tr->right);

		freeTreeNode(tr);

}



