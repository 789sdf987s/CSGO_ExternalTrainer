#pragma once

struct Vector3
{
	float x, y, z;
};

class EntityBase
{
public:
	bool IsInGame() const;
	int GetHealth(uintptr_t const entityIndex) const;
	int GetTeam(uintptr_t const entityIndex) const;
	Vector3 GetVelocity(uintptr_t const entityIndex);
	Vector3 GetOrigin(uintptr_t const entityIndex);
	bool IsAlive(uintptr_t const entityIndex) const;
	bool IsDormant(uintptr_t const entityIndex) const;
	bool IsSpotted(uintptr_t const entityIndex) const;
	bool IsEnemy(uintptr_t const entityIndex1, uintptr_t const entityIndex2) const;
	bool isInvulnerable(uintptr_t const entityIndex) const;
	bool isScoped(uintptr_t const entityIndex) const;
	float GetFlashDuration(uintptr_t const entityIndex) const;
};

extern EntityBase entity;