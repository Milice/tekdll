#include "player.h"

Player::Player(Client* c)
{
	client = client;
}

void Player::say(char* text, uint32_t talkType)
{
	switch (talkType) {
		case TALK_SAY:
		case TALK_WHISPER:
		case TALK_YELL:
			__asm {
				PUSH text
				PUSH talkType
				CALL addressPlayerSay
			}
		break;

		case TALK_TO_NPC:
			char* charName = "Milice";
			__asm {
				PUSH 0xFFFE
				PUSH 0x00
				PUSH talkType
				PUSH text
				PUSH 0xCE
				PUSH charName
				PUSH text
				PUSH talkType
				CALL addressPlayerSay
			}
		break;
	}
}

void Player::step(uint32_t direction)
{
	uint32_t walkA = 0x00;
	uint32_t walkB = 0x00;
	uint32_t walkC = 0x00;
	switch (direction) {
	case STEP_NORTH:
		walkB = 0xFFFFFFFF;
		break;

	case STEP_SOUTH:
		walkB = 0x01;
		break;

	case STEP_WEST:
		walkA = 0xFFFFFFFF;
		break;

	case STEP_EAST:
		walkA = 0x01;
		break;

	case STEP_NORTH_WEST:
		walkA = 0xFFFFFFFF;
		walkB = 0xFFFFFFFF;
		walkC = 0x01;
		break;

	case STEP_SOUTH_WEST:
		walkA = 0xFFFFFFFF;
		walkB = 0x01;
		walkC = 0x01;
		break;

	case STEP_NORTH_EAST:
		walkA = 0x01;
		walkB = 0xFFFFFFFF;
		walkC = 0x01;
		break;

	case STEP_SOUTH_EAST:
		walkA = 0x01;
		walkB = 0x01;
		walkC = 0x01;
		break;
	}

	__asm {
		PUSH walkC
		PUSH walkB
		PUSH walkA
		CALL addressPlayerWalk
	}
}

uint32_t Player::getNextStep(uint16_t toX, uint16_t toY, uint16_t toZ)
{
	uint16_t currentX = getPosX();
	uint16_t currentY = getPosY();
	uint16_t currentZ = getPosY();

	if (toX < currentX) {
		return STEP_WEST;
	}
	else if (toX > currentX) {
		return STEP_EAST;
	}
	else if (toY > currentY) {
		return STEP_SOUTH;
	}
	else if (toY < currentY) {
		return STEP_NORTH;
	}

	return STEP_NONE;
}

void Player::walkToPos()
{
	while((getPosX() != toPosX || getPosY() != toPosY || getPosZ() != toPosZ) && walkerActive) {
		uint32_t nextStep = getNextStep(toPosX, toPosY, toPosZ);
		if(nextStep != STEP_NONE) {
			step(nextStep);
		}
		Sleep(100);
	}

	walkerActive = false;
}

bool Player::executeCommand(std::string commandText)
{
	std::string commandName = commandText.substr(0, (commandText.find(" ") != std::string::npos ? commandText.find(" ") : commandText.length()));
	std::vector<std::string> commandParams;

	if(commandText.length() > commandName.length()) {
		commandText = commandText.substr(commandName.length() + 1);
	}

	while(size_t delimiter = commandText.find(" ")) {
		if(delimiter == std::string::npos) {
			if((!commandParams.empty() && !commandText.empty()) || commandName != commandText) {
				commandParams.push_back(commandText);
			}
			break;
		}
		commandParams.push_back(commandText.substr(0, delimiter));
		commandText = commandText.substr(delimiter + 1);
	}

	if(commandName == "step") {
		if(commandParams.size() > 0) {
			std::string param = commandParams.at(0);
			if(param == "north") {
				step(STEP_NORTH);
			} else if(param == "south") {
				step(STEP_SOUTH);
			} else if(param == "east") {
				step(STEP_EAST);
			} else if(param == "west") {
				step(STEP_WEST);
			} else if(param == "northwest") {
				step(STEP_NORTH_WEST);
			} else if(param == "northeast") {
				step(STEP_NORTH_EAST);
			} else if(param == "southwest") {
				step(STEP_SOUTH_WEST);
			} else if(param == "southeast") {
				step(STEP_SOUTH_EAST);
			}
		}
		return true;
	} else if(commandName == "walk") {
		if(commandParams.size() >= 3) {
			toPosX = stoi(commandParams.at(0));
			toPosY = stoi(commandParams.at(1));
			toPosZ = stoi(commandParams.at(2));
		}
		return true;
	} else if(commandName == "attack") {
		if(commandParams.size() > 0) {
			doAttack(commandParams.at(0));
			return true;
		} else {
			doAttack(0);
			return true;
		}
	}

	if (commandName == "walker") {
		walkerActive = !walkerActive;
		return true;
	}

	return false;
}

void Player::doAttack(uint32_t creatureId)
{
	client->writeData(addressPlayerAttacking, creatureId);

	__asm {
		PUSH creatureId
		CALL addressPlayerAttack
	}
}

void Player::doAttack(std::string creatureName)
{
	updateBattleList();
	for(BattleEntry entry : battleList) {
		if(entry.name == creatureName && entry.health > 0) {
			doAttack(entry.id);
		}
	}
}

void Player::updateBattleList()
{
	std::vector<BattleEntry> tempBattleList;

	for(uint8_t i = 0; i < 249; i++) {
		uintptr_t addressId = addressBattleList + (i * BATTLELIST_STEP);

		uint32_t id = client->readData<uint32_t>(addressId);
		std::string name = client->readDataString(addressId + BATTLELIST_NAME, 32);
		uint16_t health = client->readData<uint16_t>(addressId + BATTLELIST_HEALTH);
		uint16_t posX = client->readData<uint16_t>(addressId + BATTLELIST_POSX);
		uint16_t posY = client->readData<uint16_t>(addressId + BATTLELIST_POSY);
		uint16_t posZ = client->readData<uint16_t>(addressId + BATTLELIST_POSZ);
		
		if(id != 0 || health != 0) {
			BattleEntry newEntry = {id, name, health, posX, posY, posZ};
			tempBattleList.push_back(newEntry);
		}
	}

	battleList = tempBattleList;
}