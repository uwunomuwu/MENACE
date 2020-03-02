#include "Tiramisu.h"
#include <random>
#include <chrono>
using namespace std;

Tiramisu::Tiramisu(const double p, const unsigned short prs, const unsigned short rs, const unsigned short ps, const string n) : Aura(p, prs, rs, ps, n)
{
}
Tiramisu::~Tiramisu()
{
}
const Board::Position Tiramisu::TakeTurn(const Board b)
{
	bool boardFound = false;
	Board::Transformation t = Board::R0;
	for (unsigned short i = 0; i < 8 && boardFound == false; i++)
	{
		if (compendium.count(b.Transform((Board::Transformation)i)) != 0)
		{
			t = (Board::Transformation)i;
			boardFound = true;
		}
	}
	if (!boardFound)
	{
		AddPages(b);
	}

	if (compendium.at(b.Transform(t)).Sum() == 0)
	{
		compendium.erase(b.Transform(t));
		AddPages(b.Transform(t));
	}

	while (compendium.at(b.Transform(t)).Sum() > (1 / precision))
	{
		ShrinkPossibilities(b.Transform(t));
	}

	minstd_rand rng;
	rng.seed((unsigned)chrono::system_clock::now().time_since_epoch().count());
	unsigned long rnum = rng() % compendium.at(b.Transform(t)).Sum();

	if (b.PositionsLeft() % 2 == 1)
	{
		XmoveSet.insert(make_pair(b.PositionsLeft(), t));
	}
	else
	{
		OmoveSet.insert(make_pair(b.PositionsLeft(), t));
	}

	return Board::InverseTransform(compendium.at(b.Transform(t)).AtV(rnum), t);
}
void Tiramisu::AddPages(const Board b)
{
	PMap temp;
	Board tempBoard;

	tempBoard.Place(Board::B2, 'X');
	if (b == tempBoard)
	{
		temp[0] = 1;
		temp[2] = 1;
		temp[6] = 1;
		temp[8] = 1;
		compendium.insert(make_pair(b, temp));
		return;
	}

	tempBoard.Place(Board::B1, 'O');
	if (b == tempBoard || b == tempBoard.Transform(Board::R180))
	{
		temp[3] = 1;
		temp[5] = 1;
		compendium.insert(make_pair(b, temp));
		return;
	}
	else if (b == tempBoard.Transform(Board::R90) || b == tempBoard.Transform(Board::R270))
	{
		temp[1] = 1;
		temp[7] = 1;
		compendium.insert(make_pair(b, temp));
		return;
	}

	tempBoard.Place(Board::A2, 'X');
	tempBoard.Place(Board::C2, 'O');
	if (b == tempBoard)
	{
		temp[6] = 1;
		compendium.insert(make_pair(b, temp));
		return;
	}

	for (unsigned short i = 0; i < 9; i++)
	{
		if (b.At((Board::Position)i) == ' ')
		{
			temp[i] = b.PositionsLeft();
		}
	}

	compendium.insert(make_pair(b, temp));
}