#pragma once
#include "Definitions.h"
class PacmanBoardCell
{
private:
	int PacmanSign;
	int GhostSign[NUM_GHOSTS];
	int boardInt;
	int IsCoin;
public:
	PacmanBoardCell();
	int getPacmanSign() { return PacmanSign; }
	int getGhostSign(int ghostNum) { return GhostSign[ghostNum]; }
	int getBoardInt() { return boardInt; }
	int getIsCoin() { return IsCoin; }
	void setPacmanSign(int sign) { PacmanSign = sign; }
	void setGhostSign(int ghost, int sign) { GhostSign[ghost] = sign; }
	void setBoardInt(int sign) { boardInt = sign; }
	void setIsCoin(bool isCoin) { IsCoin = isCoin; }
};

