#pragma once

#include <exception>
#include <string>
using namespace std;

class Board
{
public:
	Board();
	~Board();

	class PosTaken : public exception
	{
	private:
		char c;
	public:
		PosTaken(char a) { c = a; }
		char Other() { return c; }
		const char* what() const override
		{
			string msg = "That position is already taken by ";
			msg += c;
			msg += ".";
			return msg.c_str();
		}
	};
	class BadChar : public exception
	{
	private:
		char c;
	public:
		BadChar(char a) { c = a; }
		char Bad() { return c; }
		const char* what() const override
		{
			string msg = "Only X\'s and O\'s can be placed.\n";
			msg += c;
			msg += " was not an X or an O.";
			return msg.c_str();
		}
	};
	class BadPos : public exception
	{
		const char* what() const override
		{
			return "The position provided was not valid.";
		}
	};
	class BadTrans : public exception
	{
		const char* what() const override
		{
			return "The transformation provided was not valid.";
		}
	};

	enum Position {A1, B1, C1, A2, B2, C2, A3, B3, C3};
	enum Transformation {R0, R90, R180, R270, H, V, D, AD};

	const bool operator==(const Board) const;
	const bool operator<(const Board) const;
	const Board Transform(const Transformation) const;
	static const Board::Position Transform(const Board::Position, const Transformation);
	static const Board::Position InverseTransform(const Board::Position, const Transformation);
	static const Board::Position StringToPos(const string&);

	const char At(const Position) const;
	char& AtR(const Position);
	void Place(const Position, const char);
	const unsigned short PositionsLeft() const;
	void Print() const;
	void Reset();
	const char Winner() const;

private:
	char b[9];

	const Board R90b() const;
	const Board R180b() const;
	const Board R270b() const;
	const Board Hb() const;
	const Board Vb() const;
	const Board Db() const;
	const Board ADb() const;
	static const Position R90p(const Position);
	static const Position R180p(const Position);
	static const Position R270p(const Position);
	static const Position Hp(const Position);
	static const Position Vp(const Position);
	static const Position Dp(const Position);
	static const Position ADp(const Position);
};