#pragma once
#include <vector>
#include "GameObject.h"
#include <unordered_map>
using namespace std;

#define CELLS_GRID  4
#define MAPWIDTH	640
#define MAPHEIGHT	480
#define GRID_WIDTH	2
#define GRID_HEIGHT 2

class Grid
{
	int cell_width;
	int cell_height;
	int cells = CELLS_GRID;
	unordered_map<int, LPGAMEOBJECT> matrix[GRID_HEIGHT][GRID_WIDTH];

public:

	Grid();
	~Grid();

	void initGird(int _cell_width, int _cell_height);
	void changeVectorOb(vector<LPGAMEOBJECT> *coObjects);
	void AddOB(vector<LPGAMEOBJECT> *Objects);
	void isSetCollision(vector<LPGAMEOBJECT> *coObjects, float xViewport, float yViewport);
	void deleteObject();

	//chuyen doi tuong da co sang cell khac phu hop
	void standardized(int &_xcell, int &_ycell);
	void moveCells(CGameObject *obMove, int _xcell, int _ycell);
};