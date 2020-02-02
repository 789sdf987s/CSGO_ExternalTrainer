#include "stdafx.h"
#include <iostream>
#include "ProcMan.h"
#include "game.h"
#include "Settings.h"
#include "EntityBase.h"
#include "BunnyHop.h"
#include "NoFlash.h"
#include "triggerBot.h"
#include "Glow.h"
#include "ESP.h"
#include "RankReveal.h"
#include "Output.h"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

int main()
{
	// Getting process ID
    game.procID = getProcID(L"csgo.exe");
	
	// Getting handle to the process
    game.hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, game.procID);
	
	// Getting module addresses
    game.client = getModuleBaseAddress(game.procID, L"client_panorama.dll");
	game.engine = getModuleBaseAddress(game.procID, L"engine.dll");

	// Getting pointer for ClientState. It's used to read ClientState_State (in-game check)
	game.ClientState = readMem<uintptr_t>(game.engine + dwClientState);

	// Setup to ESP (draw on top of the game window)
	setup();
	// Getting the game window to later draw on top using the ESP
	GetWindowRect(FindWindow(nullptr, L"Counter-Strike: Global Offensive"), &game.m_Rect);

	// Show menu on the screen
	handleOutput();
	
	// Infinite main loop unless insert (Panic key) is pressed
	while (!GetAsyncKeyState(VK_INSERT))
	{
		// Reading Local Player
		game.ClocalPlayer = readMem<uintptr_t>(game.client + dwLocalPlayer);

		// Getting glowObjMan to use glow later
		game.glowObjMan = readMem<uintptr_t>(game.client + dwGlowObjectManager);

		// Check if noFlash is turned on/off
		if (GetAsyncKeyState(VK_F5) & 1)
		{
			settings.noFlashStatus = !settings.noFlashStatus;
			handleOutput();
		}

		// Checking if glow is turned on/off
		if (GetAsyncKeyState(VK_F6) & 1)
		{
			settings.glowStatus = !settings.glowStatus;
			handleOutput();
			if (settings.glowStatus)
			{
				setColor();
				setBrightness();
			}
			else
			{
				resetColor();
				resetBrightness();
			}
		}

		// Check if ESP is turned on/off
		if (GetAsyncKeyState(VK_F7) & 1)
		{
			settings.espStatus = !settings.espStatus;
			handleOutput();
		}

		// Check if rank reveal is pressed
		if (GetAsyncKeyState(VK_END) & 1)
		{
			handleRankReveal();
			handleOutput();
		}


		
		// Check if player is in game
		if (game.ClocalPlayer != NULL && entity.IsInGame())
		{
			if (settings.glowStatus || settings.espStatus)
			{
				for (auto i = 0; i < 32; i++)
				{
					if (settings.glowStatus)
					{
						handleGlow(i);
					}
					if (settings.espStatus)
					{
						ReadProcessMemory(game.hProcess, PBYTE(game.client + dwViewMatrix), &game.world_to_screen_matrix, sizeof(game.world_to_screen_matrix), 0);
						handleESP(i, game.world_to_screen_matrix);
					}
				}
			}


			// Check if player is alive
			if (entity.IsAlive(game.ClocalPlayer))
			{
				handleBHop();
				handleTriggerBot();
				
				if (settings.noFlashStatus)
				{
					handleNoFlash();
				}
			}
		}
	}
}
