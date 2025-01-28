#pragma once

const int MAX_HGF = 1000;

class Cell
{
private:
	int row, col;
	Cell* parent;

	double H;
	double G;
	double F;
public:
	Cell();
	Cell(int r, int c, Cell* p);
	int getRow() { return row; }
	int getCol() { return col; }
	Cell* getParent() { return parent; }
	double getH() { return H; }
	void setH(double h) { H = h; }
	double getG() { return G; }
	void setG(double g) { G = g; }
	double getF() { return F; }
	void setF(double f) { F = f; }
	
};

