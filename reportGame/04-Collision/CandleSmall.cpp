#include "CandleSmall.h"
#include "tinyxml.h"

CandleSmall::CandleSmall()
{

}

CandleSmall::~CandleSmall()
{

}

void CandleSmall::is_Item()
{
	//whip collision vs candlesmall
	_item->SetPosition(this->x, this->y);
	_item->SetInvisible(false);

	//update y = x for equation sin
	_item->Setcurrent_Oy(this->x);
}

void CandleSmall::not_Item()
{
	//simon eat item
	_item->SetDead(true);
}

void CandleSmall::LoadResource()
{
	//Load Item
	this->_item = new Items();
	this->_item->LoadResource();
	//this->_item->SetState(I_HEART_BIG);
	this->_item->SetInvisible(true);
	this->_item->Set_epuationFly(EQUATION_SIN);

	//load resource candle
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_CANDLE_SMALL, L"Resource\\sprites\\Ground\\1.png", D3DCOLOR_XRGB(255, 0, 255));

	//sprite
	CSprites * sprites = CSprites::GetInstance();
	LPDIRECT3DTEXTURE9 texCanSmall = textures->Get(ID_TEX_CANDLE_SMALL);

	//animations
	LPANIMATION ani;
	CAnimations * animations = CAnimations::GetInstance();

	//init XML
	TiXmlDocument doc("ResourceXML\\candlesmall.xml");
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
	}
	int id;
	int idFrame;
	int left, top, right, bottom;
	TiXmlElement* anim = nullptr;

	//load XML candlesmall
	TiXmlElement* mani = doc.RootElement();
	anim = mani->FirstChildElement("animation");
	for (anim; anim != NULL; anim = anim->NextSiblingElement())
	{
		ani = new CAnimation(100);
		anim->QueryIntAttribute("id", &id);

		for (TiXmlElement* a = anim->FirstChildElement(); a != NULL; a = a->NextSiblingElement())
		{
			a->QueryIntAttribute("id", &idFrame);
			a->QueryIntAttribute("left", &left);
			a->QueryIntAttribute("top", &top);
			a->QueryIntAttribute("right", &right);
			a->QueryIntAttribute("bottom", &bottom);

			//add frame to sprite
			sprites->Add(idFrame, left, top, right, bottom, texCanSmall);
			ani->Add(idFrame);
		}

		//load 1 frame finish
		animations->Add(id, ani);
		this->AddAnimation(id);
	}
}

void CandleSmall::Render(float xViewport, float yViewport)
{
	this->X_view = x - xViewport;
	this->Y_view = y - yViewport;

	switch (this->state)
	{
		case CANDLESMALL_STATE_NO:
		{
			if (!(isDead && isvisible))
			{
				ani = CANDLESMALL_STATE_NO;
				animations[ani]->Render(x - xViewport, y - yViewport);
				break;
			}
			break;
		}

		case CANDLESMALL_STATE_DIE:
		{
			if (isDead)
			{
				ani = CANDLESMALL_STATE_DIE;
				animations[ani]->Render(x - xViewport, y - yViewport);

				if (animations[CANDLESMALL_STATE_DIE]->CheckDoAllFrame())
				{
					this->SetState(CANDLESMALL_STATE_NO);
					animations[CANDLESMALL_STATE_DIE]->SetDoAllFrame(false);
				}
				break;
			}
			break;
		}
	}
}

void CandleSmall::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!(isDead && isvisible))
	{
		l = x;
		t = y;
		r = l + CANDLESMALL_BBOX_WIDTH;
		b = t + CANDLESMALL_BBOX_HEIGHT;
	}
}

void CandleSmall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT>* coObjectStatic, vector<LPGAMEOBJECT>* coObjectEnemy)
{

}
