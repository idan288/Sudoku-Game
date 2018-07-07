#ifndef _LISTCORD_

#define _LISTCORD_

/************************Defines*****************/
#define ERROR_CODE -1
#define TRUE 1
#define FALSE 0
#define LINECOLSIZE 9
#define ERRORMSG(FUNCTION){																	\
	fprintf(stderr, "ERROR!! Failed to allocate capacity in %s", FUNCTION);					\
	exit(ERROR_CODE);																		\
}

/*********************Data Strcutures************************/

typedef struct cordinate{
	short Row;
	short Col;
}CORDINATE;

typedef struct _LCNODE{
	struct _LCNODE *next;
	CORDINATE data;
}LCNODE;

typedef struct _listcord{
	LCNODE *head;
	LCNODE *tail;
	unsigned int size;
}ListCord;

/****************************Functions prototypes**********************/

void makeEmptyList_ListCord(ListCord* result);
int ListIsEmptyList_ListCord(ListCord *list);
void AddToEndOfList_ListCord(ListCord *list, CORDINATE DataToAdd);
void makeEmptyList_ListCord(ListCord* result);
void FreeList_ListCord(ListCord *list);
ListCord CreateCordinateList();
void DeleteFromList_ListCord(ListCord *list, LCNODE *PlaceToDelete);


#endif