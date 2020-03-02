#pragma once

#include "Board.h"
#include <ctime>
#include <exception>

#include <map>
#include <string>
using namespace std;

class Menace
{
public:
	Menace(const string);
	virtual ~Menace();

	class NoFile : public exception
	{

	};
	class BadFile : public exception
	{

	};

	string name;
	time_t lastLearnCycle;

	virtual const Board::Position TakeTurn(const Board) = 0;
	virtual void Learn(const map<const Board, const Board::Position>&, const map<const Board, const Board::Position>&) = 0;
	virtual void Reset() = 0;
};

