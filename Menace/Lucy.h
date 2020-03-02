#pragma once

#include "Menace.h"

class Lucy : public Menace
{
public:
	Lucy(const string n = "lucy");
	~Lucy();

	virtual const Board::Position TakeTurn(const Board);
	virtual void Learn(const map<const Board, const Board::Position>&, const map<const Board, const Board::Position>&);
	virtual void Reset();
};

