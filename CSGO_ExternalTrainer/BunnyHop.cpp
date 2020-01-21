#include "stdafx.h"
#include "BunnyHop.h"
#include "Settings.h"
#include "EntityBase.h"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

constexpr auto KeyDown = -32768;
constexpr auto KeyUp = 0;

bool isSpaceHeld = false;
int forceJump = 6;

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
		if (playerVelocity.x != 0 && playerVelocity.y != 0 && playerVelocity.z == 0 && settings.bhopStatus)
		{
			writeMem(game.client + dwForceJump, forceJump);
		}
	}
}
