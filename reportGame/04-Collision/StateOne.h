#pragma once
#include "GameState.h"

class StateOne :public CGameState
{
	CTextures *textures;
	CEffect *ef;
	//UI *uigame;
public:

	StateOne();
	~StateOne();

	void initText(LPDIRECT3DDEVICE9 d3ddv);

	virtual void InitState(LPDIRECT3DDEVICE9 dv, LPDIRECT3DSURFACE9 bb);
	virtual void LoadState(int AS);
	virtual void UpdateState(DWORD dt);
	virtual void RenderState(LPDIRECT3DDEVICE9 d3ddv, LPDIRECT3DSURFACE9 bb, LPD3DXSPRITE spriteHandler);
};