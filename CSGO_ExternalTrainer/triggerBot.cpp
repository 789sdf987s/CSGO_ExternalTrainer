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
	auto const crosshairID = readMem<int>(game.ClocalPlayer + m_iCrosshairId);
	if (crosshairID != 0 && crosshairID < 64)
	{
		auto const entityIndex = readMem<uintptr_t>(game.client + dwEntityList + ((crosshairID - 1) * 0x10));
		if (entity.GetTeam(entityIndex) != entity.GetTeam(game.ClocalPlayer))
		{
			if (entity.GetHealth(entityIndex))
			{
				if (!entity.isInvulnerable(entityIndex))
				{
					return true;
				}
			}
		}
	}

	return false;
}

void handleTriggerBot()
{
	getTriggerBotStatus();
	
	if (settings.triggerBotStatus)
	{
		if (checkCrosshair())
		{
			shoot();
		}
	}
}