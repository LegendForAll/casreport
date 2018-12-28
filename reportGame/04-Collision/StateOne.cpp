#include "StateOne.h"
#include "tinyxml.h"

StateOne::StateOne()
{

}

StateOne::~StateOne()
{

}

void StateOne::initText(LPDIRECT3DDEVICE9 d3ddv)
{
	uigame->initFont(d3ddv);
}


void StateOne::InitState(LPDIRECT3DDEVICE9 dv, LPDIRECT3DSURFACE9 bb)
{

}

void StateOne::LoadState(int AS)
{
	CGameState::LoadState(AS);
	isInitSimon = true;

	//effect
	ef = new CEffect();
	ef->LoadResource();
	ef->SetPosition(600, 200);
	ef->SetState(EFFECT_THOR);
	ef->SetDead(true);
	objecttsStatic.push_back(ef);

	//UI
	uigame = new UI();
	uigame->LoadResource();
	uigame->SetPosition(0.0f, 20.0f);
	objects.push_back(uigame);

	//----------------------------------------------------------
	//init XML
	TiXmlDocument doc("ResourceXML\\mapR.xml");
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
	}
	int id;
	int id_obj;
	int posX, posY;
	

	//load XML candle
	TiXmlElement* rootcan = doc.RootElement();
	TiXmlElement* candle = nullptr;
	int itemOb;
	candle = rootcan->FirstChildElement("candle");
	for (TiXmlElement* obj = candle->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);
		obj->QueryIntAttribute("it", &itemOb);

		//init candle
		Candle *can = new Candle();
		can->SetID(id_obj);
		can->LoadResource();
		can->setItOb(itemOb);
		can->SetPosition((float)posX, (float)posY);
		objects.push_back(can->GetItem());
		objecttsStatic.push_back(can);
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
		lim->SetID(id_obj);
		lim->SetPosition((float)posX, (float)posY);
		objects.push_back(lim);
	}

	//load XML ground
	TiXmlElement* rootG = doc.RootElement();
	TiXmlElement* ground = nullptr;
	float multiplier;
	int typeG;
	ground = rootG->FirstChildElement("ground");
	for (TiXmlElement* obj = ground->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);
		obj->QueryFloatAttribute("multiplier", &multiplier);
		obj->QueryIntAttribute("state", &typeG);


		//init ground
		CLimit *limG = new CLimit();
		limG->SetID(id_obj);
		limG->SetwidthBBox(multiplier);
		limG->SetPosition((float)posX, (float)posY);
		limG->SetState(typeG);
		objects.push_back(limG);
	}

	//load XML boxmap
	TiXmlElement* rootMap = doc.RootElement();
	TiXmlElement* change = nullptr;
	int Cmap;
	change = rootMap->FirstChildElement("changemap");
	for (TiXmlElement* obj = change->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);
		obj->QueryIntAttribute("change", &Cmap);


		//init box map
		BoxMap *map = new BoxMap();
		map->LoadResource();
		map->SetID(id_obj);
		map->SetPosition((float)posX, (float)posY);
		map->SetAniBoxMap(Cmap);
		objecttsStatic.push_back(map);
	}
	//----------------------------------------------------------
}

void StateOne::UpdateState(DWORD dt)
{
	CGameState::UpdateState(dt);
	//effect map
	if (simon->GetStateItemSimon() == I_ROSARY)
	{
		ef->SetDead(false);
	}

	//--------------------------------------------------------------------
	if (simon->GetStateItemSimon() == I_STOPWATCH)
	{
		ef->SetDead(false);
	}
	//---------------------------------------------------------------------
	this->uigame->UpdateItem(simon);
}

void StateOne::RenderState(LPDIRECT3DDEVICE9 d3ddv, LPDIRECT3DSURFACE9 bb, LPD3DXSPRITE spriteHandler)
{
	//Update viewport
	tilemap->UpdateScrollPosition();
	CGameState::RenderState(d3ddv, bb, spriteHandler);

}
