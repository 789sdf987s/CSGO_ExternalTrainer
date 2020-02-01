#include "stdafx.h"
#include "Output.h"
#include "Settings.h"

class Colors
{
public:
	void changeToBlue()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	}

	void changeToWhite()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}

	void changeToRed()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	}

	void changeToGreen()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	}
} colors;



void handleOutput()
{
	// Title
	colors.changeToBlue();
	std::cout << "CSGO External Trainer V3 \n";
	
	// Bunnyhop
	colors.changeToWhite();
	std::cout << "[SPACE] BunnyHop \n";

	// Triggerbot
	colors.changeToWhite();
	std::cout << "[ALT] Triggerbot \n";

	// No Flash
	colors.changeToWhite();
	std::cout << "[F5] No Flash                  ";
	if (settings.noFlashStatus)
	{
		colors.changeToGreen();
		std::cout << "ON \n";
	}
	else
	{
		colors.changeToRed();
		std::cout << "OFF \n";
	}

	// Glow
	colors.changeToWhite();
	std::cout << "[F6] Glow Wallhack             ";
	if (settings.glowStatus)
	{
		colors.changeToGreen();
		std::cout << "ON \n";
	}
	else
	{
		colors.changeToRed();
		std::cout << "OFF \n";
	}

	// ESP
	colors.changeToWhite();
	std::cout << "[F7] ESP                       ";
	if (settings.espStatus)
	{
		colors.changeToGreen();
		std::cout << "ON \n";
	}
	else
	{
		colors.changeToRed();
		std::cout << "OFF \n";
	}

}
