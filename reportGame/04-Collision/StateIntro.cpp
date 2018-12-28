#include "StateIntro.h"

StateIntro::StateIntro()
{

}

StateIntro::~StateIntro()
{

}

void StateIntro::InitState(LPDIRECT3DDEVICE9 dv, LPDIRECT3DSURFACE9 bb)
{

}

void StateIntro::LoadState(int AS)
{
	CGameState::LoadState(AS);

	simon->SetPosition(650.0f, 300.0f);
	simon->SetState(SM_GO_INTRO);
	isInitSimon = true;

	AniUI *bat = new AniUI();
	bat->LoadResource();
	bat->SetState(MI_BAT_INTRO);
	bat->SetPosition(260, 80);
	objecttsStatic.push_back(bat);

	AniUI *sky = new AniUI();
	sky->LoadResource();
	sky->SetState(MI_SKY_INTRO);
	sky->SetPosition(443, 85);
	objecttsStatic.push_back(sky);

	//init ground
	CLimit *limG = new CLimit();
	limG->SetwidthBBox(1.6f);
	limG->SetPosition(0.0f, 412.0f);
	limG->SetState(LIMIT_GROUND);
	objects.push_back(limG);
}

void StateIntro::UpdateState(DWORD dt)
{
	CGameState::UpdateState(dt);
}

void StateIntro::RenderState(LPDIRECT3DDEVICE9 d3ddv, LPDIRECT3DSURFACE9 bb, LPD3DXSPRITE spriteHandler)
{
	tilemap->UpdateScrollPosition();
	CGameState::RenderState(d3ddv, bb, spriteHandler);
}
