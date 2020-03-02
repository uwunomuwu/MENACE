#include "Lucy.h"
#include <vector>
#include <chrono>
#include <random>
using namespace std;

Lucy::Lucy(const string n) : Menace(n)
{
	lastLearnCycle = 0;
}
Lucy::~Lucy()
{
}
const Board::Position Lucy::TakeTurn(const Board b)
{
	vector<Board::Position> possibilities;

	for (unsigned short i = 0; i < 9; i++)
	{
		if (b.At((Board::Position)i) == ' ')
		{
			possibilities.push_back((Board::Position)i);
		}
	}

	minstd_rand rng;
	rng.seed((unsigned int)chrono::system_clock::now().time_since_epoch().count());

	return possibilities.at(rng() % possibilities.size());
}
void Lucy::Learn(const map<const Board, const Board::Position>& w, const map<const Board, const Board::Position>& l)
{

}
void Lucy::Reset()
{

}