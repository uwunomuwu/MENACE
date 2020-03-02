#include "Eclair.h"
#include <vector>
#include <chrono>
#include <random>
#include <cstdlib>
#include <iostream>

Eclair::Eclair(const string n) : Menace(n)
{
	Xturns = 0;
	Oturns = 0;

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
Eclair::~Eclair()
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
const Board::Position Eclair::TakeTurn(const Board b)
{
	if (compendium.count(b) == 0)
	{
		AddPages(b);
	}

	auto matchRange = compendium.equal_range(b);
	vector<Board::Position> possibilities;
	for (auto it = matchRange.first; it != matchRange.second; it++)
	{
		possibilities.push_back(it->second);
	}

	if (b.PositionsLeft() % 2 == 1)
	{
		Xturns++;
	}
	else
	{
		Oturns++;
	}

	minstd_rand rng;
	rng.seed((unsigned int)chrono::system_clock::now().time_since_epoch().count());

	return possibilities.at(rng() % possibilities.size());
}
void Eclair::Learn(const map<const Board, const Board::Position>& w, const map<const Board, const Board::Position>& l)
{
	if (abs(Xturns - Oturns) <= 1)
	{
		Punish(l);
	}

	time(&lastLearnCycle);
}
void Eclair::Reset()
{
	Xturns = 0;
	Oturns = 0;
}
void Eclair::Punish(const map<const Board, const Board::Position>& bads)
{
	for (auto itBad = bads.cbegin(); itBad != bads.cend(); itBad++)
	{
		pair<multimap<const Board, const Board::Position>::const_iterator, multimap<const Board, const Board::Position>::const_iterator> matchRange = compendium.equal_range(itBad->first);
		for (auto itComp = matchRange.first; itComp != matchRange.second; itComp++)
		{
			if (*itBad == *itComp)
			{
				compendium.erase(itComp);
				break;
			}
		}
	}
}
void Eclair::AddPages(const Board b)
{
	for (unsigned short i = 0; i < 9; i++)
	{
		if (b.At((Board::Position)i) == ' ')
		{
			compendium.insert(make_pair(b, (Board::Position)i));
		}
	}
}