#include "Board.h"
#include <iostream>
using namespace std;

Board::Board()
{
	for (unsigned short i = 0; i < 9; i++)
	{
		b[i] = ' ';
	}
}
Board::~Board()
{
}
const bool Board::operator==(const Board right) const
{
	for (unsigned short i = 0; i < 9; i++)
	{
		if (b[i] != right.At((Board::Position)i))
		{
			return false;
		}
	}
	return true;
}
const bool Board::operator<(const Board right) const
{
	for (unsigned short i = 0; i < 9; i++)
	{
		if (b[i] == right.At((Board::Position)i))
		{
			continue;
		}
		else if (b[i] < right.At((Board::Position)i))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}
const Board Board::Transform(const Transformation t) const
{
	switch (t)
	{
	case Board::R0:
		return *this;
		break;
	case Board::R90:
		return R90b();
		break;
	case Board::R180:
		return R180b();
		break;
	case Board::R270:
		return R270b();
		break;
	case Board::H:
		return Hb();
		break;
	case Board::V:
		return Vb();
		break;
	case Board::D:
		return Db();
		break;
	case Board::AD:
		return ADb();
		break;
	default:
		throw BadTrans();
		break;
	}
}
const Board::Position Board::Transform(const Board::Position p, const Transformation t)
{
	switch (t)
	{
	case Board::R0:
		return p;
		break;
	case Board::R90:
		return R90p(p);
		break;
	case Board::R180:
		return R180p(p);
		break;
	case Board::R270:
		return R270p(p);
		break;
	case Board::H:
		return Hp(p);
		break;
	case Board::V:
		return Vp(p);
		break;
	case Board::D:
		return Dp(p);
		break;
	case Board::AD:
		return ADp(p);
		break;
	default:
		throw BadTrans();
		break;
	}
}
const Board::Position Board::InverseTransform(const Board::Position p, const Transformation t)
{
	switch (t)
	{
	case Board::R0:
		return p;
		break;
	case Board::R90:
		return R270p(p);
		break;
	case Board::R180:
		return R180p(p);
		break;
	case Board::R270:
		return R90p(p);
		break;
	case Board::H:
		return Hp(p);
		break;
	case Board::V:
		return Vp(p);
		break;
	case Board::D:
		return Dp(p);
		break;
	case Board::AD:
		return ADp(p);
		break;
	default:
		throw BadTrans();
		break;
	}
}
const Board::Position Board::StringToPos(const string& input)
{
	if (input == "A1")
	{
		return Board::A1;
	}
	else if (input == "B1")
	{
		return Board::B1;
	}
	else if (input == "C1")
	{
		return Board::C1;
	}
	else if (input == "A2")
	{
		return Board::A2;
	}
	else if (input == "B2")
	{
		return Board::B2;
	}
	else if (input == "C2")
	{
		return Board::C2;
	}
	else if (input == "A3")
	{
		return Board::A3;
	}
	else if (input == "B3")
	{
		return Board::B3;
	}
	else if (input == "C3")
	{
		return Board::C3;
	}
	else
	{
		throw BadPos();
	}
}
const char Board::At(const Position pos) const
{
	switch (pos)
	{
	case Board::A1:
		return b[0];
		break;
	case Board::B1:
		return b[1];
		break;
	case Board::C1:
		return b[2];
		break;
	case Board::A2:
		return b[3];
		break;
	case Board::B2:
		return b[4];
		break;
	case Board::C2:
		return b[5];
		break;
	case Board::A3:
		return b[6];
		break;
	case Board::B3:
		return b[7];
		break;
	case Board::C3:
		return b[8];
		break;
	default:
		throw BadPos();
		break;
	}
}
char& Board::AtR(const Position pos)
{
	switch (pos)
	{
	case Board::A1:
		return b[0];
		break;
	case Board::B1:
		return b[1];
		break;
	case Board::C1:
		return b[2];
		break;
	case Board::A2:
		return b[3];
		break;
	case Board::B2:
		return b[4];
		break;
	case Board::C2:
		return b[5];
		break;
	case Board::A3:
		return b[6];
		break;
	case Board::B3:
		return b[7];
		break;
	case Board::C3:
		return b[8];
		break;
	default:
		throw BadPos();
		break;
	}
}
void Board::Place(const Position pos, const char c)
{
	if (!(c == 'X' || c == 'O'))
	{
		throw BadChar(c);
	}

	if (At(pos) != ' ')
	{
		throw PosTaken(At(pos));
	}

	AtR(pos) = c;
}
const unsigned short Board::PositionsLeft() const
{
	unsigned short c = 0;
	for (unsigned short i = 0; i < 9; i++)
	{
		if (b[i] == ' ')
		{
			c++;
		}
	}
	return c;
}
void Board::Print() const
{
	cout << "   | A | B | C" << endl;
	cout << "---------------" << endl;
	cout << " 1 | " << b[0] << " | " << b[1] << " | " << b[2] << endl;
	cout << "---------------" << endl;
	cout << " 2 | " << b[3] << " | " << b[4] << " | " << b[5] << endl;
	cout << "---------------" << endl;
	cout << " 3 | " << b[6] << " | " << b[7] << " | " << b[8] << endl;
}
void Board::Reset()
{
	for (unsigned short i = 0; i < 9; i++)
	{
		b[i] = ' ';
	}
}
const char Board::Winner() const
{
	for (unsigned short i = 0; i < 3; i++)
	{
		if (b[3 * i] != ' ' && b[3 * i] == b[1 + 3 * i] && b[3 * i] == b[2 + 3 * i])
		{
			return b[0 + 3 * i];
		}
		else if (b[i] != ' ' && b[i] == b[3 + i] && b[i] == b[6 + i])
		{
			return b[i];
		}
	}
	if (b[0] != ' ' && b[0] == b[4] && b[0] == b[8])
	{
		return b[0];
	}
	else if (b[2] != ' ' && b[2] == b[4] && b[2] == b[6])
	{
		return b[2];
	}
	else
	{
		return ' ';
	}
}
const Board Board::R90b() const
{
	Board temp;
	temp.AtR(A1) = At(C1);
	temp.AtR(B1) = At(C2);
	temp.AtR(C1) = At(C3);
	temp.AtR(A2) = At(B1);
	temp.AtR(B2) = At(B2);
	temp.AtR(C2) = At(B3);
	temp.AtR(A3) = At(A1);
	temp.AtR(B3) = At(A2);
	temp.AtR(C3) = At(A3);
	return temp;
}
const Board Board::R180b() const
{
	Board temp;
	temp.AtR(A1) = At(C3);
	temp.AtR(B1) = At(B3);
	temp.AtR(C1) = At(A3);
	temp.AtR(A2) = At(C2);
	temp.AtR(B2) = At(B2);
	temp.AtR(C2) = At(A2);
	temp.AtR(A3) = At(C1);
	temp.AtR(B3) = At(B1);
	temp.AtR(C3) = At(A1);
	return temp;
}
const Board Board::R270b() const
{
	Board temp;
	temp.AtR(A1) = At(A3);
	temp.AtR(B1) = At(A2);
	temp.AtR(C1) = At(A1);
	temp.AtR(A2) = At(B3);
	temp.AtR(B2) = At(B2);
	temp.AtR(C2) = At(B1);
	temp.AtR(A3) = At(C3);
	temp.AtR(B3) = At(C2);
	temp.AtR(C3) = At(C1);
	return temp;
}
const Board Board::Hb() const
{
	Board temp;
	temp.AtR(A1) = At(A3);
	temp.AtR(B1) = At(B3);
	temp.AtR(C1) = At(C3);
	temp.AtR(A2) = At(A2);
	temp.AtR(B2) = At(B2);
	temp.AtR(C2) = At(C2);
	temp.AtR(A3) = At(A1);
	temp.AtR(B3) = At(B1);
	temp.AtR(C3) = At(C1);
	return temp;
}
const Board Board::Vb() const
{
	Board temp;
	temp.AtR(A1) = At(C1);
	temp.AtR(B1) = At(B1);
	temp.AtR(C1) = At(A1);
	temp.AtR(A2) = At(C2);
	temp.AtR(B2) = At(B2);
	temp.AtR(C2) = At(A2);
	temp.AtR(A3) = At(C3);
	temp.AtR(B3) = At(B3);
	temp.AtR(C3) = At(A3);
	return temp;
}
const Board Board::Db() const
{
	Board temp;
	temp.AtR(A1) = At(A1);
	temp.AtR(B1) = At(A2);
	temp.AtR(C1) = At(A3);
	temp.AtR(A2) = At(B1);
	temp.AtR(B2) = At(B2);
	temp.AtR(C2) = At(B3);
	temp.AtR(A3) = At(C1);
	temp.AtR(B3) = At(C2);
	temp.AtR(C3) = At(C3);
	return temp;
}
const Board Board::ADb() const
{
	Board temp;
	temp.AtR(A1) = At(C3);
	temp.AtR(B1) = At(C2);
	temp.AtR(C1) = At(C1);
	temp.AtR(A2) = At(B3);
	temp.AtR(B2) = At(B2);
	temp.AtR(C2) = At(B1);
	temp.AtR(A3) = At(A3);
	temp.AtR(B3) = At(A2);
	temp.AtR(C3) = At(A1);
	return temp;
}
const Board::Position Board::R90p(const Position p)
{
	switch (p)
	{
	case Board::A1:
		return A3;
		break;
	case Board::B1:
		return A2;
		break;
	case Board::C1:
		return A1;
		break;
	case Board::A2:
		return B3;
		break;
	case Board::B2:
		return B2;
		break;
	case Board::C2:
		return B1;
		break;
	case Board::A3:
		return C3;
		break;
	case Board::B3:
		return C2;
		break;
	case Board::C3:
		return C1;
		break;
	default:
		throw BadPos();
		break;
	}
}
const Board::Position Board::R180p(const Position p)
{
	switch (p)
	{
	case Board::A1:
		return C3;
		break;
	case Board::B1:
		return B3;
		break;
	case Board::C1:
		return A3;
		break;
	case Board::A2:
		return C2;
		break;
	case Board::B2:
		return B2;
		break;
	case Board::C2:
		return A2;
		break;
	case Board::A3:
		return C1;
		break;
	case Board::B3:
		return B1;
		break;
	case Board::C3:
		return A1;
		break;
	default:
		throw BadPos();
		break;
	}
}
const Board::Position Board::R270p(const Position p)
{
	switch (p)
	{
	case Board::A1:
		return C1;
		break;
	case Board::B1:
		return C2;
		break;
	case Board::C1:
		return C3;
		break;
	case Board::A2:
		return B1;
		break;
	case Board::B2:
		return B2;
		break;
	case Board::C2:
		return B3;
		break;
	case Board::A3:
		return A1;
		break;
	case Board::B3:
		return A2;
		break;
	case Board::C3:
		return A3;
		break;
	default:
		throw BadPos();
		break;
	}
}
const Board::Position Board::Hp(const Position p)
{
	switch (p)
	{
	case Board::A1:
		return A3;
		break;
	case Board::B1:
		return B3;
		break;
	case Board::C1:
		return C3;
		break;
	case Board::A2:
		return A2;
		break;
	case Board::B2:
		return B2;
		break;
	case Board::C2:
		return C2;
		break;
	case Board::A3:
		return A1;
		break;
	case Board::B3:
		return B1;
		break;
	case Board::C3:
		return C1;
		break;
	default:
		throw BadPos();
		break;
	}
}
const Board::Position Board::Vp(const Position p)
{
	switch (p)
	{
	case Board::A1:
		return C1;
		break;
	case Board::B1:
		return B1;
		break;
	case Board::C1:
		return A1;
		break;
	case Board::A2:
		return C2;
		break;
	case Board::B2:
		return B2;
		break;
	case Board::C2:
		return A2;
		break;
	case Board::A3:
		return C3;
		break;
	case Board::B3:
		return B3;
		break;
	case Board::C3:
		return A3;
		break;
	default:
		throw BadPos();
		break;
	}
}
const Board::Position Board::Dp(const Position p)
{
	switch (p)
	{
	case Board::A1:
		return A1;
		break;
	case Board::B1:
		return A2;
		break;
	case Board::C1:
		return A3;
		break;
	case Board::A2:
		return B1;
		break;
	case Board::B2:
		return B2;
		break;
	case Board::C2:
		return B3;
		break;
	case Board::A3:
		return C1;
		break;
	case Board::B3:
		return C2;
		break;
	case Board::C3:
		return C3;
		break;
	default:
		throw BadPos();
		break;
	}
}
const Board::Position Board::ADp(const Position p)
{
	switch (p)
	{
	case Board::A1:
		return C3;
		break;
	case Board::B1:
		return C2;
		break;
	case Board::C1:
		return C1;
		break;
	case Board::A2:
		return B3;
		break;
	case Board::B2:
		return B2;
		break;
	case Board::C2:
		return B1;
		break;
	case Board::A3:
		return A3;
		break;
	case Board::B3:
		return A2;
		break;
	case Board::C3:
		return A1;
		break;
	default:
		throw BadPos();
		break;
	}
}