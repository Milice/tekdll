#pragma once
#include "client.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player {
	public:
		Player(Client* c);
		void say(char* text, uint32_t talkType);
		void step(uint32_t direction);
		uint16_t getPosX() {
			uint16_t posX = 0;
			memcpy(&posX, (void*)addressPlayerPosX, 2);
			return posX;
		}

		uint16_t getPosY() {
			uint16_t posY = 0;
			memcpy(&posY, (void*)addressPlayerPosY, 2);
			return posY;
		}

		uint16_t getPosZ() {
			uint16_t posZ = 0;
			memcpy(&posZ, (void*)addressPlayerPosZ, 2);
			return posZ;
		}

		uint16_t getToPosX() {
			return toPosX;
		}

		uint16_t getToPosY() {
			return toPosY;
		}

		uint16_t getToPosZ() {
			return toPosZ;
		}

		uint32_t getNextStep(uint16_t toX, uint16_t toY, uint16_t toZ);

		void walkToPos();
		bool executeCommand(std::string commandText);

		bool walkerActive = false;

		void doAttack(uint32_t creatureId);
		void doAttack(std::string creatureName);
		void updateBattleList();

		std::vector<BattleEntry> getBattleList() {
			return battleList;
		}
	protected:
		Client* client;
		uint16_t toPosX;
		uint16_t toPosY;
		uint16_t toPosZ;
		std::vector<BattleEntry> battleList;
};

#endif