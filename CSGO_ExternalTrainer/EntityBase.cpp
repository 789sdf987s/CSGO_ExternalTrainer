#include "stdafx.h"
#include "EntityBase.h"
#include "csgo.hpp"
#include "game.h"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

EntityBase entity;

bool EntityBase::IsInGame() const
{
	auto const ClientState_State = readMem<int>(game.ClientState + dwClientState_State);
	return (ClientState_State == 6 ? true : false);
}

int EntityBase::GetHealth(uintptr_t const entityIndex) const
{
	auto const health = readMem<int>(entityIndex + m_iHealth);
	return health;
}

int EntityBase::GetTeam(uintptr_t const entityIndex) const
{
	auto const team = readMem<int>(entityIndex + m_iTeamNum);
	return team;
}

Vector3 EntityBase::GetVelocity(uintptr_t const entityIndex)
{
	Vector3 velocity;
	ReadProcessMemory(game.hProcess, (BYTE*)entityIndex + m_vecVelocity, &velocity, sizeof(velocity), nullptr);
	return velocity;
}

Vector3 EntityBase::GetOrigin(uintptr_t const entityIndex)
{
	Vector3 origin;
	ReadProcessMemory(game.hProcess, (BYTE*)entityIndex + m_vecVelocity, &origin, sizeof(origin), nullptr);
	return origin;
}


bool EntityBase::IsAlive(uintptr_t const entityIndex) const
{
	const auto health = readMem<int>(entityIndex + m_iHealth);
	return (health == 0 ? false : true);
}

bool EntityBase::IsDormant(uintptr_t const entityIndex) const
{
	auto const dormant = readMem<bool>(entityIndex + m_bDormant);
	return dormant;
}

bool EntityBase::IsSpotted(uintptr_t const entityIndex) const
{
	auto spotted = readMem<bool>(entityIndex + m_bSpotted);
	return spotted;
}

bool EntityBase::IsEnemy(uintptr_t const entityIndex1, uintptr_t const entityIndex2) const
{
	const auto entityIndex1Team = GetTeam(entityIndex1);
	const auto entityIndex2Team = GetTeam(entityIndex2);
	return (entityIndex1Team == entityIndex2Team ? false : true);
}

bool EntityBase::isInvulnerable(uintptr_t const entityIndex) const
{
	auto const invulnerable = readMem<bool>(entityIndex + m_bGunGameImmunity);
	return invulnerable;
}


float EntityBase::GetFlashDuration(uintptr_t const entityIndex) const
{
	auto const flashDuration = readMem<float>(entityIndex + m_flFlashDuration);
	return flashDuration;
}
