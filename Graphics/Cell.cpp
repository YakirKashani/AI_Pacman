#include "Cell.h"

Cell::Cell()
{
	parent = nullptr;
	G = MAX_HGF;
	H = MAX_HGF;
	F = MAX_HGF;
}

Cell::Cell(int r, int c, Cell* p)
{
	row = r;
	col = c;
	parent = p;
	G = MAX_HGF;
	H = MAX_HGF;
	F = MAX_HGF;
}
