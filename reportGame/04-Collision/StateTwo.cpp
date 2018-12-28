#include "StateTwo.h"
#include "Knife.h"
#include "Axe.h"
#include "tinyxml.h"

StateTwo::StateTwo()
{

}

StateTwo::~StateTwo()
{

}

void StateTwo::initText(LPDIRECT3DDEVICE9 d3ddv)
{
	uigame->initFont(d3ddv);
}

void StateTwo::InitState(LPDIRECT3DDEVICE9 dv, LPDIRECT3DSURFACE9 bb)
{

}

void StateTwo::LoadState(int AS)
{
	CGameState::LoadState(AS);

	pointdoor.clear();
	D3DXVECTOR2 pos(3164.0f, 415.0f);
	D3DXVECTOR2 pos1(3805.0f, 415.0f);
	pointdoor.push_back(pos);
	pointdoor.push_back(pos1);

	isInitSimon = true;

	//effect
	ef = new CEffect();
	ef->LoadResource();
	ef->SetPosition(600, 200);
	ef->SetState(EFFECT_THOR);
	ef->SetDead(true);
	objecttsStatic.push_back(ef);

	//RECT enemy
	r1.left = 125;
	r1.top = 100;
	r1.right = 1662;
	r1.bottom = 480;

	r2.left = 2045;
	r2.top = 100;
	r2.right = 2942;
	r2.bottom = 480;

	r3.left = 4222;
	r3.top = 100;
	r3.right = 5435;
	r3.bottom = 480;

	//set position simon undoor
	if (AS >= 0)
	{
		simon->SetPosition(pointdoor[AS].x, pointdoor[AS].y - 80);
	}

	//UI
	uigame = new UI();
	uigame->LoadResource();
	uigame->SetPosition(0.0f, 20.0f);
	objects.push_back(uigame);

	//Box Map
	BoxMap *map1 = new BoxMap();
	map1->LoadResource();
	map1->SetPosition(pointdoor[0].x, pointdoor[0].y);
	map1->SetASDoor(AS1);
	map1->SetAniBoxMap(BOX_CHANGE_MAP2);
	objecttsStatic.push_back(map1);

	BoxMap *map2 = new BoxMap();
	map2->LoadResource();
	map2->SetPosition(pointdoor[1].x, pointdoor[1].y);
	map2->SetASDoor(AS2);
	map2->SetAniBoxMap(BOX_CHANGE_MAP2);
	objecttsStatic.push_back(map2);

	//------------------------------------------------------
	//init XML
	TiXmlDocument doc("ResourceXML\\map1A.xml");
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
	}
	int id;
	int id_obj;
	int posX, posY;

	//load XML ground
	TiXmlElement* rootG = doc.RootElement();
	TiXmlElement* ground = nullptr;
	float multiplier;
	ground = rootG->FirstChildElement("ground");
	for (TiXmlElement* obj = ground->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);
		obj->QueryFloatAttribute("multiplier", &multiplier);

		//init ground
		CLimit *limG = new CLimit();
		limG->SetwidthBBox(multiplier);
		limG->SetPosition((float)posX, (float)posY);
		limG->SetState(LIMIT_GROUND);
		objects.push_back(limG);
	}

	//load XML limit
	TiXmlElement* rootlim = doc.RootElement();
	TiXmlElement* limit = nullptr;
	limit = rootlim->FirstChildElement("limit");
	for (TiXmlElement* obj = limit->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);

		//init limit
		CLimit *lim = new CLimit();
		lim->SetPosition((float)posX, (float)posY);
		lim->SetState(LIMIT_CAMERA);
		//objects.push_back(lim);
	}

	//load XML candle
	TiXmlElement* rootcan = doc.RootElement();
	TiXmlElement* candle = nullptr;
	int itemOb;
	candle = rootcan->FirstChildElement("candlemi");
	for (TiXmlElement* obj = candle->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);
		obj->QueryIntAttribute("it", &itemOb);

		//init candle
		CandleSmall *can = new CandleSmall();
		can->LoadResource();
		can->setItOb(itemOb);
		can->SetPosition((float)posX, (float)posY);
		can->SetState(CANDLESMALL_STATE_NO);
		objects.push_back(can->GetItem());
		objecttsStatic.push_back(can);
	}

	//load XML candle
	TiXmlElement* rootric = doc.RootElement();
	TiXmlElement* brick = nullptr;
	int itemObBrick;
	brick = rootric->FirstChildElement("brick");
	for (TiXmlElement* obj = brick->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);
		obj->QueryIntAttribute("it", &itemObBrick);

		//brick
		BrickCell *ric = new BrickCell();
		ric->LoadResource();
		ric->setItOb(itemObBrick);
		ric->SetPosition((float)posX, (float)posY);
		objects.push_back(ric->GetItem());
		objecttsStatic.push_back(ric);
	}

	//load XML boss
	TiXmlElement* rootbos = doc.RootElement();
	TiXmlElement* bos = nullptr;
	bos = rootbos->FirstChildElement("boss");
	for (TiXmlElement* obj = bos->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);

		//init boss
		boss = new BossBat();
		boss->LoadResource();
		boss->SetID(id_obj);
		boss->SetPosition((float)posX, (float)posY);
		boss->SetState(BOSSBAT_STATIC);
		objectsEnemy.push_back(boss);
		objects.push_back(boss->GetItem());
	}

	//load XML panther
	TiXmlElement* rootpan = doc.RootElement();
	TiXmlElement* panther = nullptr;
	panther = rootpan->FirstChildElement("panther");
	for (TiXmlElement* obj = panther->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);

		//init panther
		Panther *pan1 = new Panther();
		pan1->LoadResource();
		pan1->SetID(id_obj);
		pan1->SetPosition((float)posX, (float)posY);
		pan1->SetNX(-1);
		pan1->SetState(PANTHER_STATIC);
		objects.push_back(pan1->GetItem());
		objectsEnemy.push_back(pan1);
	}

	//load XML bat red
	TiXmlElement* rootbat = doc.RootElement();
	TiXmlElement* bat = nullptr;
	bat = rootbat->FirstChildElement("batmi");
	for (TiXmlElement* obj = bat->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);

		//init bat red
		BatRed *bat1 = new BatRed();
		bat1->LoadResource();
		bat1->SetID(id_obj);
		bat1->SetPosition((float)posX, (float)posY);
		bat1->SetNX(-1);
		bat1->SetState(BAT_RED_STATIC);
		objectsEnemy.push_back(bat1);
	}

	//load XML zombie
	TiXmlElement* rootzom = doc.RootElement();
	TiXmlElement* zom = nullptr;
	int rectlim;
	int padRight;
	zom = rootzom->FirstChildElement("zombie");
	for (TiXmlElement* obj = zom->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);
		obj->QueryIntAttribute("rect", &rectlim);
		obj->QueryIntAttribute("pad", &padRight);

		//init bat red
		Zombie *zom9 = new Zombie();
		zom9->LoadResource();
		zom9->SetID(id_obj);
		zom9->SetSpeed(-0.1f, 1.0f);
		zom9->SetNX(-1);
		switch (rectlim)
		{
		case 1:
		{
			zom9->SetRectLim(r1);
			zom9->SetPosition((float)r1.right - padRight, (float)posY);
			break;
		}
		case 2:
		{
			zom9->SetRectLim(r2);
			zom9->SetPosition((float)r2.right - padRight, (float)posY);
			break;
		}
		case 3:
		{
			zom9->SetRectLim(r3);
			zom9->SetPosition((float)r3.right - padRight, (float)posY);
			break;
		}
		}
		zom9->SetState(ZOMBIE_MOVE_LEFT);
		objectsEnemy.push_back(zom9);
	}

	//load XML stair
	TiXmlElement* rootstair = doc.RootElement();
	TiXmlElement* stair = nullptr;
	int isLeft, boxState;
	stair = rootstair->FirstChildElement("boxstair");
	for (TiXmlElement* obj = stair->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);
		obj->QueryIntAttribute("left", &isLeft);
		obj->QueryIntAttribute("state", &boxState);

		//init boss
		BOXStair *box11 = new BOXStair();
		box11->LoadResource();
		box11->SetcurrentBox(isLeft);
		box11->SetState(boxState);
		box11->SetPosition((float)posX, (float)posY);
		objecttsStatic.push_back(box11);
	}
	//------------------------------------------------------

	//Groud
	BOXStair *box_dl = new BOXStair();
	box_dl->LoadResource();
	box_dl->SetPosition(2972.0f, 150);
	box_dl->SetState(BOX_LEFT);
	objecttsStatic.push_back(box_dl);

	BOXStair *box_dr = new BOXStair();
	box_dr->LoadResource();
	box_dr->SetPosition(3200.0f, 150);
	box_dr->SetState(BOX_RIGHT);
	objecttsStatic.push_back(box_dr);

	//door
	Door *d1 = new Door();
	d1->LoadResource();
	d1->SetState(DOOR_CLOSE);
	d1->SetPosition(3055.0f, 127.0f);
	objects.push_back(d1);

	Door *d2 = new Door();
	d2->LoadResource();
	d2->SetState(DOOR_CLOSE);
	d2->SetPosition(4077.0f, 129.0f);
	objects.push_back(d2);
}

void StateTwo::UpdateState(DWORD dt)
{
	boss->UpdatePosSimon(simon->x, simon->y);
	CGameState::UpdateState(dt);
	this->uigame->UpdateItem(simon);
	this->uigame->UpdateHPBoss(boss);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SM_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
		this->stop = false;
	}

	if (simon->GetStateItemSimon() == I_STOPWATCH)
	{
		this->StartUntouchable();
		this->stop = true;
		simon->SetItem(new Items());
	}

	//effect map
	if (simon->GetStateItemSimon() == I_ROSARY)
	{
		ef->SetDead(false);
	}
}

void StateTwo::RenderState(LPDIRECT3DDEVICE9 d3ddv, LPDIRECT3DSURFACE9 bb, LPD3DXSPRITE spriteHandler)
{
	//Update viewport
	tilemap->UpdateScrollPosition();
	CGameState::RenderState(d3ddv, bb, spriteHandler);
}