// generated by "sampctl package init"
#include <open.mp>
#include <streamer>

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

#include "item_extra_id.inc"

main() 
{
	// write code here and run "sampctl package build" to compile
	// then run "sampctl package run" to run it
	new STREAMER_TAG_AREA:areaId, EXTRA_ID_TYPE:type, value;
	areaId = CreateDynamicCircle(0.0, 0.0, 50.0);
	Streamer_SetItemExtraId(STREAMER_TYPE_AREA, areaId, EXTRA_ID_TYPE_BUSINESS, 1337);
	Streamer_GetItemExtraId(STREAMER_TYPE_AREA, areaId, type, value);
	printf("Item Id: %d - ExtraId Type: %d - ExtraId Value: %d", _:areaId, _:type, value);
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