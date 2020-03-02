#define _CRT_SECURE_NO_WARNINGS

#include "GM.h"
#include "Eclair.h"
#include "Fromage.h"
#include <chrono>
#include <ctime>
#include <exception>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <utility>
using namespace std;

//#define _SHOW_PROGRESS
//#define _SHOW_MOVES

GM::GM(const unsigned long ci) : counterInverval(ci)
{
}
GM::~GM()
{
}
void GM::operator()(Menace& m)
{
	string name = m.name;
	name[0] = toupper(name[0]);

	bool changeSettings = true;
	string input;
	bool learning = false;
	unsigned short playerTurnOrder;
	unsigned short playerFirst;
	Board::Position playerMove;
	unsigned short winner;

	Board b;
	unsigned short playerWins = 0;
	unsigned short menaceWins = 0;
	unsigned short ties = 0;

	map<const Board, const Board::Position> Xmoves;
	map<const Board, const Board::Position> Omoves;

	// loop for each round
	while (true)
	{
		// settings
		if (changeSettings)
		{
			if (m.lastLearnCycle)
			{
				learning = GetLearning(m);
			}
			playerTurnOrder = GetTurn();
		}

		// establish turn order
		if (playerTurnOrder == 2)
		{
			minstd_rand rng;
			rng.seed((unsigned int)chrono::system_clock::now().time_since_epoch().count());
			playerFirst = rng() % 2;
		}
		else
		{
			playerFirst = playerTurnOrder;
		}

		// play round
		while (b.Winner() == ' ' && b.PositionsLeft() > 0)
		{
			system("cls");
			PrintTitleBar(m);
			cout << endl;
			if (m.lastLearnCycle)
			{
				if (learning)
				{
					cout << name << " learning." << endl;
				}
				else
				{
					cout << name << " static." << endl;
				}
				cout << endl;
			}
			cout << "Player Wins: " << playerWins << endl;
			cout << name << " Wins: " << menaceWins << endl;
			cout << "Ties: " << ties << endl;
			cout << endl;
			b.Print();
			cout << endl;

			if (b.PositionsLeft() % 2 == 1)
			{
				if (playerFirst == 1)
				{
					// get playerMove
					while (true)
					{
						try
						{
							playerMove = TakePlayerTurn(b, name);
							break;
						}
						catch (Board::BadPos)
						{
							system("cls");
							PrintTitleBar(m);
							cout << endl;
							if (learning)
							{
								cout << name << " learning." << endl;
							}
							else
							{
								cout << name << " static." << endl;
							}
							cout << endl;
							cout << "Player Wins: " << playerWins << endl;
							cout << name << " Wins: " << menaceWins << endl;
							cout << "Ties: " << ties << endl;
							cout << endl;
							b.Print();
							cout << endl;
							cout << "Those were not valid coordinates." << endl;
							cout << endl;
						}
						catch (Board::PosTaken)
						{
							system("cls");
							PrintTitleBar(m);
							cout << endl;
							if (learning)
							{
								cout << name << " learning." << endl;
							}
							else
							{
								cout << name << " static." << endl;
							}
							cout << endl;
							cout << "Player Wins: " << playerWins << endl;
							cout << name << " Wins: " << menaceWins << endl;
							cout << "Ties: " << ties << endl;
							cout << endl;
							b.Print();
							cout << endl;
							cout << "That position is already taken." << endl;
							cout << endl;
						}
					}
					Xmoves.insert(make_pair(b, playerMove));
					b.Place(Xmoves.at(b), 'X');
				}
				else
				{
					Xmoves.insert(make_pair(b, m.TakeTurn(b)));
					b.Place(Xmoves.at(b), 'X');
				}
			}
			else
			{
				if (playerFirst == 0)
				{
					// get playerMove
					while (true)
					{
						try
						{
							playerMove = TakePlayerTurn(b, name);
							break;
						}
						catch (Board::BadPos)
						{
							system("cls");
							PrintTitleBar(m);
							cout << endl;
							if (learning)
							{
								cout << name << " learning." << endl;
							}
							else
							{
								cout << name << " static." << endl;
							}
							cout << endl;
							cout << "Player Wins: " << playerWins << endl;
							cout << name << " Wins: " << menaceWins << endl;
							cout << "Ties: " << ties << endl;
							cout << endl;
							b.Print();
							cout << endl;
							cout << "Those were not valid coordinates." << endl;
							cout << endl;
						}
						catch (Board::PosTaken)
						{
							system("cls");
							PrintTitleBar(m);
							cout << endl;
							if (learning)
							{
								cout << name << " learning." << endl;
							}
							else
							{
								cout << name << " static." << endl;
							}
							cout << endl;
							cout << "Player Wins: " << playerWins << endl;
							cout << name << " Wins: " << menaceWins << endl;
							cout << "Ties: " << ties << endl;
							cout << endl;
							b.Print();
							cout << endl;
							cout << "That position is already taken." << endl;
							cout << endl;
						}
					}
					Omoves.insert(make_pair(b, playerMove));
					b.Place(Omoves.at(b), 'O');
				}
				else
				{
					Omoves.insert(make_pair(b, m.TakeTurn(b)));
					b.Place(Omoves.at(b), 'O');
				}
			}
		}

		// prepare winner
		if (learning && b.Winner() == 'X' && playerFirst == 1)
		{
			winner = 1;
			m.Learn(Xmoves, Omoves);
		}
		else if (learning && b.Winner() == 'O' && playerFirst == 0)
		{
			winner = 1;
			m.Learn(Omoves, Xmoves);
		}
		else if (learning && b.Winner() == 'X' && playerFirst == 0)
		{
			winner = 0;
			m.Learn(Xmoves, Omoves);
		}
		else if (learning && b.Winner() == 'O' && playerFirst == 1)
		{
			winner = 0;
			m.Learn(Omoves, Xmoves);
		}
		else if (b.Winner() == 'X' && playerFirst == 1 || b.Winner() == 'O' && playerFirst == 0)
		{
			winner = 1;
		}
		else if (b.Winner() != ' ')
		{
			winner = 0;
		}
		else
		{
			winner = 2;
		}

		// display winner
		system("cls");
		if (winner == 1)
		{
			playerWins++;
			cout << "You win!" << endl;
		}
		else if (winner == 0)
		{
			menaceWins++;
			cout << name << " wins!" << endl;
		}
		else
		{
			ties++;
			cout << "It\'s a tie!" << endl;
		}
		cout << endl;
		b.Print();
		cout << endl;
		cout << "Enter 1 to play again with the same settings." << endl;
		cout << "Enter 2 to play again with different settings." << endl;
		cout << "Enter 3 to return to the Home Screen." << endl;
		cout << endl;
		getline(cin, input);
		while (input != "1" && input != "2" && input != "3")
		{
			system("cls");
			if (winner == 1)
			{
				cout << "You win!" << endl;
			}
			else if (winner == 0)
			{
				cout << name << " wins!" << endl;
			}
			else if (winner == 2)
			{
				ties++;
				cout << "It\'s a tie!" << endl;
			}
			cout << endl;
			b.Print();
			cout << endl;
			cout << "That was not a valid choice." << endl;
			cout << endl;
			cout << "Enter 1 to play again with the same settings." << endl;
			cout << "Enter 2 to play again with different settings." << endl;
			cout << "Enter 3 to return to the Home Screen." << endl;
			cout << endl;
			getline(cin, input);
		}

		// reset
		m.Reset();
		b.Reset();
		Xmoves.clear();
		Omoves.clear();

		// repeat
		if (input == "1")
		{
			changeSettings = false;
		}
		else if (input == "2")
		{
			changeSettings = true;
		}
		else
		{
			return;
		}
	}
}
void GM::SoloTrain(Menace& m, const unsigned long n)
{
	Board b;

	map<const Board, const Board::Position> Xmoves;
	map<const Board, const Board::Position> Omoves;

	for (unsigned long i = 0; i < n; i++)
	{
		while (b.Winner() == ' ' && b.PositionsLeft() > 0)
		{
			if (b.PositionsLeft() % 2 == 1)
			{
				Xmoves.insert(make_pair(b, m.TakeTurn(b)));
				b.Place(Xmoves.at(b), 'X');
#ifdef _SHOW_MOVES
				cout << "X\'s turn:" << endl;
				cout << endl;
				b.Print();
				cout << endl;
#endif // _SHOW_MOVES

			}
			else
			{
				Omoves.insert(make_pair(b, m.TakeTurn(b)));
				b.Place(Omoves.at(b), 'O');
#ifdef _SHOW_MOVES
				cout << "O\'s turn:" << endl;
				cout << endl;
				b.Print();
				cout << endl;
#endif // _SHOW_MOVES
			}
		}

		if (b.Winner() == 'X')
		{
			m.Learn(Xmoves, Omoves);
		}
		else if (b.Winner() == 'O')
		{
			m.Learn(Omoves, Xmoves);
		}

		m.Reset();
		b.Reset();
		Xmoves.clear();
		Omoves.clear();

#ifdef _SHOW_PROGRESS
		if (i % (n / counterInverval) == 0)
		{
			cout << 100.0 * i / n << '%' << endl;
		}
#endif // _SHOW_PROGRESS
	}
}
void GM::PartnerTrain(Menace& A, Menace& B, const unsigned long n)
{
	Board b;
	minstd_rand rng;

	map<const Board, const Board::Position> Xmoves;
	map<const Board, const Board::Position> Omoves;
	Menace* X;
	Menace* O;

	for (unsigned long i = 0; i < n; i++)
	{
		rng.seed((unsigned int)chrono::system_clock::now().time_since_epoch().count());

		if (rng() % 2 == 0)
		{
			X = &A;
			O = &B;
		}
		else
		{
			X = &B;
			O = &A;
		}

		while (b.Winner() == ' ' && b.PositionsLeft() > 0)
		{
			if (b.PositionsLeft() % 2 == 1)
			{
				Xmoves.insert(make_pair(b, X->TakeTurn(b)));
				b.Place(Xmoves.at(b), 'X');
#ifdef _SHOW_MOVES
				cout << "X\'s turn:" << endl;
				cout << endl;
				b.Print();
				cout << endl;
#endif // _SHOW_MOVES
			}
			else
			{
				Omoves.insert(make_pair(b, O->TakeTurn(b)));
				b.Place(Omoves.at(b), 'O');
#ifdef _SHOW_MOVES
				cout << "O\'s turn:" << endl;
				cout << endl;
				b.Print();
				cout << endl;
#endif // _SHOW_MOVES
			}
		}

		if (b.Winner() == 'X')
		{
			A.Learn(Xmoves, Omoves);
			B.Learn(Xmoves, Omoves);
		}
		else if (b.Winner() == 'O')
		{
			A.Learn(Omoves, Xmoves);
			B.Learn(Omoves, Xmoves);
		}

		A.Reset();
		B.Reset();
		b.Reset();
		Xmoves.clear();
		Omoves.clear();

#ifdef _SHOW_PROGRESS
		if (i % (n / counterInverval) == 0)
		{
			cout << 100.0 * i / n << '%' << endl;
		}
#endif // _SHOW_PROGRESS
	}
}
void GM::PrintTitleBar(const Menace& m)
{
	string name = m.name;
	name[0] = toupper(name[0]);
	cout << "Menace - " << name << endl;

	if (m.lastLearnCycle)
	{
		char buffer[80];
		struct tm * timeinfo;
		timeinfo = localtime(&m.lastLearnCycle);
		strftime(buffer, 80, "%x at %r %Z", timeinfo);
		cout << "Last learn cycle completed on " << buffer << endl;
	}
}
bool GM::GetLearning(const Menace& m)
{
	string input;
	string name = m.name;
	name[0] = toupper(name[0]);

	system("cls");
	cout << "Enter 1 to play a static version of " << name << "." << endl;
	cout << "Enter 2 to play a dynamic version of " << name << " that learns from her experiences with you." << endl;
	cout << endl;
	getline(cin, input);
	while (input != "1" && input != "2")
	{
		system("cls");
		cout << "That was not a valid choice." << endl;
		cout << endl;
		cout << "Enter 1 to play a static version of " << name << "." << endl;
		cout << "Enter 2 to play a dynamic version of " << name << " that learns from her experiences with you." << endl;
		cout << endl;
		getline(cin, input);
	}

	if (input == "1")
	{
		return false;
	}
	else
	{
		return true;
	}
}
unsigned short GM::GetTurn()
{
	string input;

	system("cls");
	cout << "Enter 1 to go first." << endl;
	cout << "Enter 2 to go second." << endl;
	cout << "Enter 3 to randomly go first or second." << endl;
	cout << endl;
	getline(cin, input);
	while (input != "1" && input != "2" && input != "3")
	{
		system("cls");
		cout << "That was not a valid choice." << endl;
		cout << endl;
		cout << "Enter 1 to go first." << endl;
		cout << "Enter 2 to go second." << endl;
		cout << "Enter 3 to randomly go first or second." << endl;
		cout << endl;
		getline(cin, input);
	}

	if (input == "1")
	{
		return 1;
	}
	else if (input == "2")
	{
		return 0;
	}
	else
	{
		return 2;
	}
}
const Board::Position GM::TakePlayerTurn(const Board b, const string n)
{
	char you;
	string input;

	if (b.PositionsLeft() % 2 == 1)
	{
		you = 'X';
		cout << "You are X." << endl;
		cout << n << " is O." << endl;
	}
	else
	{
		you = 'O';
		cout << "You are O." << endl;
		cout << n << " is X." << endl;
	}
	cout << "Enter the coordinates of the position you would like to place an " << you << "." << endl;
	cout << endl;
	getline(cin, input);

	if (input.size() != 2)
	{
		throw Board::BadPos();
	}
	if (!((isalpha(input[0]) && isdigit(input[1])) || (isalpha(input[1]) && isdigit(input[0]))))
	{
		throw Board::BadPos();
	}
	if (isalpha(input[0]) && isdigit(input[1]))
	{
		input[0] = toupper(input[0]);
	}
	else if (isalpha(input[1]) && isdigit(input[0]))
	{
		swap(input[1], input[0]);
		input[0] = toupper(input[0]);
	}

	if (input != "A1" && input != "B1" && input != "C1" && input != "A2" && input != "B2" && input != "C2" && input != "A3" && input != "B3" && input != "C3")
	{
		throw Board::BadPos();
	}

	if (b.At(Board::StringToPos(input)) != ' ')
	{
		throw Board::PosTaken(b.At(Board::StringToPos(input)));
	}

	return Board::StringToPos(input);
}