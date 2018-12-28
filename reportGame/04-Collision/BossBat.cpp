#include "BossBat.h"
#include "tinyxml.h"
#include "Axe.h"
#include "Game.h"
#include <ctime>


void BossBat::LoadResource()
{
	//Load Item
	this->item = new Items();
	this->item->LoadResource();
	this->item->SetState(I_BOSS_BAT);
	this->item->SetInvisible(true);

	//load resource zombie
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_BOSSBAT, L"Resource\\sprites\\Bosses\\VAMPIRE_BAT.png", D3DCOLOR_XRGB(255, 0, 255));

	//sprite
	CSprites * sprites = CSprites::GetInstance();
	LPDIRECT3DTEXTURE9 texBoss = textures->Get(ID_TEX_BOSSBAT);

	//animations
	LPANIMATION ani;
	CAnimations * animations = CAnimations::GetInstance();

	//init XML
	TiXmlDocument doc("ResourceXML\\bossbat.xml");
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
			sprites->Add(idFrame, left, top, right, bottom, texBoss);
			ani->Add(idFrame);
		}

		//load 1 frame finish
		animations->Add(id, ani);
		this->AddAnimation(id);
	}

	//Vector speed random
	vRandom.push_back(BOSSBAT_SPEED_RANDOM_1);
	vRandom.push_back(BOSSBAT_SPEED_RANDOM_2);
	vRandom.push_back(BOSSBAT_SPEED_RANDOM_3);
	vRandom.push_back(BOSSBAT_SPEED_RANDOM_4);
}

void BossBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT>* coObjectStatic, vector<LPGAMEOBJECT>* coObjectEnemy)
{
	CGameObject::Update(dt);

	if (!isDead)
	{
		if (isAction)
		{
			if (isAttack)
			{
				this->TestA(dt);
			}
			else if (!isAttack)
			{
				x += dx;
				y += dy;
				deplayAttack -= 10;
			}
		}
		if (this->HPboss == 0)
		{
			this->isDead = true;
			this->state = BOSSBAT_DIE;
			ani = BOSSBAT_DIE;
			this->is_Item();
		}
		//-------------------------
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{

		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

			// block 
			x += dx;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.4f;

			//if (nx != 0) vx = 0;
			if (ny != 0) vy = 0;

			// Collision logic with Object after collision vs mario
			for (UINT i = 0; i < coEventsResult.size(); i++)//UNIT
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<Axe *>(e->obj))
				{
					if (this->HPboss > 0)
					{
						this->HPboss -= 1;
					}
				}

			}
		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		//-------------------------
	}


}

void BossBat::Render(float xViewport, float yViewport)
{
	//Update action
	this->Action(xViewport, yViewport);

	//Update state
	this->SetState(this->state);

	//Update limit
	this->UpdateLimit(xViewport, yViewport);

	int alpha = 255;
	this->isLeft = this->nx == 1 ? true : false;

	this->X_view = x - xViewport;
	this->Y_view = y - yViewport;

	//if (!(isDead && isvisible))
	if (!isDead)
	{
		animations[ani]->Render(x - xViewport, y - yViewport, alpha, this->isLeft);
		RenderBoundingBox();
	}
}

void BossBat::SetState(int state)
{
	CGameObject::SetState(state);

	switch (this->state)
	{
		case BOSSBAT_STATIC:
		{
			ani = BOSSBAT_STATIC;
			if (isAction)
			{
				this->state = BOSSBAT_FLY;
				vx = vRandom[1];
				vy = vRandom[1];
			}
			break;
		}
		case BOSSBAT_FLY:
		{			
			ani = BOSSBAT_FLY;
			if (deplayAttack == 0)
			{
				this->state = BOSSBAT_ATTACK;
				isAttack = true;
			}
			break;
		}

		case BOSSBAT_ATTACK:
		{
			this->vx = 0.2f;
			this->vy = 0.2f;
			if (!isAttack)
			{
				this->state = BOSSBAT_FLY;
				this->deplayAttack = 5000;
			}
			break;
		}
	}
}

void BossBat::is_Item()
{
	//whip collision vs candlesmall
	item->SetPosition(this->x, this->y);
	item->SetInvisible(false);

	//update y = x for equation sin
	item->Setcurrent_Oy(this->x);
}

void BossBat::not_Item()
{
	//simon eat item
	item->SetDead(true);
}

void BossBat::UpdateLimit(float xViewport, float yViewport)
{
	r.left = xViewport;
	r.top = yViewport + 100;
	r.right = r.left + SCREEN_WIDTH;
	r.bottom = r.top + SCREEN_HEIGHT - 170;

	if (this->x + 100 >= r.right)
	{
		srand(time(0));
		int b = 0 + rand() % (4);
		vx = -vRandom[b];
	}

	else if (this->x <= r.left)
	{
		srand(time(0));
		int b = 0 + rand() % (4);
		vx = vRandom[b];
		
	}

	else if(this->y <= r.top)
	{
		srand(time(0));
		int b = 0 + rand() % (4);
		vy = vRandom[b];
		
	}

	else if (this->y + 46 >= r.bottom)
	{
		srand(time(0));
		int b = 0 + rand() % (4);
		vy = -vRandom[b];
	}
}

void BossBat::Action(float xViewport, float yViewport)
{
	if ((this->x - (xViewport + SCREEN_WIDTH / 2)) <= DIS_BOSSBAT_ATTACK)
	{
		this->isAction = true;
	}
}

void BossBat::UpdatePosSimon(float _simonX, float _simonY)
{
	if (this->isAction && !isDead)
	{
		this->simonX = _simonX;
		this->simonY = _simonY;

	}
}

void BossBat::TestA(DWORD dt)
{
	//position target
	float xA = this->simonX;
	float yA = this->simonY;

	//boss -> target
	distance = std::sqrt(
		std::pow(xA - this->x, 2) +
		std::pow(yA - this->y, 2));

	//vector PT
	float ax, ay;
	ax = (xA - this->x) / distance;
	ay = (yA - this->y) / distance;

	//position Boss temp
	float posX , posY;

	posX = this->x;
	posY = this->y;
	posX += ax * vx *dt;
	posY += ay * vy *dt;

	if (std::sqrt(std::pow(xA - posX, 2) + std::pow(yA - posY, 2)) >= distance)
	{
		posX = xA;
		posY = yA;
		isAttack = false;
	}

	if (isAttack)
	{
		this->x = posX;
		this->y = posY;
	}
}

void BossBat::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (!isDead)
	{
		left = x;
		top = y;
		right = left + BOSSBAT_BBOX_WIDTH;
		bottom = top + BOSSBAT_BBOX_HEIGHT;
	}
}
