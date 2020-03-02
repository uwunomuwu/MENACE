#pragma once

#include "Menace.h"
#include <fstream>
#include <array>
#include <set>

class Bree : public Menace
{
public:
	Bree(const double p = 0.00002, const unsigned short prs = 5, const unsigned short rs = 3, const unsigned short ps = 1, const string n = "bree");
	virtual ~Bree();

	virtual const Board::Position TakeTurn(const Board) override;
	virtual void Learn(const map<const Board, const Board::Position>&, const map<const Board, const Board::Position>&) override;
	virtual void Reset() override;

protected:
	class PMap : public array<unsigned long, 9>
	{
	public:
		PMap() { fill(0); }
		const unsigned long Sum() const { return at(0) + at(1) + at(2) + at(3) + at(4) + at(5) + at(6) + at(7) + at(8); }
		Board::Position AtV(long v) const
		{
			if (v < 0 || v > (long)(Sum() - 1))
			{
				throw out_of_range("Value passed was negative or larger than sum");
			}

			v++;

			for (unsigned short i = 0; i < 9; i++)
			{
				if (v > 0)
				{
					v -= at(i);
				}
				
				if (v <= 0)
				{
					return (Board::Position)i;
				}
			}

			throw out_of_range("Value passed was negative or larger than sum");
		}
	};

	fstream file;
	map<const Board, PMap> compendium;

	unsigned short playerRewardSize;
	unsigned short rewardSize;
	unsigned short punishmentSize;
	double precision;
	map<const unsigned short, const Board::Transformation> XmoveSet;
	map<const unsigned short, const Board::Transformation> OmoveSet;

	virtual void Reward(const map<const Board, const Board::Position>&, const char);
	virtual void Copy(const map<const Board, const Board::Position>&);
	virtual void Punish(const map<const Board, const Board::Position>&, const char);
	virtual void AddPages(const Board);
	virtual void ShrinkPossibilities(const Board);
};

