#pragma once
#include "Windows.h"

class GameVars
{
public:
	uintptr_t procID;
	HANDLE hProcess;
	uintptr_t client;
	uintptr_t engine;
	uintptr_t ClientState;
	uintptr_t glowObjMan;

	uintptr_t ClocalPlayer;
};

extern GameVars game;