#include "Aura.h"
#include <chrono>
#include <random>
#include <iostream>
using namespace std;

Aura::Aura(const double p, const unsigned short prs, const unsigned short rs, const unsigned short ps, const string n) : Bree(p, prs, rs, ps, n)
{
}
Aura::~Aura()
{
}
const Board::Position Aura::TakeTurn(const Board b)
{
	if (b.PositionsLeft() % 2 == 1)
	{
		if (Check2(b, 'X').second)
		{
			return Check2(b, 'X').first;
		}
		else if (Check2(b, 'O').second)
		{
			return Check2(b, 'O').first;
		}
	}
	else
	{
		if (Check2(b, 'O').second)
		{
			return Check2(b, 'O').first;
		}
		else if (Check2(b, 'X').second)
		{
			return Check2(b, 'X').first;
		}
	}

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
void Aura::Reward(const map<const Board, const Board::Position>& goods, const char c)
{
	if (c == 'X')
	{
		for (auto it = goods.cbegin(); it != goods.cend(); it++)
		{
			if (XmoveSet.count(it->first.PositionsLeft()))
			{
				compendium.at(it->first.Transform(XmoveSet.at(it->first.PositionsLeft()))).at((unsigned short)Board::Transform(it->second, XmoveSet.at(it->first.PositionsLeft()))) += rewardSize;
			}
		}
	}
	else
	{
		for (auto it = goods.cbegin(); it != goods.cend(); it++)
		{
			if (OmoveSet.count(it->first.PositionsLeft()))
			{
				compendium.at(it->first.Transform(OmoveSet.at(it->first.PositionsLeft()))).at((unsigned short)Board::Transform(it->second, OmoveSet.at(it->first.PositionsLeft()))) += rewardSize;
			}
		}
	}
}
void Aura::Copy(const map<const Board, const Board::Position>& goods)
{
	for (auto it = goods.cbegin(); it != goods.cend(); it++)
	{
		bool boardFound = false;
		Board::Transformation t = Board::R0;
		for (unsigned short i = 0; i < 8 && boardFound == false; i++)
		{
			if (compendium.count(it->first.Transform((Board::Transformation)i)) != 0)
			{
				t = (Board::Transformation)i;
				boardFound = true;
			}
		}
		if (!boardFound)
		{
			AddPages(it->first);
		}

		if (!(Check2(it->first, 'X').second || Check2(it->first, 'O').second))
		{
			compendium.at(it->first.Transform(t)).at((unsigned short)Board::Transform(it->second, t)) += playerRewardSize;
		}
	}
}
void Aura::Punish(const map<const Board, const Board::Position>& bads, const char c)
{
	if (c == 'X')
	{
		for (auto it = bads.cbegin(); it != bads.cend(); it++)
		{
			if (XmoveSet.count(it->first.PositionsLeft()))
			{
				compendium.at(it->first.Transform(XmoveSet.at(it->first.PositionsLeft()))).at((unsigned short)Board::Transform(it->second, XmoveSet.at(it->first.PositionsLeft()))) -= punishmentSize;
			}
		}
	}
	else
	{
		for (auto it = bads.cbegin(); it != bads.cend(); it++)
		{
			if (OmoveSet.count(it->first.PositionsLeft()))
			{
				compendium.at(it->first.Transform(OmoveSet.at(it->first.PositionsLeft()))).at((unsigned short)Board::Transform(it->second, OmoveSet.at(it->first.PositionsLeft()))) -= punishmentSize;
			}
		}
	}
}
const pair<const Board::Position, const bool> Aura::Check2(Board b, const char c)
{
	for (unsigned short i = 0; i < 2; i++, b = b.Transform(Board::R90))
	{
		for (unsigned short j = 0; j < 3; j++)
		{
			if (b.At(Board::Position(j)) == c && (b.At(Board::Position(j)) == b.At(Board::Position(j + 3)) && b.At(Board::Position(j + 6)) == ' '))
			{
				if (i == 0)
				{
					return make_pair(Board::Position(j + 6), true);
				}
				else
				{
					return make_pair(Board::InverseTransform(Board::Position(j + 6), Board::R90), true);
				}
			}
			else if (b.At(Board::Position(j)) == c && (b.At(Board::Position(j)) == b.At(Board::Position(j + 6)) && b.At(Board::Position(j + 3)) == ' '))
			{
				if (i == 0)
				{
					return make_pair(Board::Position(j + 3), true);
				}
				else
				{
					return make_pair(Board::InverseTransform(Board::Position(j + 3), Board::R90), true);
				}
			}
			else if (b.At(Board::Position(j + 3)) == c && (b.At(Board::Position(j + 3)) == b.At(Board::Position(j + 6)) && b.At(Board::Position(j)) == ' '))
			{
				if (i == 0)
				{
					return make_pair(Board::Position(j), true);
				}
				else
				{
					return make_pair(Board::InverseTransform(Board::Position(j), Board::R90), true);
				}
			}
		}
		if (b.At(Board::A1) == c && (b.At(Board::A1) == b.At(Board::B2) && b.At(Board::C3) == ' '))
		{
			if (i == 0)
			{
				return make_pair(Board::C3, true);;
			}
			else
			{
				return make_pair(Board::InverseTransform(Board::C3, Board::R90), true);
			}
		}
		else if (b.At(Board::A1) == c && (b.At(Board::A1) == b.At(Board::C3) && b.At(Board::B2) == ' '))
		{
			if (i == 0)
			{
				return make_pair(Board::B2, true);
			}
			else
			{
				return make_pair(Board::InverseTransform(Board::B2, Board::R90), true);
			}
		}
		else if (b.At(Board::B2) == c && (b.At(Board::B2) == b.At(Board::C3) && b.At(Board::A1) == ' '))
		{
			if (i == 0)
			{
				return make_pair(Board::A1, true);
			}
			else
			{
				return make_pair(Board::InverseTransform(Board::A1, Board::R90), true);
			}
		}
	}

	return make_pair(Board::A1, false);
}