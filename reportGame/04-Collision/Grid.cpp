#include "Grid.h"

Grid::Grid()
{

}

Grid::~Grid()
{

}

void Grid::initGird(int _cell_width, int _cell_height)
{
	this->cell_width = _cell_width/2;
	this->cell_height = _cell_height/2;
}

void Grid::changeVectorOb(vector<LPGAMEOBJECT>* coObjects)
{
	for (int i = 0; i < GRID_HEIGHT;i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			for (auto it = matrix[i][j].begin(); it != matrix[i][j].end(); )
			{
				coObjects->push_back(it->second);
			}
		}
	}
}


void Grid::AddOB(vector<LPGAMEOBJECT>* Objects)
{
	//duyet objects
	for (UINT i = 1; i < Objects->size(); i++)
	{
		int xCell =(int) Objects->at(i)->x / this->cell_width;
		int yCell =(int) Objects->at(i)->y / this->cell_height;
		
		//xu ly object cellA -> cellB, trung cell
		this->moveCells(Objects->at(i), xCell, yCell);	
		this->standardized(xCell, yCell);
		matrix[yCell][xCell].insert({ Objects->at(i)->GetID(),Objects->at(i) });
	}

}

void Grid::isSetCollision(vector<LPGAMEOBJECT>* coObjects, float xViewport, float yViewport)
{
	//kiem tra Object is Dead
	this->deleteObject();

	//lay dc objects of cell to collision
	int hereViewPort =(int) xViewport / this->cell_width; 
	for (int i = 0; i < GRID_HEIGHT;i++)
	{
		for (const auto& n : matrix[i][hereViewPort])
		{
			coObjects->push_back(n.second);
		}
	}
}

void Grid::deleteObject()
{
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			for (auto it = matrix[i][j].begin();it != matrix[i][j].end();)
			{
				if (it->second->GetDead() == true)
					it = matrix[i][j].erase(it);
				else
					++it;
			}
		}
	}
}

void Grid::standardized(int &_xcell, int &_ycell)
{
	if (_xcell > GRID_WIDTH - 1)
	{
		_xcell = GRID_WIDTH - 1;
	}
	else if (_xcell < 0)
	{
		_xcell = 0;
	}

	if (_ycell > GRID_HEIGHT - 1)
	{
		_ycell = GRID_HEIGHT - 1;
	}
	else if (_ycell < 0)
	{
		_ycell = 0;
	}
}

void Grid::moveCells(CGameObject *obMove, int _xcell, int _ycell)
{
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			auto search = matrix[i][j].find(obMove->GetID());
			if (search != matrix[i][j].end()) //tim thay doi tuong dang co trong cell khac
			{
				//xoa doi tuong do di
				for (auto it = matrix[i][j].begin();it != matrix[i][j].end();)
				{
					if (it->second->GetID() == obMove->GetID())
						it = matrix[i][j].erase(it);
					else
						++it;
				}
			}
		}
	}
}
