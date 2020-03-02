#include "GM.h"
#include "Menace.h"
#include "Lucy.h"
#include "Eclair.h"
#include "Fromage.h"
#include "Bree.h"
#include "Aura.h"
#include "Tiramisu.h"
#include "Eclat.h"
#include <iostream>
using namespace std;

Menace* GetMenace();
class Quit : public exception
{
	const char* what() const override
	{
		return "The user chose to quit.";
	}
};
constexpr unsigned long maxGames = 4294967295ul;

// _Play _Train1 _Train2
#define _Play

int main()
{
#pragma region Play Menace
#ifdef _Play
	Menace* menacePtr = nullptr;
	bool homeScreen = true;

	while (true)
	{
		if (homeScreen)
		{
			system("cls");
			cout << "Welcome to Noughts-and-Crosses aka Tic-Tac-Toe." << endl;
			cout << endl;
		}
		homeScreen = false;

		try
		{
			menacePtr = GetMenace();
			GM uwu;
			uwu(*menacePtr);
			delete menacePtr;
			menacePtr = nullptr;
			homeScreen = true;
		}
		catch (Quit)
		{
			return EXIT_SUCCESS;
		}
		catch (string input)
		{
			system("cls");
			cout << input << " was not a name of a MENACE." << endl;
			cout << endl;
		}
		catch (Menace::NoFile)
		{
			system("cls");
			cout << "The file for the MENACE could not be found." << endl;
			cout << endl;
		}
		catch (Menace::BadFile)
		{
			system("cls");
			cout << "The file for the MENACE has been found, but the file has been corrupted." << endl;
			cout << endl;
		}
	}
#endif // !_Play
#pragma endregion // _Play

#pragma region Solo Test Menace
#ifdef  _Train1
	auto menacePtr = new Aura;
	GM uwu;
	uwu.SoloTrain(*menacePtr, 1e6);
	delete menacePtr;
	system("pause");
#endif // !_Train1
#pragma endregion // _Train1

#pragma region Duo Test Menace
#ifdef _Train2
	auto menacePtr1 = new Aura;
	auto menacePtr2 = new Bree;
	GM uwu;
	uwu.PartnerTrain(*menacePtr1, *menacePtr2, 100);
	delete menacePtr1;
	delete menacePtr2;
	system("pause");
#endif // !_Train2
#pragma endregion // _Train2

	return EXIT_SUCCESS;
}

Menace* GetMenace()
{
	string input;
	string lcInput;

	cout << "Enter the name of the machine-educable noughts-and-crosses engine (MENACE) you would like to play against." << endl;
	cout << "Alternatively, enter nothing to quit." << endl;
	cout << endl;
	getline(cin, input);
	if (input == "")
	{
		throw Quit();
	}

	lcInput = input;
	for (auto it = lcInput.begin(); it != lcInput.end(); it++)
	{
		if (isalpha(*it))
		{
			*it = tolower(*it);
		}
	}

	if (lcInput == "lucy")
	{
		return new Lucy;
	}
	else if (lcInput == "eclair")
	{
		return new Eclair;
	}
	else if (lcInput == "fromage")
	{
		return new Fromage;
	}
	else if (lcInput == "bree")
	{
		return new Bree;
	}
	else if (lcInput == "aura")
	{
		return new Aura;
	}
	//else if (lcInput == "tiramisu")
	//{
	//	return new Tiramisu;
	//}
	//else if (lcInput == "eclat")
	//{
	//	return new Eclat;
	//}
	else
	{
		throw input;
	}
}