#include "Panther.h"
#include "tinyxml.h"

void Panther::LoadResource()
{
	//Load Item
	this->item = new Items();
	this->item->LoadResource();
	this->item->SetState(I_HEART_BIG);
	this->item->SetInvisible(true);

	//load resource zombie
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_PANTHER, L"Resource\\sprites\\Enemies\\PANTHER.png", D3DCOLOR_XRGB(255, 0, 255));

	//sprite
	CSprites * sprites = CSprites::GetInstance();
	LPDIRECT3DTEXTURE9 texPan = textures->Get(ID_TEX_PANTHER);

	//animations
	LPANIMATION ani;
	CAnimations * animations = CAnimations::GetInstance();

	//init XML
	TiXmlDocument doc("ResourceXML\\panther.xml");
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
			sprites->Add(idFrame, left, top, right, bottom, texPan);
			ani->Add(idFrame);
		}

		//load 1 frame finish
		animations->Add(id, ani);
		this->AddAnimation(id);
	}
}

void Panther::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT>* coObjectStatic, vector<LPGAMEOBJECT>* coObjectEnemy)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	vy += PANTHER_GRAVITY * dt;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();


	CalcPotentialCollisions(coObjects, coEvents); 

	// No collision occured, proceed normally
	if (coEvents.size() == 0) 
	{
		if (!isDead)
		{
			x += dx;
			y += dy;
		}
	}
	else	
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += dx;						// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0; 
		if (ny != 0) vy = 0; 

		// Collision logic with Object after collision vs panther
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Panther::Render(float xViewport, float yViewport)
{
	CGameObject::SetState(state);

	//Update limit
	this->UpdateLimit(xViewport, yViewport);

	//Update action
	this->Action(xViewport, yViewport);
	
	//Update state of panther
	this->SetState(state);

	this->X_view = x - xViewport;
	this->Y_view = y - yViewport;

	int alpha = 255;
	this->isLeft = this->nx == 1 ? true : false;

	if (!(isDead && isvisible))
	{
		animations[ani]->Render(x - xViewport, y - yViewport, alpha, this->isLeft);
		RenderBoundingBox();
	}
	else if (isDead)
	{
		if(this->state == PANTHER_DIE)
		{
			animations[ani]->Render(x - xViewport, y - yViewport, alpha, this->isLeft);
			RenderBoundingBox();
		}
	}
}

void Panther::SetState(int state)
{
	CGameObject::SetState(state);

	switch (this->state)
	{
		case PANTHER_STATIC:
		{
			this->vx = 0;

			if (isAction)
			{
				this->state = PANTHER_JUMP;
				ani = PANTHER_JUMP;
				vx = -PANTHER_WALKING_SPEED;
				vy = -PANTHER_JUMP_SPEED;
				break;
			}
			break;
		}
		case PANTHER_JUMP:
		{
			if (animations[PANTHER_JUMP]->CheckDoAllFrame())
			{
				this->state = PANTHER_MOVE;
				ani = PANTHER_MOVE;
				break;
			}
			break;
		}
		case PANTHER_MOVE:
		{
			ani = PANTHER_MOVE;
			break;
		}

		case PANTHER_DIE:
		{
			ani = PANTHER_DIE;
			if (animations[PANTHER_DIE]->CheckDoAllFrame())
			{
				animations[PANTHER_DIE]->SetDoAllFrame(false);
				this->state = PANTHER_STATIC;
				ani = PANTHER_STATIC;
				break;
			}
			break;
		}
	}
}

void Panther::is_Item()
{
	item->SetPosition(this->x, this->y);
	item->SetInvisible(false);
}

void Panther::UpdateLimit(float xViewport, float yViewport)
{
	if (this->x - xViewport < 0.0f)
	{
		this->SetInvisible(true);
		this->SetDead(true);
	}
}

void Panther::Action(float xViewport, float yViewport)
{
	if ((this->x - (xViewport + SCREEN_WIDTH/2)) <= DIS_PANTHER_ATTACK)
	{
		this->isAction = true;
	}
}

void Panther::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (!isDead)
	{
		left = x;
		top = y;
		right = left + PANTHER_BBOX_WIDTH;
		bottom = top + PANTHER_BBOX_HEIGHT;
	}
}
