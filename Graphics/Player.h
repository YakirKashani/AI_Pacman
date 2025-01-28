#pragma once
class Player
{
private:
	int row, col;
public:
	Player();
	Player(int r, int c);
	void setRow(int r) { row = r; }
	void setCol(int c) { col = c; }
	int getRow() { return row; }
	int getCol() { return col; }
};

