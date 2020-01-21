#include "stdafx.h"
#include "ESP.h"
#include <sstream>
#include "game.h"
#include "EntityBase.h"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

HWND hwndCSGO;
HDC hdcCSGO;
HBRUSH enemyBrush;
HFONT font;

HWND handle;

COLORREF snapLineColor;
COLORREF textColor;

void drawFilledRect(float const x, float const y, float const w, int const h)
{
	RECT rect = { x, y, x + w, y + h };
	FillRect(hdcCSGO, &rect, enemyBrush);
}

// Draw rectangles on enemies
void drawBorderBox(float const x, float const y, float const w, float const h, int const thickness)
{
	//Top horizontal line
	drawFilledRect(x, y, w, thickness);
	//Left vertical line
	drawFilledRect(x, y, thickness, h);
	//right vertical line
	drawFilledRect((x + w), y, thickness, h);
	//bottom horizontal line
	drawFilledRect(x, y + h, w + thickness, thickness);
}

void drawLine(int const startX, int const startY, int const endX, int const endY, COLORREF const pen)
{
	// pen style, width, color
	auto const hNPen = CreatePen(PS_SOLID, 2, pen);
	auto const hOPen = HPEN(SelectObject(hdcCSGO, hNPen));
	// starting point of line
	MoveToEx(hdcCSGO, startX, startY, nullptr);
	// ending point of line
	LineTo(hdcCSGO, endX, endY);
	DeleteObject(SelectObject(hdcCSGO, hOPen));
}

void drawString(int const x, int const y, COLORREF const color, const char* text)
{
	SetTextAlign(hdcCSGO, TA_CENTER | TA_NOUPDATECP);

	SetBkColor(hdcCSGO, RGB(0, 0, 0));
	SetBkMode(hdcCSGO, TRANSPARENT);

	SetTextColor(hdcCSGO, color);

	SelectObject(hdcCSGO, font);

	TextOutA(hdcCSGO, x, y, text, strlen(text));

	DeleteObject(font);
}

void drawESP(float const x, float const y, float const distance, const char* gun, RECT const rect)
{
	//ESP RECTANGLE
	auto const width = 18100 / distance;
	auto const height = 37500 / distance;
	drawBorderBox(x - (width / 2), y - height, width, height, 1);

	drawLine((rect.right - rect.left) / 2, rect.bottom - rect.top, x, y, snapLineColor);

	std::stringstream ss;
	ss << int(distance);

	std::stringstream ss2;
	ss2 << (const char*)gun;

	auto const distanceInfo = new char[ss.str().size() + 1];
	strcpy(distanceInfo, ss.str().c_str());

	auto const gunInfo = new char[ss2.str().size() + 1];
	strcpy(gunInfo, ss2.str().c_str());

	drawString(x, y, textColor, distanceInfo);
	drawString(x, y + 30, textColor, gunInfo);

	delete[] distanceInfo;
	delete[] gunInfo;
}

const char* getWeaponName(short int const weaponID)
{
	const char* weapon;
	switch (weaponID)
	{
	case 1:
		weapon = "DEagle";
		break;
	case 2:
		weapon = "Dualies";
		break;
	case 3:
		weapon = "Five-Seven";
		break;
	case 4:
		weapon = "Glock";
		break;
	case 7:
		weapon = "AK-47";
		break;
	case 8:
		weapon = "AUG";
		break;
	case 9:
		weapon = "AWP";
		break;
	case 10:
		weapon = "Famas";
		break;
	case 11:
		weapon = "AUTO";
		break;
	case 13:
		weapon = "Galil";
		break;
	case 14:
		weapon = "M249";
		break;
	case 16:
		weapon = "M4";
		break;
	case 17:
		weapon = "MAC-10";
		break;
	case 19:
		weapon = "P90";
		break;
	case 23:
		weapon = "MP5";
		break;
	case 24:
		weapon = "UMP";
		break;
	case 25:
		weapon = "XM1014";
		break;
	case 26:
		weapon = "PP-Bizon";
		break;
	case 27:
		weapon = "MAG-7";
		break;
	case 28:
		weapon = "Negev";
		break;
	case 29:
		weapon = "Sawed-Off";
		break;
	case 30:
		weapon = "Tec-9";
		break;
	case 31:
		weapon = "Zeus";
		break;
	case 32:
		weapon = "P2000";
		break;
	case 33:
		weapon = "MP7";
		break;
	case 34:
		weapon = "MP9";
		break;
	case 35:
		weapon = "Nova";
		break;
	case 36:
		weapon = "P250";
		break;
	case 38:
		weapon = "AUTO";
		break;
	case 39:
		weapon = "Krieg";
		break;
	case 40:
		weapon = "Scout";
		break;
	case 43:
		weapon = "Flash";
		break;
	case 44:
		weapon = "HE";
		break;
	case 45:
		weapon = "Smoke";
		break;
	case 46:
		weapon = "Molotov";
		break;
	case 47:
		weapon = "Decoy";
		break;
	case 48:
		weapon = "Molotov";
		break;
	case 49:
		weapon = "C4";
		break;
	case 60:
		weapon = "M4";
		break;
	case 61:
		weapon = "USP";
		break;
	case 63:
		weapon = "CZ75";
		break;
	case 64:
		weapon = "Revolver";
		break;
	default:
		weapon = "Knife";
		break;
	}
	return weapon;
}

// Checks if the player is on the screen (e.g. not behind you) and converts his 3D coords to 2D
bool WorldToScreen(float* from, float* to, RECT rect, WorldToScreenMatrix_t WtSMatrix)
{
	auto w = 0.0f;

	to[0] = WtSMatrix.flMatrix[0][0] * from[0] + WtSMatrix.flMatrix[0][1] * from[1] + WtSMatrix.flMatrix[0][2] * from[2] + WtSMatrix.flMatrix[0][3];
	to[1] = WtSMatrix.flMatrix[1][0] * from[0] + WtSMatrix.flMatrix[1][1] * from[1] + WtSMatrix.flMatrix[1][2] * from[2] + WtSMatrix.flMatrix[1][3];
	w = WtSMatrix.flMatrix[3][0] * from[0] + WtSMatrix.flMatrix[3][1] * from[1] + WtSMatrix.flMatrix[3][2] * from[2] + WtSMatrix.flMatrix[3][3];

	if (w < 0.01f)
		return false;

	auto const invw = 1.0f / w;
	to[0] *= invw;
	to[1] *= invw;

	auto const width = (int)(rect.right - rect.left);
	auto const height = (int)(rect.bottom - rect.top);

	float x = width / 2;
	float y = height / 2;

	x += 0.5 * to[0] * width + 0.5;
	y -= 0.5 * to[1] * height + 0.5;

	to[0] = x + rect.left;
	to[1] = y + rect.top;

	return true;
}

float Get3dDistance(float* myCoords, float* enemyCoords)
{
	return sqrt(
		pow((enemyCoords[0] - myCoords[0]), 2) +
		pow((enemyCoords[1] - myCoords[1]), 2) +
		pow((enemyCoords[2] - myCoords[2]), 2));

}

void setup()
{
	hwndCSGO = FindWindowA(nullptr, "Counter-Strike: Global Offensive");
	hdcCSGO = GetDC(hwndCSGO);

	enemyBrush = CreateSolidBrush(RGB(255, 0, 0));
	snapLineColor = RGB(0, 0, 255);
	textColor = RGB(0, 255, 0);
}

void handleESP(int const i, WorldToScreenMatrix_t WtSMatrix)
{
	auto const entityIndex = readMem<int>(game.client + dwEntityList + i * 0x10);

	if (!entity.IsAlive(entityIndex))
		return;

	if (entity.GetTeam(game.ClocalPlayer) == entity.GetTeam(entityIndex))
		return;

	// Getting the current weapon from the entity
	auto const weaponIndex = readMem<int>(entityIndex + m_hActiveWeapon) & 0xFFF;
	auto const weaponBase = readMem<int>(game.client + dwEntityList + (weaponIndex - 1) * 0x10);
	auto const weaponID = readMem<short>(weaponBase + m_iItemDefinitionIndex);
	auto const weapon = getWeaponName(weaponID);

	float enemyXY[3];

	float entityPosition[3];
	ReadProcessMemory(game.hProcess, (BYTE*)(entityIndex + m_vecOrigin), &entityPosition, sizeof(float[3]), nullptr);

	float myPosition[3];
	ReadProcessMemory(game.hProcess, (BYTE*)(game.ClocalPlayer + m_vecOrigin), &myPosition, sizeof(float[3]), nullptr);
	
	if (WorldToScreen(entityPosition, enemyXY, game.m_Rect, WtSMatrix))
	{
		drawESP(enemyXY[0] - game.m_Rect.left, enemyXY[1] - game.m_Rect.top, Get3dDistance(myPosition, entityPosition), weapon, game.m_Rect);
	}
}
