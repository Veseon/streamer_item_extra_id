//----------------------------------------------------------
//
//  Streamer Item Extra Id Library
//  Dependencies: omp-stdlib, samp-streamer-plugin, YSI-Includes (y_hooks)
//
//  MIT License
//
//  Copyright (c) 2024 Veseon
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//
//----------------------------------------------------------

#if defined streamer_item_extra_id_included
    #endinput
#endif
#define streamer_item_extra_id_included

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

/*
    Example enum for extraid tag & definitions.
    Add before including the dependency.

    enum EXTRA_ID_TYPE
    {
        EXTRA_ID_TYPE_ENTRANCE,
        EXTRA_ID_TYPE_HOUSE,
        EXTRA_ID_TYPE_BUSINESS
    }

    If you want auto completion you might need to define them aswell
    after creating the enum.

    #define EXTRA_ID_TYPE_ENTRANCE  (EXTRA_ID_TYPE:0)
    #define EXTRA_ID_TYPE_HOUSE     (EXTRA_ID_TYPE:1)
    #define EXTRA_ID_TYPE_BUSINESS  (EXTRA_ID_TYPE:2)

    If you do not wish to use tags you can simply not include
    the enum and just make definitions.

    #define EXTRA_ID_TYPE_ENTRANCE  (0)
    #define EXTRA_ID_TYPE_HOUSE     (1)
    #define EXTRA_ID_TYPE_BUSINESS  (2)
*/

#if !defined EXTRA_ID_TYPE
    #define EXTRA_ID_TYPE: _:
#endif

static  s_gItemCustomData,
        s_gPlayerLastItemId[MAX_PLAYERS] = {INVALID_STREAMER_ID, ...},
        STREAMER_TYPE:s_gPlayerLastItemType[MAX_PLAYERS] = {STREAMER_TYPE:STREAMER_TYPE_ANY, ...};

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

static stock bool:IsPlayerInDynamicItem(playerid, STREAMER_TYPE:itemtype, STREAMER_ALL_TAGS:itemid, Float:pickup_range = 1.3)
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

stock bool:Streamer_SetItemExtraId(STREAMER_TYPE:itemtype, STREAMER_ALL_TAGS:itemid, EXTRA_ID_TYPE:type, value)
{
    if(!s_gItemCustomData)
    {
        // Generate unique (more like random?) identifier for streamer extra id.
        s_gItemCustomData = E_STREAMER_CUSTOM(random(2^31 - 1));
    }

    if(!IsValidDynamicItem(itemtype, itemid))
        return false;

    new data[2];
    data[0] = _:type;
    data[1] = value;
    return bool:Streamer_SetArrayData(itemtype, itemid, s_gItemCustomData, data, sizeof data);
}

//----------------------------------------------------------

static stock bool:GetStreamerItemExtraId(STREAMER_TYPE:itemtype, STREAMER_ALL_TAGS:itemid, &EXTRA_ID_TYPE:type, &value)
{
    if(!Streamer_HasArrayData(itemtype, itemid, s_gItemCustomData))
        return false;

    new data[2];
    if(!Streamer_GetArrayData(itemtype, itemid, s_gItemCustomData, data, sizeof data))
        return false;

    type = EXTRA_ID_TYPE:data[0];
    value = data[1];
    return true;
}

//----------------------------------------------------------

stock bool:Streamer_GetItemExtraId(STREAMER_TYPE:itemtype, STREAMER_ALL_TAGS:itemid, &EXTRA_ID_TYPE:type, &value)
{
    if(!IsValidDynamicItem(itemtype, itemid))
        return false;

    return GetStreamerItemExtraId(itemtype, itemid, type, value);
}

//----------------------------------------------------------

stock bool:Streamer_GetPlayerExtraId(playerid, &EXTRA_ID_TYPE:type, &value)
{
    if(!(0 <= playerid < MAX_PLAYERS))
        return false;

    if(!IsValidDynamicItem(s_gPlayerLastItemType[playerid], s_gPlayerLastItemId[playerid]))
        return false;

    if(!IsPlayerInDynamicItem(playerid, s_gPlayerLastItemType[playerid], s_gPlayerLastItemId[playerid]))
        return false;

    if(!Streamer_HasArrayData(s_gPlayerLastItemType[playerid], s_gPlayerLastItemId[playerid], s_gItemCustomData))
        return false;

    new data[2];
    if(!Streamer_GetArrayData(s_gPlayerLastItemType[playerid], s_gPlayerLastItemId[playerid], s_gItemCustomData, data, sizeof data))
        return false;

    type = EXTRA_ID_TYPE:data[0];
    value = data[1];
    return true;
}

//----------------------------------------------------------

hook OnPlayerPickUpDynPickup(playerid, STREAMER_TAG_PICKUP:pickupid)
{
    if(s_gPlayerLastItemType[playerid] == STREAMER_TYPE_PICKUP && s_gPlayerLastItemId[playerid] == _:pickupid)
        return Y_HOOKS_CONTINUE_RETURN_1;

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

static stock OnPlayerEnterDynamicItem(playerid, STREAMER_TYPE:itemtype, STREAMER_ALL_TAGS:itemid)
{
    new EXTRA_ID_TYPE:type, value;
    if(!GetStreamerItemExtraId(itemtype, itemid, type, value))
        return Y_HOOKS_CONTINUE_RETURN_0;

    s_gPlayerLastItemId[playerid] = _:itemid;
    s_gPlayerLastItemType[playerid] = itemtype;
    return Y_HOOKS_CONTINUE_RETURN_1;
}

//----------------------------------------------------------
// EOF