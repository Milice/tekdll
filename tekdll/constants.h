#pragma once
#include <windows.h>
#include <sstream>
#include <stdint.h>
#include <psapi.h>
#include <thread>
#include <vector>

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define TALK_SAY 0x01
#define TALK_WHISPER 0x02
#define TALK_YELL 0x03
#define TALK_TO_NPC 0x04
/*#define TALK_GAMEMASTER_REQUEST 0x09
#define TALK_PLAYER_ANSWER 0x0B
#define TALK_GAMEMASTER_BROADCAST 0x0C
#define TALK_ANONYMOUS_BROADCAST 0x10
*/

#define STEP_NONE 0
#define STEP_NORTH 1
#define STEP_SOUTH 2
#define STEP_WEST 3
#define STEP_EAST 4
#define STEP_NORTH_WEST 5
#define STEP_NORTH_EAST 6
#define STEP_SOUTH_WEST 7
#define STEP_SOUTH_EAST 8

#define BATTLELIST_STEP 0xA8
#define BATTLELIST_NAME 0x04
#define BATTLELIST_POSX 0x24
#define BATTLELIST_POSY 0x28
#define BATTLELIST_POSZ 0x2C
#define BATTLELIST_HEALTH 0x88

struct BattleEntry {
	uint32_t id;
	std::string name;
	uint16_t health;
	uint16_t posX;
	uint16_t posY;
	uint16_t posZ;
};

#endif