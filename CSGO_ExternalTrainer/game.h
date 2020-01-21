#pragma once

struct WorldToScreenMatrix_t
{
	float flMatrix[4][4];
};

class GameVars
{
public:
	uintptr_t procID;
	HANDLE hProcess;
	uintptr_t client;
	uintptr_t engine;
	uintptr_t ClientState;
	uintptr_t glowObjMan;

	RECT m_Rect;
	WorldToScreenMatrix_t world_to_screen_matrix;
	
	uintptr_t ClocalPlayer;
};

extern GameVars game;