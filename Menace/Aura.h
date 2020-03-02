#pragma once

#include "Bree.h"
#include <fstream>
#include <array>
#include <set>

class Aura : public Bree
{
public:
	Aura(const double p = 0.00002, const unsigned short prs = 5, const unsigned short rs = 3, const unsigned short ps = 1, const string n = "aura");
	virtual ~Aura();

	virtual const Board::Position TakeTurn(const Board) override;

private:
	void Reward(const map<const Board, const Board::Position>&, const char) override;
	void Copy(const map<const Board, const Board::Position>&) override;
	void Punish(const map<const Board, const Board::Position>&, const char) override;

	static const pair<const Board::Position, const bool> Check2(Board, const char);
};