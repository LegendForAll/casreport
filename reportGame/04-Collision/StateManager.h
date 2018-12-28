#pragma once
#include "Game.h"
#include "GameState.h"
#include "StateMenu.h"
#include "StateIntro.h"
#include "StateOne.h"
#include "StateTwo.h"
#include "StateThree.h"


class MState 
{
	CGame *gameText;
	CGameState *gamestate;
	int stateID;
	bool Change = true;
	float pointSimonX, pointSimonY;
	int ASpointsimon = -1;

public:

	MState();
	~MState();

	void LoadState(int _stateID);
	void NextState();
	void Render(LPDIRECT3DDEVICE9 d3ddv, LPDIRECT3DSURFACE9 bb, LPD3DXSPRITE spriteHandler);
	void Update(DWORD dt);
	void ChangeState(DWORD dt);
	void SetChangeGameState();

	void SetStateID(int _IDstate) { this->stateID = _IDstate; }
	int GetStateID() { return this->stateID; }

	void setChangemap(bool _change) { this->Change = _change; }

	void setTextGame(CGame *_game);

};
