#ifndef _PLAYERS_HEADER_
#define _PLEAYERS_HEADER_

#include "BoardHeader.h"
/************************Defines*****************/
#define ERROR_CODE -1
#define TRUE 1
#define FALSE 0
#define LINECOL_SIZE 9
#define EMPTY_CELL -1
#define BASIC_LOG 2 // for extending the array
#define INITIAL_SIZE 0
#define PRINT_WINNER(X) printf("\nCongratulations %s , you won the game\n Moving %s to the winners list...\n", X, X);
#define PRINT_LOSER(X) printf("\nUnfortunately %s Lost the game...\nDeleting %s from the players list..\n", X, X)
#define ERRORMSG(FUNCTION){																	\
	fprintf(stderr, "ERROR!! Failed to allocate capacity in %s", FUNCTION);					\
	exit(ERROR_CODE);																		\
}

/*********************Data Strcutures************************/

// Players List

typedef struct _PLAYER
{
	char name[NAMESIZE + 1];
	short board[LINECOL_SIZE][LINECOL_SIZE];
	ARRAY *** PossibleDigits;
	short FilledCellsOnStart;
}PLAYER;

typedef struct _PLAYERSNODE
{
	PLAYER data;
	struct _PLAYERSNODE * next;
}PLAYERSNODE;


typedef struct _LISTPLAYERS
{
	PLAYERSNODE * head;
	PLAYERSNODE * tail;
	unsigned int size;
}LISTPLAYERS;

//Players tree
typedef struct playertnode{
	struct playertnode *left;
	struct playertnode *right;
	PLAYERSNODE **data;
}PlayerTNode;

typedef struct playerstree{
	PlayerTNode *root;
}PlayersTree;

/****************************Functions prototypes**********************/

/**************Functions responsible for creating and managing the players list**************************************/
/*************************************Under PlayersListCreation.c**************************/
void makeEmptyList_LISTPLAYERS(LISTPLAYERS* result);
int ListIsEmptyList_LISTPLAYERS(LISTPLAYERS *list);
void AddToEndOfList_LISTPLAYERS(LISTPLAYERS *list, PLAYER* DataToAdd);
PLAYERSNODE *AllocateListNode_LISTPLAYERS(PLAYER* DataToAdd);
void FreeList_LISTPLAYERS(LISTPLAYERS *list);
void DeleteFromList_LISTPLAYERS(LISTPLAYERS *list, PLAYER *DataToDelete);
void CreatePlayersList(LISTPLAYERS *list);
PLAYERSNODE *FindPlaceToDelete_LISTPLAYERS(LISTPLAYERS *list, PLAYER* DataToDelete);
void FreePossibillitiesArray(ARRAY *** PossibleDigits);


/*********************************Functions responsible for creating the players database and scan during the game till it finishes******************/

/***********************************Under PlayersManaging.c***********************************/
void CreatePlayersDatabase(PlayersTree *tree, LISTPLAYERS *list, PLAYERSNODE ***arr);
void ScanPlayersTreeLDR(PlayersTree *tree, LISTPLAYERS *CurrPlayersList, LISTPLAYERS *WinningPlayers);
void PrintWinListToFile(LISTPLAYERS WinningPlayers);

/******************Functions which build the players tree from the array and also functions which deletes the tree nodes if needed**********/
/**************Under PlayersTree.c******/
void BuildTreeFromArray(PlayersTree* tree, PLAYERSNODE **arr, unsigned int size);
void FreeTreePlayers(PlayersTree* tr);



#endif