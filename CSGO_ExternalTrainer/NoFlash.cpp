#include "stdafx.h"
#include "NoFlash.h"
#include "game.h"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

constexpr auto newFlashDuration = 0;

void handleNoFlash()
{
	// constantly writing 0 to the localPlayer flash duration
	writeMem<int>(game.ClocalPlayer + m_flFlashDuration, newFlashDuration);
}
