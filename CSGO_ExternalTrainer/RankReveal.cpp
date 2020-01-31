#include "stdafx.h"
#include "RankReveal.h"
#include "EntityBase.h"
#include "game.h"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

const char* CompRanks[19] =
{
		"Unranked",
		"Silver I",
		"Silver II",
		"Silver III",
		"Silver IV",
		"Silver V",
		"Silver VI",
		"Gold Nova I",
		"Gold Nova II",
		"Gold Nova III",
		"Gold Nova IV",
		"Master Guardian I",
		"Master Guardian II",
		"Master Guardian Elite",
		"Distinguished Master Guardian",
		"Legendary Eagle",
		"Legendary Eagle Master",
		"Supreme",
		"Global"
};

void handleRankReveal()
{
	auto const playerResource = readMem<uintptr_t>(game.client + dwPlayerResource);
	for (auto i = 0; i < 32; i++)
	{
		auto const entityRank = readMem<int>(playerResource + m_iCompetitiveRanking + i * 4);
		auto const entityScore = readMem<int>(playerResource + m_iScore + i * 4);
		auto const entityPing = readMem<int>(playerResource + m_iPing + i * 4);
		
		auto const entityIndex = readMem<int>(game.client + dwEntityList + i * 0x10);
		auto const entityTeam = entity.GetTeam(entityIndex);

		// Checking if player is valid
		if (entityTeam > 1 && entityPing > 5)
		{
			// Enemies
			if (entityTeam != entity.GetTeam(game.ClocalPlayer))
			{
				std::cout << "[TEAM] [SCORE: " << entityScore << "]" << " [RANK: " << CompRanks[entityRank] << "]" << std::endl;
				
			}
			// Allies
			else
			{
				std::cout << "[ENEMY] [SCORE: " << entityScore << "]" << " [RANK: " << CompRanks[entityRank] << "]" << std::endl;
			}
		}
	}
}
