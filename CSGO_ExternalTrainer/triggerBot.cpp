#include "stdafx.h"
#include "triggerBot.h"
#include <thread>
#include "Settings.h"
#include "EntityBase.h"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

constexpr auto KeyDown = -32768;
constexpr auto KeyUp = 0;

bool isAltHeld = false;

// Check if triggerbot is turned on/off
void getTriggerBotStatus()
{
	if (GetAsyncKeyState(VK_MENU) == KeyDown && !isAltHeld)
	{
		isAltHeld = true;
		settings.triggerBotStatus = true;
	}
	if (GetAsyncKeyState(VK_MENU) == KeyUp && isAltHeld)
	{
		isAltHeld = false;
		settings.triggerBotStatus = false;
	}
}

// Send mouse1 input (shoot)
void shoot()
{
	INPUT leftMouseShoot = { 0 };
	//press
	leftMouseShoot.type = INPUT_MOUSE;
	leftMouseShoot.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &leftMouseShoot, sizeof(INPUT));

	std::this_thread::sleep_for(std::chrono::milliseconds(30));

	// Release
	leftMouseShoot.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &leftMouseShoot, sizeof(INPUT));
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

// Check if enemy is in crosshair
bool checkCrosshair()
{
	// Checking what is in the crosshair. (0 = floor,wall... / 1 ~ 64 = player / > 64 = window, gun...)
	auto const crosshairID = readMem<int>(game.ClocalPlayer + m_iCrosshairId);
	if (crosshairID != 0 && crosshairID < 64)
	{
		// Getting the player that is in the crosshair and checking if he's and enemy
		auto const entityIndex = readMem<uintptr_t>(game.client + dwEntityList + ((crosshairID - 1) * 0x10));
		if (entity.GetTeam(entityIndex) != entity.GetTeam(game.ClocalPlayer))
		{
			// Checking If the player is alive
			if (entity.GetHealth(entityIndex))
			{
				// Checking if the player is invulnerable (deathmatch, gungame)
				if (!entity.isInvulnerable(entityIndex))
				{
					// Ready to shoot
					return true;
				}
			}
		}
	}

	// Don't shoot
	return false;
}

void handleTriggerBot()
{
	getTriggerBotStatus();
	
	if (settings.triggerBotStatus)
	{
		// Check if an enemy is on the crosshair, if true, send mouse1 input to shoot
		if (checkCrosshair())
		{
			shoot();
		}
	}
}