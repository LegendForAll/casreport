#include "BrickCell.h"

BrickCell::BrickCell()
{

}

BrickCell::~BrickCell()
{

}

void BrickCell::is_Item()
{
	//whip collision vs candlesmall
	_item->SetPosition(this->x, this->y);
	_item->SetInvisible(false);

	//update y = x for equation sin
	_item->Setcurrent_Oy(this->x);
}

void BrickCell::not_Item()
{
	//simon eat item
	_item->SetDead(true);
}

void BrickCell::LoadResource()
{
	//Load Item
	this->_item = new Items();
	this->_item->LoadResource();
	//this->_item->SetState(I_HEART_BIG);
	this->_item->SetInvisible(true);

	//load resource
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_GROUND, L"Resource\\sprites\\Ground\\Brick.png", D3DCOLOR_XRGB(176, 224, 248));

	//sprite
	CSprites * sprites = CSprites::GetInstance();
	LPDIRECT3DTEXTURE9 texGround = textures->Get(ID_TEX_GROUND);

	//animations
	LPANIMATION ani;
	CAnimations * animations = CAnimations::GetInstance();

	sprites->Add(20002, 0, 0, 32, 32, texGround);

	ani = new CAnimation(100);		// ground
	ani->Add(20002);
	animations->Add(602, ani);

	this->AddAnimation(602);
}

void BrickCell::Render(float xViewport, float yViewport)
{
	this->X_view = x - xViewport;
	this->Y_view = y - yViewport;

	if (!isDead) //Kiem tra object co ton tai tren game sau khi va cham
	{
		animations[0]->Render(x - xViewport, y - yViewport);
		//RenderBoundingBox();
	}
}

void BrickCell::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + GROUND_BBOX_WIDTH;
	b = y + GROUND_BBOX_HEIGHT;
}

void BrickCell::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT>* coObjectStatic, vector<LPGAMEOBJECT>* coObjectEnemy)
{

}
