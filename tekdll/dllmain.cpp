#include "addresses.h"
#include "constants.h"
#include "client.h"
#include "player.h"

Client* client = nullptr;
Player* player = nullptr;

DWORD WINAPI mainThread(LPVOID params)
{
	return true;
}

DWORD WINAPI walkerThread(LPVOID params)
{
	while(true) {
		if(player->walkerActive) {
			player->walkToPos();
		} else {
			Sleep(100);
		}
	}
	return true;
}

DWORD WINAPI attackerThread(LPVOID params)
{
	while(true) {
		std::vector<BattleEntry> bList = player->getBattleList();
		if(sizeof(bList) < 1) {
			player->updateBattleList();
		} else {
			//if(std::find(bList.begin(), bList.end(), )
		}
	}
	return true;
}

void sayForward(uint32_t talkType, char* text, char* charName, uint32_t unknownOne, char* textTwo, uint32_t talkTypeTwo, uint32_t unknownTwo, uint32_t unknownThree)
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
		__asm {
			PUSH unknownThree
			PUSH unknownTwo
			PUSH talkTypeTwo
			PUSH textTwo
			PUSH unknownOne
			PUSH charName
			PUSH text
			PUSH talkType
			CALL addressPlayerSay
		}
		break;
	}
}

void testCall(uint32_t talkType, char* text, char* charName, uint32_t unknownOne, char* textTwo, uint32_t talkTypeTwo, uint32_t unknownTwo, uint32_t unknownThree)
{
	std::string textString = std::string(text);
	if(textString.substr(0, 1) == "/") {
		if(player->executeCommand(textString.substr(1, textString.length() - 1))) {
			return;
		}
	}
	sayForward(talkType, text, charName, unknownOne, textTwo, talkTypeTwo, unknownTwo, unknownThree);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		//Initialize stuff
		client = new Client();
		player = new Player(client);
		//CreateThread(NULL, 0, mainThread, 0, 0, NULL);
		CreateThread(NULL, 0, walkerThread, 0, 0, NULL);
		//CreateThread(NULL, 0, attackerThread, 0, 0, NULL);
		client->hookCall(0x428EE1, (void*)&testCall);
		//////////////////////////////////////////////
	}

	return true;
}

