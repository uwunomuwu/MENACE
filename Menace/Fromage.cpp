#include "Fromage.h"
#include <chrono>
#include <random>
#include <iostream>
using namespace std;

Fromage::Fromage(const double p, const unsigned short rs, const unsigned short ps, const string n) : Menace(n)
{
	precision = p;
	rewardSize = rs;
	punishmentSize = ps;

	XmoveSet.clear();
	OmoveSet.clear();

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
	unsigned short pos;
	while (file.peek() != '\n' && !file.eof())
	{
		for (unsigned short i = 0; i < 9; i++)
		{
			temp.AtR((Board::Position)i) = file.get();
		}
		pos = file.get() - 48;
		compendium.insert(make_pair(temp, (Board::Position)pos));
		file.ignore();
	}
	file.close();
}
Fromage::~Fromage()
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
		file << (unsigned short)it->second;
		file << endl;
	}
	file.close();
}
const Board::Position Fromage::TakeTurn(const Board b)
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

	auto matchRange = compendium.equal_range(b.Transform(t));
	multiset<Board::Position> possibilities;

	for (auto it = matchRange.first; it != matchRange.second; it++)
	{
		possibilities.insert(it->second);
	}

	while (possibilities.size() > (1 / precision))
	{
		possibilities = ShrinkPossibilities(b.Transform(t), possibilities);
	}

	minstd_rand rng;
	rng.seed((unsigned)chrono::system_clock::now().time_since_epoch().count());
	unsigned short rnum = unsigned short(rng() % possibilities.size());
	auto randIt = possibilities.begin();
	for (unsigned short i = 0; i < rnum; i++)
	{
		randIt++;
	}

	if (b.PositionsLeft() % 2 == 1)
	{
		XmoveSet.insert({ {b.PositionsLeft(), t} });
	}
	else
	{
		OmoveSet.insert({ {b.PositionsLeft(), t} });
	}

	return Board::InverseTransform(*randIt, t);
}
void Fromage::Learn(const map<const Board, const Board::Position>& w, const map<const Board, const Board::Position>& l)
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
		if (w.count(blank) > 0)
		{
			Reward(w, 'X');
		}
		else
		{
			Punish(l, 'X');
		}
	}
	else if (XmoveSet.size() == 0)
	{
		if (w.count(blank) > 0)
		{
			Punish(l, 'O');
		}
		else
		{
			Reward(w, 'O');
		}
	}

	time(&lastLearnCycle);
}
void Fromage::Reset()
{
	XmoveSet.clear();
	OmoveSet.clear();
}
void Fromage::Reward(const map<const Board, const Board::Position>& goods, const char c)
{
	if (c == 'X')
	{
		for (auto it = goods.cbegin(); it != goods.cend(); it++)
		{
			for (unsigned short i = 0; i < rewardSize; i++)
			{
				compendium.insert(make_pair(it->first.Transform(XmoveSet.at(it->first.PositionsLeft())),
											Board::Transform(it->second, XmoveSet.at(it->first.PositionsLeft()))));
			}
		}
	}
	else
	{
		for (auto it = goods.cbegin(); it != goods.cend(); it++)
		{
			for (unsigned short i = 0; i < rewardSize; i++)
			{
				compendium.insert(make_pair(it->first.Transform(OmoveSet.at(it->first.PositionsLeft())),
											Board::Transform(it->second, OmoveSet.at(it->first.PositionsLeft()))));
			}
		}
	}
}
void Fromage::Punish(const map<const Board, const Board::Position>& bads, const char c)
{
	if (c == 'X')
	{
		for (auto itBad = bads.cbegin(); itBad != bads.cend(); itBad++)
		{
			for (unsigned short i = 0; i < punishmentSize; i++)
			{
				auto matchRange = compendium.equal_range(itBad->first.Transform(XmoveSet.at(itBad->first.PositionsLeft())));
				for (auto itComp = matchRange.first; itComp != matchRange.second; itComp++)
				{
					if (Board::Transform(itBad->second, XmoveSet.at(itBad->first.PositionsLeft())) == itComp->second)
					{
						compendium.erase(itComp);
						break;
					}
				}
			}
		}
	}
	else
	{
		for (auto itBad = bads.cbegin(); itBad != bads.cend(); itBad++)
		{
			for (unsigned short i = 0; i < punishmentSize; i++)
			{
				auto matchRange = compendium.equal_range(itBad->first.Transform(OmoveSet.at(itBad->first.PositionsLeft())));
				for (auto itComp = matchRange.first; itComp != matchRange.second; itComp++)
				{
					if (Board::Transform(itBad->second, OmoveSet.at(itBad->first.PositionsLeft())) == itComp->second)
					{
						compendium.erase(itComp);
						break;
					}
				}
			}
		}
	}
}
void Fromage::EraseAll(const multimap<const Board, const Board::Position>& bads)
{
	for (auto itBad = bads.cbegin(); itBad != bads.cend(); itBad++)
	{
		auto matchRange = compendium.equal_range(itBad->first);
		for (auto itComp = matchRange.first; itComp != matchRange.second; itComp++)
		{
			if (itBad->second == itComp->second)
			{
				compendium.erase(itComp);
				break;
			}
		}
	}
}
void Fromage::AddPages(const Board b)
{
	for (unsigned short i = 0; i < 9; i++)
	{
		if (b.At((Board::Position)i) == ' ')
		{
			for (unsigned short j = 0; j < b.PositionsLeft(); j++)
			{
				compendium.insert(make_pair(b, (Board::Position)i));
			}
		}
	}
}
const multiset<Board::Position> Fromage::ShrinkPossibilities(const Board b, const multiset<Board::Position>& p)
{
	multimap<const Board, const Board::Position> bads;
	multiset<Board::Position> shrunk;

	auto matchRange = p.equal_range(*p.cbegin());
	if (matchRange.second == p.cend())
	{
		for (unsigned short i = 0; i < p.size() / 2; i++)
		{
			bads.insert(make_pair(b, *p.cbegin()));
			shrunk.insert(*p.cbegin());
		}
		if (p.size() % 2 == 1)
		{
			shrunk.insert(*p.cbegin());
		}
	}

	auto tempPtr = p.begin();
	unsigned short count = 0;
	for (auto it = p.begin(); it != p.end(); it++)
	{
		if (*it != *tempPtr)
		{
			for (unsigned short i = 0; i < count / 2; i++)
			{
				bads.insert(make_pair(b, *tempPtr));
				shrunk.insert(*tempPtr);
			}
			if (count % 2 == 1)
			{
				shrunk.insert(*tempPtr);
			}
			count = 0;
			tempPtr = it;
		}
		count++;
	}

	EraseAll(bads);

	return shrunk;
}