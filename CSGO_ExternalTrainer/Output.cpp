#include "stdafx.h"
#include "Output.h"
#include "Settings.h"

class Colors
{
public:
	static void changeToBlue()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	}

	static void changeToWhite()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}

	static void changeToRed()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	}

	static void changeToGreen()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	}
} colors;



void handleOutput()
{
	system("cls");
	// Title
	colors.changeToBlue();
	std::cout << "CSGO External Trainer V3 \n \n";
	
	// Bunnyhop
	colors.changeToWhite();
	std::cout << "[SPACE] BunnyHop \n \n";

	// Triggerbot
	colors.changeToWhite();
	std::cout << "[ALT] Triggerbot \n \n";

	// No Flash
	colors.changeToWhite();
	std::cout << "[F5] No Flash                  ";
	if (settings.noFlashStatus)
	{
		colors.changeToGreen();
		std::cout << "ON \n \n";
	}
	else
	{
		colors.changeToRed();
		std::cout << "OFF \n \n";
	}

	// Glow
	colors.changeToWhite();
	std::cout << "[F6] Glow Wallhack             ";
	if (settings.glowStatus)
	{
		colors.changeToGreen();
		std::cout << "ON \n \n";
	}
	else
	{
		colors.changeToRed();
		std::cout << "OFF \n \n";
	}

	// ESP
	colors.changeToWhite();
	std::cout << "[F7] ESP                       ";
	if (settings.espStatus)
	{
		colors.changeToGreen();
		std::cout << "ON \n \n";
	}
	else
	{
		colors.changeToRed();
		std::cout << "OFF \n \n";
	}

	// Exit
	colors.changeToWhite();
	std::cout << "[INSERT] Exit \n \n";
}
