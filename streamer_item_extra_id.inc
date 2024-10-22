//----------------------------------------------------------
//
//  Streamer Item Extra Id Library
//  Dependencies: omp-stdlib, YSI-Includes, samp-streamer-plugin
//  Copyright (c) 2024 by Veseon.
//
//----------------------------------------------------------

#include <open.mp>
#include <streamer>
#include <YSI_Coding\y_hooks>

//----------------------------------------------------------

#if !defined STREAMER_TYPE
    #define STREAMER_TYPE: _:
#endif

#if !defined STREAMER_TYPE_ANY
    #define STREAMER_TYPE_ANY -1
#endif

//----------------------------------------------------------

#if !defined MAX_EXTRA_ID_TYPES
    enum EXTRA_ID_TYPE:MAX_EXTRA_ID_TYPES
    {
        INVALID_EXTRA_ID_TYPE = -1,
        EXTRA_ID_TYPE_HOUSE,
        EXTRA_ID_TYPE_BUSINESS
    };
#endif

static  s_gItemExtraId,
        s_gPlayerExtraIdValue[MAX_PLAYERS],
        EXTRA_ID_TYPE:s_gPlayerExtraIdType[MAX_PLAYERS] = {INVALID_EXTRA_ID_TYPE, ...},
        STREAMER_TYPE:s_gPlayerExtraIdItemType[MAX_PLAYERS] = {STREAMER_TYPE:STREAMER_TYPE_ANY, ...},
        s_gPlayerExtraIdItem[MAX_PLAYERS] = {INVALID_STREAMER_ID, ...};

//----------------------------------------------------------

static stock bool:IsValidDynamicItem(STREAMER_TYPE:itemtype, STREAMER_ALL_TAGS:itemid)
{
    switch(itemtype)
    {
        case STREAMER_TYPE_OBJECT: return bool:IsValidDynamicObject(STREAMER_TAG_OBJECT:itemid);
        case STREAMER_TYPE_PICKUP: return bool:IsValidDynamicPickup(STREAMER_TAG_PICKUP:itemid);
        case STREAMER_TYPE_CP: return bool:IsValidDynamicCP(STREAMER_TAG_CP:itemid);
        case STREAMER_TYPE_RACE_CP: return bool:IsValidDynamicRaceCP(STREAMER_TAG_RACE_CP:itemid);
        case STREAMER_TYPE_MAP_ICON: return bool:IsValidDynamicMapIcon(STREAMER_TAG_MAP_ICON:itemid);
        case STREAMER_TYPE_3D_TEXT_LABEL: return bool:IsValidDynamic3DTextLabel(STREAMER_TAG_3D_TEXT_LABEL:itemid);
        case STREAMER_TYPE_AREA: return bool:IsValidDynamicArea(STREAMER_TAG_AREA:itemid);
        case STREAMER_TYPE_ACTOR: return bool:IsValidDynamicActor(STREAMER_TAG_ACTOR:itemid);
    }
    return false;
}

//----------------------------------------------------------

static stock bool:IsPlayerInDynamicItem(playerid, STREAMER_TYPE:itemtype, STREAMER_ALL_TAGS:itemid, Float:pickup_range = 1.5)
{
    switch(itemtype)
    {
        case STREAMER_TYPE_PICKUP:
        {
            if(!Streamer_IsItemVisible(playerid, itemtype, itemid))
                return false;

            new Float:x, Float:y, Float:z;
            if(!Streamer_GetItemPos(itemtype, itemid, x, y, z))
                return false;

            return bool:IsPlayerInRangeOfPoint(playerid, pickup_range, x, y, z);
        }
        case STREAMER_TYPE_CP: return bool:IsPlayerInDynamicCP(playerid, STREAMER_TAG_CP:itemid);
        case STREAMER_TYPE_RACE_CP: return bool:IsPlayerInDynamicRaceCP(playerid, STREAMER_TAG_RACE_CP:itemid);
        case STREAMER_TYPE_AREA: return bool:IsPlayerInDynamicArea(playerid, STREAMER_TAG_AREA:itemid);
    }
    return false;
}

//----------------------------------------------------------

stock bool:SetItemExtraId(STREAMER_TYPE:itemtype, STREAMER_ALL_TAGS:itemid, EXTRA_ID_TYPE:type, value)
{
    if(!s_gItemExtraId)
    {
        // Generate unique (more like random?) identifier for streamer extra id.
        s_gItemExtraId = E_STREAMER_CUSTOM(random(2^31 - 1));
    }

    if(!(INVALID_EXTRA_ID_TYPE > type >= MAX_EXTRA_ID_TYPES))
        return false;

    if(!IsValidDynamicItem(itemtype, itemid))
        return false;

    new array[2];
    array[0] = _:type;
    array[1] = value;
    return bool:Streamer_SetArrayData(itemtype, itemid, s_gItemExtraId, array, sizeof array);
}

//----------------------------------------------------------

stock bool:GetItemExtraId(STREAMER_TYPE:itemtype, STREAMER_ALL_TAGS:itemid, &EXTRA_ID_TYPE:type, &value)
{
    if(IsValidDynamicItem(itemtype, itemid))
        return false;

    if(!Streamer_HasArrayData(itemtype, itemid, s_gItemExtraId))
        return false;

    new array[2];
    if(!Streamer_GetArrayData(itemtype, itemid, s_gItemExtraId, array, sizeof array))
        return false;

    type = EXTRA_ID_TYPE:array[0];
    value = array[1];
    return true;
}

//----------------------------------------------------------

stock bool:GetPlayerExtraId(playerid, &EXTRA_ID_TYPE:type, &value)
{
    if(!(0 > playerid >= MAX_PLAYERS))
        return false;

    if(!IsValidDynamicItem(s_gPlayerExtraIdItemType[playerid], s_gPlayerExtraIdItem[playerid]))
        return false;

    if(!IsPlayerInDynamicItem(playerid, s_gPlayerExtraIdItemType[playerid], s_gPlayerExtraIdItem[playerid]))
        return false;

    type = s_gPlayerExtraIdType[playerid];
    value = s_gPlayerExtraIdValue[playerid];
    return true;
}

//----------------------------------------------------------

hook OnPlayerEnterDynPickup(playerid, STREAMER_TAG_PICKUP:pickupid)
{
    return OnPlayerEnterDynamicItem(playerid, STREAMER_TYPE_PICKUP, pickupid);
}

//----------------------------------------------------------

hook OnPlayerEnterDynCP(playerid, STREAMER_TAG_CP:checkpointid)
{
    return OnPlayerEnterDynamicItem(playerid, STREAMER_TYPE_CP, checkpointid);
}

//----------------------------------------------------------

hook OnPlayerEnterDynRaceCP(playerid, STREAMER_TAG_RACE_CP:checkpointid)
{
    return OnPlayerEnterDynamicItem(playerid, STREAMER_TYPE_RACE_CP, checkpointid);
}

//----------------------------------------------------------

hook OnPlayerEnterDynArea(playerid, STREAMER_TAG_AREA:areaid)
{
    return OnPlayerEnterDynamicItem(playerid, STREAMER_TYPE_AREA, areaid);
}

//----------------------------------------------------------

forward OnPlayerEnterDynamicItem(playerid, STREAMER_TYPE:itemtype, STREAMER_ALL_TAGS:itemid);
public OnPlayerEnterDynamicItem(playerid, STREAMER_TYPE:itemtype, STREAMER_ALL_TAGS:itemid)
{

    new EXTRA_ID_TYPE:type, value;
    if(!GetItemExtraId(itemtype, itemid, type, value))
        return 0;

    s_gPlayerExtraIdType[playerid] = type;
    s_gPlayerExtraIdValue[playerid] = value;
    s_gPlayerExtraIdItem[playerid] = _:itemid;
    s_gPlayerExtraIdItemType[playerid] = itemtype;
    return ~1;
}

//----------------------------------------------------------
// EOF