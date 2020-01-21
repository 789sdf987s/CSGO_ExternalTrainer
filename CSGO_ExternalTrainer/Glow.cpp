#include "stdafx.h"
#include "Glow.h"
#include "EntityBase.h"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

struct ClrRender
{
	BYTE red, green, blue;
};
ClrRender clrRenderEnemy;
ClrRender clrRenderTeam;

void setColor()
{
	clrRenderEnemy.red = 255;
	clrRenderEnemy.green = 0;
	clrRenderEnemy.blue = 0;
	clrRenderTeam.red = 0;
	clrRenderTeam.green = 0;
	clrRenderTeam.blue = 255;
}

void resetColor()
{
	clrRenderEnemy.red = -1;
	clrRenderEnemy.green = -1;
	clrRenderEnemy.blue = -1;
	clrRenderTeam.red = -1;
	clrRenderTeam.green = -1;
	clrRenderTeam.blue = -1;
}

void setBrightness()
{
	auto brightness = 5.0f;
	auto const ptr = readMem<int>(game.engine + model_ambient_min);
	auto const xorptr = *(int*)&brightness ^ ptr;
	writeMem<int>(game.engine + model_ambient_min, xorptr);
}

void resetBrightness()
{
	auto brightness = 0.0f;
	auto const ptr = readMem<int>(game.engine + model_ambient_min);
	auto const xorptr = *(int*)&brightness ^ ptr;
	writeMem<int>(game.engine + model_ambient_min, xorptr);
}

struct glowStruct
{
	BYTE base[4];
	float red;
	float green;
	float blue;
	float alpha;
	BYTE buffer[16];
	bool renderWhenOccluded;
	bool renderWhenUnOccluded;
	bool fullBloom;
	BYTE buffer1;
	BYTE buffer2[4];
	int glowStyle;
} Glow;

glowStruct setEnemyGlowColor(glowStruct Glow, uintptr_t const entity, int const health)
{
	auto const isDefusing = readMem<bool>(entity + m_bIsDefusing);
	if (isDefusing)
	{
		Glow.red = 1.0f;
		Glow.green = 1.0f;
		Glow.blue = 1.0f;
	}
	else
	{
		Glow.red = health * -0.01f + 1;
		Glow.green = health * 0.01f;
	}
	Glow.alpha = 1.0f;
	Glow.renderWhenOccluded = true;
	Glow.renderWhenUnOccluded = false;
	return Glow;
}

void setTeamGlow(int const index)
{
	auto TGlow = readMem<glowStruct>(game.glowObjMan + (index * 0x38));
	
	TGlow.blue = 1.0f;
	TGlow.alpha = 1.0f;
	TGlow.renderWhenOccluded = true;
	TGlow.renderWhenUnOccluded = false;
	
	writeMem<glowStruct>(game.glowObjMan + (index * 0x38), TGlow);
}

void setEnemyGlow(uintptr_t const entityIndex, int const index)
{
	auto EGlow = readMem<glowStruct>(game.glowObjMan + (index * 0x38));

	EGlow = setEnemyGlowColor(EGlow, entityIndex, entity.GetHealth(entityIndex));
	
	writeMem<glowStruct>(game.glowObjMan + (index * 0x38), EGlow);
}

void handleGlow(int const i)
{
	auto const entityIndex = readMem<uintptr_t>(game.client + dwEntityList + i * 0x10);
	if (entityIndex != NULL && !entity.IsDormant(entityIndex))
	{
		if (entity.GetHealth(entityIndex))
		{
			auto const entityGlowIndex = readMem<int>(entityIndex + m_iGlowIndex);
			if (entity.GetTeam(game.ClocalPlayer) == entity.GetTeam(entityIndex))
			{
				setTeamGlow(entityGlowIndex);
				writeMem<ClrRender>(entityIndex + m_clrRender, clrRenderTeam);
			}
			else
			{
				setEnemyGlow(entityIndex, entityGlowIndex);
				writeMem<ClrRender>(entityIndex + m_clrRender, clrRenderEnemy);
			}
		}
	}
}