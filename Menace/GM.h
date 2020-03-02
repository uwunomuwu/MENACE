#pragma once

#include "Board.h"
#include "Menace.h"

class GM
{
public:
	GM(const unsigned long ci = 100);
	~GM();

	void operator()(Menace&);
	void SoloTrain(Menace&, const unsigned long n = 5);
	void PartnerTrain(Menace&, Menace&, const unsigned long n = 5);

private:
	unsigned long counterInverval;

	void PrintTitleBar(const Menace&);
	bool GetLearning(const Menace&);
	unsigned short GetTurn();
	const Board::Position TakePlayerTurn(const Board, const string);
};