#pragma once
#include "Aura.h"
class Tiramisu : public Aura
{
public:
	Tiramisu(const double p = 0.00002, const unsigned short prs = 5, const unsigned short rs = 3, const unsigned short ps = 1, const string n = "tiramisu");
	~Tiramisu();

	virtual const Board::Position TakeTurn(const Board) override;

private:
	virtual 
	virtual void AddPages(const Board) override;
};

