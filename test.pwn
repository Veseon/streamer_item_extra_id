// Definitions (Tags)
enum EXTRA_ID_TYPE
{
	EXTRA_ID_TYPE_ENTRANCE,
	EXTRA_ID_TYPE_HOUSE,
	EXTRA_ID_TYPE_BUSINESS
}

// Auto completion
#define EXTRA_ID_TYPE_ENTRANCE  (EXTRA_ID_TYPE:0)
#define EXTRA_ID_TYPE_HOUSE     (EXTRA_ID_TYPE:1)
#define EXTRA_ID_TYPE_BUSINESS  (EXTRA_ID_TYPE:2)

#include <streamer\item_extra_id>

main() 
{
	// write code here and run "sampctl package build" to compile
	// then run "sampctl package run" to run it
}

public OnGameModeInit()
{
	new STREAMER_TAG_PICKUP:pickupId;
	pickupId = CreateDynamicPickup(1239, 1, -6.2148, 29.3139, 3.1172, .worldid = 0);
	Streamer_SetItemExtraId(STREAMER_TYPE_PICKUP, pickupId, EXTRA_ID_TYPE_BUSINESS, 420);

	new STREAMER_TAG_CP:cpId;
	cpId = CreateDynamicCP(17.2306, 14.0025, 3.1172, 5.0);
	Streamer_SetItemExtraId(STREAMER_TYPE_CP, cpId, EXTRA_ID_TYPE_ENTRANCE, 69);

	new STREAMER_TAG_RACE_CP:racecpId;
	racecpId = CreateDynamicRaceCP(1, 29.0830, -10.0900, 3.1172, 0.0, 0.0, 0.0, 5.0);
	Streamer_SetItemExtraId(STREAMER_TYPE_RACE_CP, racecpId, EXTRA_ID_TYPE_HOUSE, 42);

	new STREAMER_TAG_AREA:areaId, EXTRA_ID_TYPE:type, value;
	areaId = CreateDynamicCircle(0.0000, 0.0000, 5.0);
	Streamer_SetItemExtraId(STREAMER_TYPE_AREA, areaId, EXTRA_ID_TYPE_BUSINESS, 1337);
	Streamer_GetItemExtraId(STREAMER_TYPE_AREA, areaId, type, value);
	printf("Item Id: %d - ExtraId Type: %d - ExtraId Value: %d", _:areaId, _:type, value);
}

public OnPlayerConnect(playerid)
{
	SetSpawnInfo(playerid, NO_TEAM, 0, 0.0, 0.0, 3.0, 0.0);
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	TogglePlayerSpectating(playerid, true);
	TogglePlayerSpectating(playerid, false);
	return 0;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	if(strcmp(cmdtext, "/extraid", false, 5) == 0)
	{
		new EXTRA_ID_TYPE:type, value;
		if(!Streamer_GetPlayerExtraId(playerid, type, value))
		{
			SendClientMessage(playerid, -1, "You are not in any streamer item!");
			return 1;
		}
		SendClientMessage(playerid, -1, "ExtraId Type: %d - ExtraId Value: %d", _:type, value);
		return 1;
	}
	return 0;
}