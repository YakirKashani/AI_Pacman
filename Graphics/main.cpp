
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <queue>
#include <vector>
#include "Cell.h"
#include <iostream>
#include "Definitions.h"
#include "Node.h"
#include "Edge.h"
#include "CompareNodes.h"
#include "CompareCells.h"
#include "Player.h"
#include "PacmanBoardCell.h"

using namespace std;

const int WIDTH = 900;
const int HEIGHT = 900;
const int MSZ = 20; // maze-size


priority_queue<Cell*, vector<Cell*>, CompareCells> GhostAStarPQ[NUM_GHOSTS];

PacmanBoardCell maze[MSZ][MSZ];

bool runBFS = false;
bool runDFS = false;
bool runBestFirstSearch = false;
bool runAStar = false;
bool runPacman = false;

bool gameOn = false;

const int PACMAN_SEARCH_DEPTH = 5;

vector<Cell*> dfs_grays;

queue <pair<Cell*,int>> PacmanGrays;

int startRow, startCol;
int targetRow, targetCol;

Player* pacman;
Player* ghosts[NUM_GHOSTS];

void SetupMaze()
{
	int i, j;
	for (i = 0;i < MSZ;i++) { // outer margins
		maze[0][i].setBoardInt(WALL); // first line
		maze[MSZ - 1][i].setBoardInt(WALL); // last line
		maze[i][0].setBoardInt(WALL); // left column
		maze[i][MSZ - 1].setBoardInt(WALL); // right column
	}

	for (i = 1;i < MSZ - 1;i++) { //inner space
		for (j = 1;j < MSZ - 1;j++) {
			if (i % 2 == 1) { //mostly spaces
				if (rand() % 10 > 0) { // 90%
					maze[i][j].setBoardInt(SPACE);
					maze[i][j].setIsCoin(true);
				}
				else // 10%
					maze[i][j].setBoardInt(WALL);
			}
			else { //mostly walls
				if (rand() % 10 > 5) { //40%
					maze[i][j].setBoardInt(WALL);
				}
				else{// 60%
					maze[i][j].setBoardInt(SPACE);
					maze[i][j].setIsCoin(true);
				}
			}
		}
	}
	pacman = new Player(MSZ / 2, MSZ / 2);
	maze[pacman->getRow()][pacman->getCol()].setBoardInt(PACMAN); // Middle point
	int row, col;
	for (i = 0;i < NUM_GHOSTS;i++) {
		do {
			row = rand() % MSZ;
			col = rand() % MSZ;
		} while (row == 0 || col == 0 || row== MSZ-1 || col == MSZ-1 || row == MSZ / 2 || col == MSZ / 2 || maze[row][col].getBoardInt() != SPACE);
		ghosts[i] = new Player(row, col);
		maze[ghosts[i]->getRow()][ghosts[i]->getCol()].setBoardInt(GHOST); // target point - random
	}
}

double Distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void init()
{
	glClearColor(0.5,0.5,0.5,0);// color of window background
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	srand(time(0));

	SetupMaze();
}

void ShowMaze() {
	int i, j;
	for (i = 0;i < MSZ;i++) {
		for (j = 0;j < MSZ;j++) {
			// 1. set color of cell
			switch (maze[i][j].getBoardInt())
			{
			case SPACE:
				if (maze[i][j].getIsCoin())
					glColor3d(1, 1, 0.5); //yellow
				else
					glColor3d(1, 1, 1); //white
				break;
			case WALL:
				glColor3d(0, 0, 0);//black
				break;
			case PACMAN:
				glColor3d(0.6, 0.6, 1);//light-blue
				break;
			case GHOST:
				glColor3d(1, 0, 0);//red
				break;
			}
			//show cell
			glBegin(GL_POLYGON);
			glVertex2d(j, i);
			glVertex2d(j, i + 1);
			glVertex2d(j + 1, i + 1);
			glVertex2d(j + 1, i);
			glEnd();
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	ShowMaze();
	glutSwapBuffers(); // show all
}

int RestoreGhostPath(Cell* pc, int ghostNum) { //BUG HERE!

	if (pc->getParent() == nullptr)
		return -1;

	while (pc->getParent()->getParent() != nullptr) {
		maze[pc->getRow()][pc->getCol()].setGhostSign(ghostNum, PATH);
		pc = pc->getParent();
	}

	maze[pc->getRow()][pc->getCol()].setGhostSign(ghostNum, PATH);
	if (pc->getRow() - pc->getParent()->getRow() == 1) //UP
		return UP;
	if (pc->getRow() - pc->getParent()->getRow() == -1) //DOWN
		return DOWN;
	if (pc->getCol() - pc->getParent()->getCol() == 1) //RIGHT
		return RIGHT;
	if (pc->getCol() - pc->getParent()->getCol() == -1) // LEFT
		return LEFT;
}

bool CheckForCollision() {
	int pacmanRow = pacman->getRow();
	int pacmanCol = pacman->getCol();
	for (int i = 0;i < NUM_GHOSTS;i++)
	{
		if (pacmanRow == ghosts[i]->getRow() && abs(pacmanCol - ghosts[i]->getCol()) == 1 ||
			pacmanCol == ghosts[i]->getCol() && abs(pacmanRow - ghosts[i]->getRow()) == 1)
			 {
			gameOn = false;
			cout << "Pacman lost" << endl;
			return false;
		}
	}
	return true;
}

void MoveGhostToPacman(int direction, int ghostNum) {
	if (direction != -1) {
		maze[ghosts[ghostNum]->getRow()][ghosts[ghostNum]->getCol()].setBoardInt(SPACE);
		if (direction == UP)
		{
			ghosts[ghostNum]->setRow(ghosts[ghostNum]->getRow() + 1);
			maze[ghosts[ghostNum]->getRow()][ghosts[ghostNum]->getCol()].setBoardInt(GHOST);
		}
		else if (direction == DOWN)
		{
			ghosts[ghostNum]->setRow(ghosts[ghostNum]->getRow() - 1);
			maze[ghosts[ghostNum]->getRow()][ghosts[ghostNum]->getCol()].setBoardInt(GHOST);
		}
		else if (direction == LEFT)
		{
			ghosts[ghostNum]->setCol(ghosts[ghostNum]->getCol() - 1);
			maze[ghosts[ghostNum]->getRow()][ghosts[ghostNum]->getCol()].setBoardInt(GHOST);
		}
		else if (direction == RIGHT)
		{
			ghosts[ghostNum]->setCol(ghosts[ghostNum]->getCol() + 1);
			maze[ghosts[ghostNum]->getRow()][ghosts[ghostNum]->getCol()].setBoardInt(GHOST);
		}
	}
}

bool CheckNeighborAStar(int row, int col, Cell* pCurrent, int ghostNum) {
	if (maze[row][col].getBoardInt() == PACMAN) {
		int direction = RestoreGhostPath(pCurrent, ghostNum);
		MoveGhostToPacman(direction, ghostNum);
		return false;
	}
	else {
		Cell* neighbor = new Cell(row, col, pCurrent);
		neighbor->setG(Distance(row, col, ghosts[ghostNum]->getRow(), ghosts[ghostNum]->getCol()));
		neighbor->setH(Distance(row, col, pacman->getRow(), pacman->getCol()));
		neighbor->setF(neighbor->getG() + neighbor->getH()); // F = G + H

		maze[row][col].setGhostSign(ghostNum,GRAY);
		GhostAStarPQ[ghostNum].push(neighbor);
		return true;
	}
}

void RunGhostAStar(int GhostNumber) {
	Cell* pCurrent;
	int row, col;
	bool go_on = true;

	if (GhostAStarPQ[GhostNumber].empty()) {
		runAStar = false;
		cout << "No solution. Priority queue is empty.\n";
		return;
	}
	else {
		while (go_on && !GhostAStarPQ[GhostNumber].empty()) {
			pCurrent = GhostAStarPQ[GhostNumber].top();
			GhostAStarPQ[GhostNumber].pop();
			row = pCurrent->getRow();
			col = pCurrent->getCol();
			if (maze[row][col].getBoardInt() != GHOST)
				maze[row][col].setGhostSign(GhostNumber, BLACK);
			if (maze[row + 1][col].getGhostSign(GhostNumber) == WHITE && (maze[row + 1][col].getBoardInt() == SPACE || maze[row + 1][col].getBoardInt() == PACMAN)) {
				go_on = CheckNeighborAStar(row + 1, col, pCurrent, GhostNumber);
			}
			if (go_on && maze[row - 1][col].getGhostSign(GhostNumber) == WHITE && (maze[row - 1][col].getBoardInt() == SPACE || maze[row - 1][col].getBoardInt() == PACMAN)) {
				go_on = CheckNeighborAStar(row - 1, col, pCurrent, GhostNumber);
			}
			if (go_on && maze[row][col - 1].getGhostSign(GhostNumber) == WHITE && (maze[row][col - 1].getBoardInt() == SPACE || maze[row][col - 1].getBoardInt() == PACMAN)) {
				go_on = CheckNeighborAStar(row, col - 1, pCurrent, GhostNumber);
			}
			if (go_on && maze[row][col + 1].getGhostSign(GhostNumber) == WHITE && (maze[row][col + 1].getBoardInt() == SPACE || maze[row][col + 1].getBoardInt() == PACMAN)) {
				go_on = CheckNeighborAStar(row, col + 1, pCurrent, GhostNumber);
			}
		}
	}
}

void cleanAllPacmanSigns() {
	for (int i = 0;i < MSZ;i++) {
		for (int j = 0;j < MSZ;j++) {
			maze[i][j].setPacmanSign(SPACE);
		}
	}
}

void burnOption(Cell* pc) {
	while (pc->getParent() != nullptr)
	{
		maze[pc->getRow()][pc->getCol()].setPacmanSign(GHOST);
		pc = pc->getParent();
	}
}

void restorePcamanPath(Cell* pc) {
	while (pc->getParent() != nullptr)
	{
		maze[pc->getRow()][pc->getCol()].setPacmanSign(PACMAN_PATH);
		pc = pc->getParent();
	}
}

bool checkPacmanPath(Cell* pc) {
	while (pc->getParent() != nullptr)
	{
		if (maze[pc->getRow()][pc->getCol()].getBoardInt() == GHOST)
			return false;
		pc = pc->getParent();
	}
	return true;
}

bool PacmanBFSCheckNeighbor(int row, int col, Cell* pCurrent, queue<Cell*>(&ghostsFound), Cell* (&closestCoin), int currentDepth) {
	Cell* pc = new Cell(row, col, pCurrent);
	if (maze[row][col].getBoardInt() == GHOST) {
		ghostsFound.push(pc);
		burnOption(pc);
		return false;
	}
	else if (maze[row][col].getIsCoin()) {
		if (closestCoin == nullptr)
			closestCoin = pc;
		maze[row][col].setPacmanSign(PACMAN_GRAY);
		PacmanGrays.push(make_pair(pc, currentDepth + 1));
	}
	else {
		maze[row][col].setPacmanSign(PACMAN_GRAY);
		PacmanGrays.push(make_pair(pc, currentDepth + 1));
	}
	return true;
}

void RunPacmanBFS(queue<Cell*>(&ghostsFound), Cell* (&closestCoin)) {
	Cell* pCurrent;
	int row, col;
	bool go_on = true;
	if (PacmanGrays.empty()) {
		runBestFirstSearch = false;
		cout << "There is no solution. Priority queue is empty.\n";
		return;
	}
	else {
		while (!PacmanGrays.empty()) {
			auto currentPair = PacmanGrays.front();
			PacmanGrays.pop();
			pCurrent = currentPair.first;
			int currentDepth = currentPair.second;

			if (currentDepth >= PACMAN_SEARCH_DEPTH) // stop processing if we've reached the maximum depth
				break;
			// 1. paint pCurrent black
			row = pCurrent->getRow();
			col = pCurrent->getCol();
			if (maze[row][col].getBoardInt() != PACMAN)
				maze[row][col].setPacmanSign(PACMAN_BLACK);
			// 2. check all the neighbors of pCurrent
			// go up
			if (maze[row + 1][col].getPacmanSign() == SPACE && (maze[row + 1][col].getIsCoin() || maze[row + 1][col].getBoardInt() == GHOST || maze[row + 1][col].getBoardInt() == SPACE)) {
				PacmanBFSCheckNeighbor(row + 1, col, pCurrent, ghostsFound, closestCoin, currentDepth);
			}
			// down
			if (maze[row - 1][col].getPacmanSign() == SPACE && (maze[row - 1][col].getIsCoin() || maze[row - 1][col].getBoardInt() == GHOST || maze[row - 1][col].getBoardInt() == SPACE)) {
				PacmanBFSCheckNeighbor(row - 1, col, pCurrent, ghostsFound, closestCoin, currentDepth);
			}
			// left
			if (maze[row][col - 1].getPacmanSign() == SPACE && (maze[row][col - 1].getIsCoin() || maze[row][col - 1].getBoardInt() == GHOST || maze[row][col - 1].getBoardInt() == SPACE)) {
				PacmanBFSCheckNeighbor(row, col - 1, pCurrent, ghostsFound, closestCoin, currentDepth);
			}
			// right
			if (maze[row][col + 1].getPacmanSign() == SPACE && (maze[row][col + 1].getIsCoin() || maze[row][col + 1].getBoardInt() == GHOST || maze[row][col + 1].getBoardInt() == SPACE)) {
				PacmanBFSCheckNeighbor(row, col + 1, pCurrent, ghostsFound, closestCoin, currentDepth);
			}
		}
	}
}

void movePacman(int direction) {
	if (maze[pacman->getRow()][pacman->getCol()].getIsCoin())
		maze[pacman->getRow()][pacman->getCol()].setIsCoin(false);
	maze[pacman->getRow()][pacman->getCol()].setBoardInt(SPACE);
	if (direction == UP) {
		pacman->setRow(pacman->getRow() + 1);
		maze[pacman->getRow()][pacman->getCol()].setBoardInt(PACMAN);
	}
	else if (direction == DOWN) {
		pacman->setRow(pacman->getRow() - 1);
		maze[pacman->getRow()][pacman->getCol()].setBoardInt(PACMAN);
	}
	else if (direction == LEFT) {
		pacman->setCol(pacman->getCol() -1);
		maze[pacman->getRow()][pacman->getCol()].setBoardInt(PACMAN);
	}
	else if (direction == RIGHT) {
		pacman->setCol(pacman->getCol() + 1);
		maze[pacman->getRow()][pacman->getCol()].setBoardInt(PACMAN);
	}
}

bool isWalkable(int direction) {
	if (direction == 0) //UP
		if (maze[pacman->getRow() + 1][pacman->getCol()].getBoardInt() == WALL || maze[pacman->getRow() + 1][pacman->getCol()].getPacmanSign() == GHOST)
			return false;
	if (direction == 1) // DOWN
		if (maze[pacman->getRow() - 1][pacman->getCol()].getBoardInt() == WALL || maze[pacman->getRow() - 1][pacman->getCol()].getPacmanSign() == GHOST)
			return false;
	if (direction == 2) // LEFT
		if (maze[pacman->getRow()][pacman->getCol() - 1].getBoardInt() == WALL || maze[pacman->getRow()][pacman->getCol() - 1].getPacmanSign() == GHOST)
			return false;
	if (direction == 3) // RIGHT
		if (maze[pacman->getRow()][pacman->getCol() + 1].getBoardInt() == WALL || maze[pacman->getRow()][pacman->getCol() + 1].getPacmanSign() == GHOST)
			return false;
	return true;
}

void pacmanDecide(queue<Cell*>(&ghostsFound), Cell* (&closestCoin)) {
	// if ghosts not empty, find the farest point from them
	if (!ghostsFound.empty()) {
		if (isWalkable(UP))
			movePacman(UP);
		else if (isWalkable(DOWN))
			movePacman(DOWN);
		else if (isWalkable(RIGHT))
			movePacman(RIGHT);
		else if (isWalkable(LEFT))
			movePacman(LEFT);
	}
	// if ghost empty, collect the closest coin if exist
	else if (closestCoin != nullptr) {
		restorePcamanPath(closestCoin);
		if (maze[pacman->getRow() + 1][pacman->getCol()].getPacmanSign() == PACMAN_PATH) // Check if UP
			movePacman(UP);
		else if (maze[pacman->getRow() - 1][pacman->getCol()].getPacmanSign() == PACMAN_PATH) // Check if DOWN
			movePacman(DOWN);
		else if (maze[pacman->getRow()][pacman->getCol() + 1].getPacmanSign() == PACMAN_PATH) // Check if RIGHT
			movePacman(RIGHT);
		else if (maze[pacman->getRow()][pacman->getCol() - 1].getPacmanSign() == PACMAN_PATH) // Check if LEFT
			movePacman(LEFT);
	}
	// if no coin close, go random
	else {
		int randomDirection;
		do {
			randomDirection = rand() % 4;
		} while (!isWalkable(randomDirection));
		movePacman(randomDirection);
	}
}

void CleanAllGhostSigns(int GhostNumber) {
	for (int i = 0;i < MSZ;i++) {
		for (int j = 0;j < MSZ;j++) {
			maze[i][j].setGhostSign(GhostNumber,WHITE);
		}
	}
}

void idle() // always running
{
	if (gameOn && CheckForCollision()) {		
		Cell* pc = new Cell(pacman->getRow(), pacman->getCol(), nullptr);
		while (!PacmanGrays.empty())
			PacmanGrays.pop();
		PacmanGrays.push(make_pair(pc, 0));
		queue <Cell*> ghostsFound;
		Cell* closestCoin = nullptr;
		cleanAllPacmanSigns();
		RunPacmanBFS(ghostsFound, closestCoin); // move pacman
		pacmanDecide(ghostsFound, closestCoin);
		for (int i = 0;i < NUM_GHOSTS;i++) {
			CleanAllGhostSigns(i);
			while (!GhostAStarPQ[i].empty())
				GhostAStarPQ[i].pop();
			Cell* ghostCell = new Cell(ghosts[i]->getRow(), ghosts[i]->getCol(), nullptr);
			GhostAStarPQ[i].push(ghostCell);
			RunGhostAStar(i);
		}
		Sleep(500);
	}
	glutPostRedisplay(); // indirect call to display
}

void menu(int choice) {
	
	switch (choice)
	{
	case 1: //PACMAN
		glutDisplayFunc(display);
		gameOn = true;
		break;
	}
}

void main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	// definitions for visual memory (Frame buffer) and double buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("BFS - DFS");
	// display is a refresh function
	glutDisplayFunc(display);
	// idle is a update function
	glutIdleFunc(idle);

	//add menu
	glutCreateMenu(menu);
	glutAddMenuEntry("PACMAN", 1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}
