#include "stdafx.h"
#include "NoFlash.h"
#include "game.h"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

auto newFlashDuration = 0;

void handleNoFlash()
{
	writeMem<int>(game.ClocalPlayer + m_flFlashDuration, newFlashDuration);
}
