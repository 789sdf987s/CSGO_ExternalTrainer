#include "stdafx.h"
#include "BunnyHop.h"
#include "Settings.h"
#include "EntityBase.h"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

constexpr auto KeyDown = -32768;
constexpr auto KeyUp = 0;
constexpr auto forceJump = 6;

bool isSpaceHeld = false;

// Checking if bunnyhop is on/off
void getBunnyHopStatus()
{
	if (GetAsyncKeyState(VK_SPACE) == KeyDown && !isSpaceHeld)
	{
		isSpaceHeld = true;
		settings.bhopStatus = true;
	}
	if (GetAsyncKeyState(VK_SPACE) == KeyUp && isSpaceHeld)
	{
		isSpaceHeld = false;
		settings.bhopStatus = false;
	}
}

void handleBHop()
{
	getBunnyHopStatus();
	if (settings.bhopStatus)
	{
		// Get localPlayer velocity
		auto const playerVelocity = entity.GetVelocity(game.ClocalPlayer);
		// Check if the player is moving on the X, Y coords, but not moving on the z coords (on the ground)
		if (playerVelocity.x != 0 && playerVelocity.y != 0 && playerVelocity.z == 0 && settings.bhopStatus)
		{
			writeMem(game.client + dwForceJump, forceJump);
		}
	}
}
