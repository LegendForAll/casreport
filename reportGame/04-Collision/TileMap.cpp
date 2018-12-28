#include "TileMap.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

TileMap::TileMap()
{

}

TileMap::TileMap(LPCWSTR filename)
{
	//load resource candle
	textures = CTextures::GetInstance();
	textures->Add(ID_TEX_MAPSET, filename, D3DCOLOR_XRGB(255, 0, 255));

}


TileMap::~TileMap()
{

}


void TileMap::LoadMapCSV(LPCWSTR filename)
{
	ifstream ip(filename);

	if (!ip.is_open())
	{
		return;
	}

	string tilenum;
	int temp;
	RECT r1;

	for (int i = 0;i < MAPHEIGHT; i++)
	{
		for (int j = 0; j < MAPWIDTH - 1;j++)
		{
			getline(ip, tilenum, ',');
			temp = stoi(tilenum);

			//readine
			r1.left = (temp % MAPWIDTH) * TILEWIDTH;
			r1.top = (temp / MAPWIDTH) * TILEHEIGHT;
			r1.right = r1.left + TILEWIDTH;
			r1.bottom = r1.top + TILEHEIGHT;

			matrix[i][j] = r1;

		}

		//read endline
		getline(ip, tilenum, '\n');
		temp = stoi(tilenum);

		r1.left = (temp % MAPWIDTH) * TILEWIDTH;
		r1.top = (temp / MAPWIDTH) * TILEHEIGHT;
		r1.right = r1.left + TILEWIDTH;
		r1.bottom = r1.top + TILEHEIGHT;

		matrix[i][MAPWIDTH - 1] = r1;
	}

	ip.close();
}

void TileMap::Render(LPD3DXSPRITE spriteHandler)
{

	float colStart = ViewPortX / TILEWIDTH;
	float colEnd = (ViewPortX / TILEWIDTH + SCREEN_WIDTH / TILEWIDTH + 1) > MAPWIDTH ? MAPWIDTH : (ViewPortX / TILEWIDTH + SCREEN_WIDTH / TILEWIDTH + 1);

	D3DXVECTOR3 pos(0, 0, 0);

	LPDIRECT3DTEXTURE9 texBG = textures->Get(ID_TEX_MAPSET);

	for (int y = 0; y < MAPHEIGHT; y++)
	{
		for (int x = (int)colStart; x < colEnd; x++)
		{
			pos.x = (int)(x * TILEWIDTH - ViewPortX);
			pos.y = (int)((y * TILEHEIGHT) - ViewPortY);
			pos.z = 0;
			spriteHandler->Draw(texBG, &matrix[y][x], NULL, &pos, D3DCOLOR_XRGB(255, 255, 255));
		}
	}
}

void TileMap::UpdateScrollPosition()
{
	const int GAMEWORLDWIDTH = TILEWIDTH * MAPWIDTH;
	const int GAMEWORLDHEIGHT = TILEHEIGHT * MAPHEIGHT;


	//update horizontal scrolling position and speed
	if (ViewPortX < 0)
	{
		ViewPortX = 0;
		SpeedX = 0;
	}
	else if (ViewPortX > GAMEWORLDWIDTH - WINDOWWIDTH)
	{
		ViewPortX = GAMEWORLDWIDTH - WINDOWWIDTH;
		SpeedX = 0;
	}

	//update vertical scrolling position and speed
	if (ViewPortY < 0)
	{
		ViewPortY = 0;
		SpeedY = 0;
	}
	else if (ViewPortY > GAMEWORLDHEIGHT)
	{
		ViewPortY = GAMEWORLDHEIGHT;
		SpeedY = 0;
	}
}

void TileMap::SetSettingMap(int _MAPWIDTH, int _MAPHEIGHT)
{
	this->MAPWIDTH = _MAPWIDTH;
	this->MAPHEIGHT = _MAPHEIGHT;

	matrix = new RECT*[this->MAPHEIGHT];
	for (int i = 0; i < this->MAPHEIGHT; i++)
		matrix[i] = new RECT[this->MAPWIDTH];

}
