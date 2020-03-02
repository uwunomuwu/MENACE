#include "Bree.h"
#include <chrono>
#include <random>
#include <iostream>
using namespace std;

Bree::Bree(const double p, const unsigned short prs , const unsigned short rs, const unsigned short ps, const string n) : Menace(n)
{
	precision = p;
	playerRewardSize = prs;
	rewardSize = rs;
	punishmentSize = ps;

	string fname = name + ".menace";
	file.open(fname.c_str(), ios::in);
	if (!file.is_open())
	{
		throw NoFile();
	}

	string tempName;
	getline(file, tempName);
	if (tempName != name)
	{
		throw BadFile();
	}

	file >> lastLearnCycle;
	file.ignore();

	Board temp;
	PMap tempP;
	while (file.peek() != '\n' && !file.eof())
	{
		for (unsigned short i = 0; i < 9; i++)
		{
			temp.AtR((Board::Position)i) = file.get();
		}
		file.ignore();
		for (unsigned short i = 0; i < 9; i++)
		{
			file >> tempP[i];
		}
		file.ignore();
		compendium.insert(make_pair(temp, tempP));
	}
	file.close();
}
Bree::~Bree()
{
	string fname = name + ".menace";
	file.open(fname.c_str(), ios::out);

	file << name << endl;
	file << lastLearnCycle << endl;

	for (auto it = compendium.cbegin(); it != compendium.cend(); it++)
	{
		for (unsigned short i = 0; i < 9; i++)
		{
			file << it->first.At((Board::Position)i);
		}
		for (unsigned short i = 0; i < 9; i++)
		{
			file << '\t' << it->second.at(i);
		}
		file << endl;
	}
	file.close();
}
const Board::Position Bree::TakeTurn(const Board b)
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
void Bree::Learn(const map<const Board, const Board::Position>& w, const map<const Board, const Board::Position>& l)
{
	Board blank;
	if (XmoveSet.size() > 0 && OmoveSet.size() > 0)
	{
		if (w.count(blank))
		{
			Reward(w, 'X');
			Punish(l, 'O');
		}
		else
		{
			Reward(w, 'O');
			Punish(l, 'X');
		}
	}
	else if (OmoveSet.size() == 0)
	{
		if (w.count(blank))
		{
			Reward(w, 'X');
		}
		else
		{
			Copy(w);
			Punish(l, 'X');
		}
	}
	else if (XmoveSet.size() == 0)
	{
		if (w.count(blank))
		{
			Copy(w);
			Punish(l, 'O');
		}
		else
		{
			Reward(w, 'O');
		}
	}

	time(&lastLearnCycle);
}
void Bree::Reset()
{
	XmoveSet.clear();
	OmoveSet.clear();
}
void Bree::Reward(const map<const Board, const Board::Position>& goods, const char c)
{
	if (c == 'X')
	{
		for (auto it = goods.cbegin(); it != goods.cend(); it++)
		{
			compendium.at(it->first.Transform(XmoveSet.at(it->first.PositionsLeft()))).at((unsigned short)Board::Transform(it->second, XmoveSet.at(it->first.PositionsLeft()))) += rewardSize;
		}
	}
	else
	{
		for (auto it = goods.cbegin(); it != goods.cend(); it++)
		{
			compendium.at(it->first.Transform(OmoveSet.at(it->first.PositionsLeft()))).at((unsigned short)Board::Transform(it->second, OmoveSet.at(it->first.PositionsLeft()))) += rewardSize;
		}
	}
}
void Bree::Copy(const map<const Board, const Board::Position>& goods)
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

		compendium.at(it->first.Transform(t)).at((unsigned short)Board::Transform(it->second, t)) += playerRewardSize;
	}
}
void Bree::Punish(const map<const Board, const Board::Position>& bads, const char c)
{
	if (c == 'X')
	{
		for (auto it = bads.cbegin(); it != bads.cend(); it++)
		{
			compendium.at(it->first.Transform(XmoveSet.at(it->first.PositionsLeft()))).at((unsigned short)Board::Transform(it->second, XmoveSet.at(it->first.PositionsLeft()))) -= punishmentSize;
		}
	}
	else
	{
		for (auto it = bads.cbegin(); it != bads.cend(); it++)
		{
			compendium.at(it->first.Transform(OmoveSet.at(it->first.PositionsLeft()))).at((unsigned short)Board::Transform(it->second, OmoveSet.at(it->first.PositionsLeft()))) -= punishmentSize;
		}
	}
}
void Bree::AddPages(const Board b)
{
	PMap temp;

	for (unsigned short i = 0; i < 9; i++)
	{
		if (b.At((Board::Position)i) == ' ')
		{
			temp[i] = b.PositionsLeft();
		}
	}

	compendium.insert(make_pair(b, temp));
}
void Bree::ShrinkPossibilities(const Board b)
{
	for (unsigned short i = 0; i < 9; i++)
	{
		if (compendium.at(b)[i] % 2 == 0)
		{
			compendium.at(b)[i] = compendium.at(b)[i] / 2;
		}
		else
		{
			compendium.at(b)[i] = (compendium.at(b)[i] / 2) + 1;
		}
	}
}