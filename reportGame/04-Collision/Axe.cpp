#include "Axe.h"
#include "tinyxml.h"

void Axe::LoadResource()
{
	//load resource
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_AXE, L"Resource\\sprites\\Sub_weapons\\AXE_ACTION.png", D3DCOLOR_XRGB(255, 0, 255));

	//sprites
	CSprites * sprites = CSprites::GetInstance();
	LPDIRECT3DTEXTURE9 texAXE = textures->Get(ID_TEX_AXE);

	//animations
	LPANIMATION ani;
	CAnimations * animations = CAnimations::GetInstance();

	//init XML
	TiXmlDocument doc("ResourceXML\\axe.xml");
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
	}
	int id;
	int idFrame;
	int left, top, right, bottom;
	TiXmlElement* anim = nullptr;

	//load XML candle
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
			sprites->Add(idFrame, left, top, right, bottom, texAXE);
			ani->Add(idFrame);
		}

		//load 1 frame finish
		animations->Add(id, ani);
		this->AddAnimation(id);
	}

}

void Axe::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!isDead)
	{
		l = x;
		t = y;
		r = l + AXE_BBOX_WIDTH;
		b = t + AXE_BBOX_HEIGHT;
	}
}

void Axe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT>* coObjectStatic, vector<LPGAMEOBJECT>* coObjectEnemy)
{
	CGameObject::Update(dt);
	vy += AXE_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents, coEnemy;
	vector<LPCOLLISIONEVENT> coEventsResult, coResultEnemy;

	coEvents.clear();
	coEnemy.clear();
	CalcPotentialCollisions(coObjectStatic, coEvents);
	CalcPotentialCollisions(coObjectEnemy, coEnemy);


	// No collision occured, proceed normally
	if (coEnemy.size() == 0 && coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEnemy, coResultEnemy, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coResultEnemy.size(); i++)
		{
			LPCOLLISIONEVENT e = coResultEnemy[i];
			if (dynamic_cast<Zombie *>(e->obj))
			{
				Zombie *_zom = dynamic_cast<Zombie *>(e->obj);
				//if (e->nx != 0)
				{
					_zom->SetState(ZOMBIE_DIE);
					_zom->SetDead(true);
					_zom->SetInvisible(true);
					_zom->is_Item();

					//Knife die
					this->SetDead(true);
				}

			}
			if (dynamic_cast<Merman *>(e->obj))
			{
				Merman *_zom = dynamic_cast<Merman *>(e->obj);
				//if (e->nx != 0)
				{
					_zom->SetState(MERMAN_DIE);
					_zom->SetDead(true);
					_zom->SetInvisible(true);
					_zom->is_Item();

					//Knife die
					this->SetDead(true);
				}

			}
			if (dynamic_cast<BatRed *>(e->obj))
			{
				BatRed *batred = dynamic_cast<BatRed *>(e->obj);
				//if (e->nx != 0)
				{
					batred->SetState(ZOMBIE_DIE);
					batred->SetDead(true);
					batred->SetInvisible(true);

					//Knife die
					this->SetDead(true);
				}
			}

			if (dynamic_cast<BossBat *>(e->obj))
			{
				//Knife die
				this->SetDead(true);
			}
		}
		
		//float min_tx, min_ty, nx = 0, ny;

		//FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		//// block 
		//x += min_tx * dx + nx * 0.1f;
		//y += min_ty * dy + ny * 0.1f;

		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;

		//for (UINT i = 0; i < coEventsResult.size(); i++)
		//{
		//	LPCOLLISIONEVENT e = coEventsResult[i];
		//	if (dynamic_cast<CandleSmall *>(e->obj))
		//	{
		//		CandleSmall *_canM = dynamic_cast<CandleSmall *>(e->obj);
		//		if (e->nx != 0)
		//		{
		//			_canM->SetState(CANDLESMALL_STATE_DIE);
		//			_canM->SetDead(true);
		//			_canM->SetInvisible(true);
		//			_canM->is_Item();
		//		}
		//	}

		//	if (dynamic_cast<CLimit *> (e->obj))
		//	{
		//		this->isDead = true;
		//		vy = 0;
		//		vx = 0;
		//	}
		//}
	}
	for (UINT i = 0; i < coEnemy.size(); i++) delete coEnemy[i];

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		//x += dx;
	}
	else if (coEvents.size() != 0)
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CandleSmall *>(e->obj))
			{
				CandleSmall *_canM = dynamic_cast<CandleSmall *>(e->obj);
				//if (e->nx != 0)
				{
					_canM->SetState(CANDLESMALL_STATE_DIE);
					_canM->SetDead(true);
					_canM->SetInvisible(true);
					_canM->is_Item();

					//Knife die
					this->SetDead(true);
				}

			}

			if (dynamic_cast<Candle *>(e->obj))
			{
				Candle *_can = dynamic_cast<Candle *>(e->obj);
				//if (e->nx != 0)
				{
					_can->SetState(CANDLE_STATE_DIE);
					_can->SetDead(true);
					_can->SetInvisible(true);
					_can->is_Item();

					//Knife die
					this->SetDead(true);
				}
			}
			if (dynamic_cast<CLimit *> (e->obj))
			{
				this->isDead = true;
				vy = 0;
				vx = 0;
			}

		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (this->y > 400)
	{
		vy = 0;
		vx = 0;
		this->isDead = true;
	}
}

void Axe::Render(float xViewport, float yViewport)
{
	this->X_view = x - xViewport;
	this->Y_view = y - yViewport;
	int alpha = 255;

	//this->SetState(this->state);

	//animation axe left or right
	this->isLeft = nx > 0 ? false : true;

	//choose animation axe stand or sit
	this->ani = isStand ? 0 : 1;
	if (!isDead)
	{
		animations[ani]->Render(x - xViewport, y - yViewport, alpha, isLeft);
		//Render boundingBox at end frame of currentAnimation
		RenderBoundingBox();
	}
	

}

void Axe::SetCurrentPosSimon(float _xSimon, float _ySimon, int _nx, bool _isStand)
{
	this->x = _xSimon + 20.0f;
	this->y = _ySimon + 15.0f;
	this->nx = _nx;
	this->isStand = _isStand;
	this->AxeFly();
	this->SetDead(false);
}

void Axe::SetState(int state)
{
	CGameObject::SetState(state);

}
