
#include "ListCord.h"
#include "BoardHeader.h"
#include "PlayersHeader.h"


void StartPlaying();


void main()
{
	StartPlaying();
}
void StartPlaying()
{
	PlayersTree tr;
	LISTPLAYERS PlaynigPlayers, WinningPlayers;
	PLAYERSNODE **PlayersArray;

	CreatePlayersDatabase(&tr, &PlaynigPlayers, &PlayersArray);
	makeEmptyList_LISTPLAYERS(&WinningPlayers);

	while (PlaynigPlayers.size != 0)//goes until the list of currect playing players not empty.
	{
		ScanPlayersTreeLDR(&tr, &PlaynigPlayers, &WinningPlayers);
	}

	FreeTreePlayers(&tr);
	FreeList_LISTPLAYERS(&PlaynigPlayers);
	free(PlayersArray);

	if (WinningPlayers.size>0)//if there is win player.
		PrintWinListToFile(WinningPlayers);

	FreeList_LISTPLAYERS(&WinningPlayers);
}