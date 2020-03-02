#pragma once

#include "Menace.h"
#include <fstream>

class Eclair : public Menace
{
public:
	Eclair(const string n = "eclair");
	~Eclair();

	virtual const Board::Position TakeTurn(const Board) override;
	virtual void Learn(const map<const Board, const Board::Position>&, const map<const Board, const Board::Position>&) override;
	virtual void Reset() override;

private:
	fstream file;
	multimap<const Board, const Board::Position> compendium;

	unsigned short Xturns;
	unsigned short Oturns;

	void Punish(const map<const Board, const Board::Position>&);
	void AddPages(const Board);
};

