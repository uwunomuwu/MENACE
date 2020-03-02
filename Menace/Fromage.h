#pragma once

#include "Menace.h"
#include <fstream>
#include <set>

class Fromage : public Menace
{
public:
	Fromage(const double p = 0.01, const unsigned short rs = 3, const unsigned short ps = 1, const string n = "fromage");
	~Fromage();

	virtual const Board::Position TakeTurn(const Board) override;
	virtual void Learn(const map<const Board, const Board::Position>&, const map<const Board, const Board::Position>&) override;
	virtual void Reset() override;

private:
	fstream file;
	multimap<const Board, const Board::Position> compendium;

	double precision;
	unsigned short rewardSize;
	unsigned short punishmentSize;
	map<const unsigned short, const Board::Transformation> XmoveSet;
	map<const unsigned short, const Board::Transformation> OmoveSet;

	void Reward(const map<const Board, const Board::Position>&, const char);
	void Punish(const map<const Board, const Board::Position>&, const char);
	void EraseAll(const multimap<const Board, const Board::Position>&);
	void AddPages(const Board);
	const multiset<Board::Position> ShrinkPossibilities(const Board, const multiset<Board::Position>&);
};

