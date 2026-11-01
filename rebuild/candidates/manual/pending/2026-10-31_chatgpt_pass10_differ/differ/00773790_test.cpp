// 00773790 behavior fixture: selected target body pasted unchanged.
// Real constructed objects; unused virtual interfaces reduced, field access exposed only for setup.
// Map lookup and external engine/definition methods are checked boundaries; no native layout claim.
#include <stdio.h>
#include <math.h>
static int failures=0;static void Check(bool x){if(!x)++failures;}
enum ETCInterfaceType{HeroStatsType=4,KeeperType=0x4f,HouseType=0xd5};
enum EOpinionAttitudeType{Attitude0,Attitude1,Attitude2};
namespace std{template<class A,class B>struct pair{A first;B second;};template<class K,class V>class map{public:typedef pair<K,V>value_type;typedef const value_type*const_iterator;value_type entries[3];unsigned int count;mutable int finds;map():count(0),finds(0){}const_iterator find(const K&k)const{++finds;for(unsigned int j=0;j<count;++j)if(entries[j].first==k)return entries+j;return end();}const_iterator end()const{return entries+count;}};}
class CDefPointeeBase{public:mutable int refs,adds,releases,deletes;CDefPointeeBase():refs(1),adds(0),releases(0),deletes(0){}virtual~CDefPointeeBase(){}void AddRef()const{++refs;++adds;}void Release()const{--refs;++releases;if(!refs)++deletes;}};
template<class T>class CDefPointer{T*Object;public:CDefPointer():Object(0){}~CDefPointer(){if(Object)Object->Release();}T*Get()const{return Object;}T*operator->()const{return Object;}void Reset(T*p){if(Object!=p){if(Object)Object->Release();Object=p;if(Object)Object->AddRef();}}};
class CShopDefFields:public CDefPointeeBase{public:float AttitudeSellPriceMultiplierDefault;std::map<EOpinionAttitudeType,float>*AttitudeSellPriceMultiplier;};
class CShopDef:public CShopDefFields{public:std::map<EOpinionAttitudeType,float>ownedMap;CShopDef(){AttitudeSellPriceMultiplier=&ownedMap;}};
class CThingBaseDef:public CDefPointeeBase{public:const CShopDef*shop;bool available;mutable int subcalls;CThingBaseDef(const CShopDef*s,bool a):shop(s),available(a),subcalls(0){}bool GetSubDef(CDefPointer<const CShopDef>*out)const{++subcalls;if(!available)return false;out->Reset(shop);return true;}};
class CTCBase{public:virtual~CTCBase(){}};
class CTCBuyableHouse:public CTCBase{public:bool owned;mutable int reads;CTCBuyableHouse(bool b):owned(b),reads(0){}bool GetOwnedByPlayer()const{++reads;return owned;}};
class CTCHeroStats:public CTCBase{public:float skill;mutable int reads;CTCHeroStats(float v):skill(v),reads(0){}float GetTradeSkill()const{++reads;return skill;}};
class CThing{public:unsigned long defIndex;bool hasHouse,hasKeeper,hasStats;CTCBuyableHouse*house;CTCHeroStats*stats;mutable int peeks,keeperChecks,statsChecks;int gets;CThing():defIndex(0),hasHouse(false),hasKeeper(false),hasStats(false),house(0),stats(0),peeks(0),keeperChecks(0),statsChecks(0),gets(0){}bool PeekTC(const CTCBuyableHouse**out)const{++peeks;if(hasHouse){*out=house;return true;}return false;}unsigned long GetDefGlobalIndex()const{return defIndex;}bool HasTC(ETCInterfaceType t)const{if(t==KeeperType){++keeperChecks;return hasKeeper;}Check(t==HeroStatsType);++statsChecks;return hasStats;}CTCBase&GetTC(ETCInterfaceType t){Check(t==HeroStatsType&&hasStats);++gets;return*stats;}};
class CThingCreatureBase:public CThing{};class CThingPlayerCreature:public CThingCreatureBase{};
template<class T>class CIntelligentPointer{public:T*ptr;mutable int calls;CIntelligentPointer():ptr(0),calls(0){}T*Get()const{++calls;return ptr;}};
class CGameDefinitionManager{public:const CThingBaseDef*def;unsigned long expectedIndex;mutable int calls;CGameDefinitionManager(const CThingBaseDef*d):def(d),expectedIndex(0),calls(0){}bool GetDef(long index,CDefPointer<const CThingBaseDef>*out)const{++calls;Check((unsigned long)index==expectedIndex);out->Reset(def);return true;}};
class COpinionReactionManager{public:EOpinionAttitudeType attitude;const CThing*expected;mutable int calls;COpinionReactionManager():attitude(Attitude0),expected(0),calls(0){}EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing&t)const{Check(&t==expected);++calls;return attitude;}};
class CWorld{public:COpinionReactionManager reactions;mutable int peeks;CWorld():peeks(0){}const COpinionReactionManager&PeekOpinionReactionManager()const{++peeks;return reactions;}};
class CPlayer{public:CThingPlayerCreature*hero;mutable int calls;CPlayer():hero(0),calls(0){}CThingPlayerCreature*GetPControlledCreature()const{++calls;return hero;}};
class CPlayerManager{public:CPlayer player;mutable int calls;CPlayerManager():calls(0){}const CPlayer&GetMainPlayer()const{++calls;return player;}};
class CTCShop:public CTCBase{public:CThing&ParentThing;CIntelligentPointer<CThingCreatureBase>PShopKeeper;const CGameDefinitionManager&defs;const CWorld&world;const CPlayerManager&players;mutable int defGets,worldGets,playerGets;
 CTCShop(CThing&t,const CGameDefinitionManager&d,const CWorld&w,const CPlayerManager&p):ParentThing(t),defs(d),world(w),players(p),defGets(0),worldGets(0),playerGets(0){}const CGameDefinitionManager&GetDefinitionManager()const{++defGets;return defs;}const CWorld&PeekWorld()const{++worldGets;return world;}const CPlayerManager&PeekPlayerManager()const{++playerGets;return players;}float GetAttitudeSellPriceMultiplier()const;
};
// Fixture changes the original writable constant to exactly representable values per scenario.
static float MAXIMUM_TRADE_SKILL=0.5f;
float CTCShop::GetAttitudeSellPriceMultiplier()const{
 const CThing&thing=ParentThing;
 const CTCBuyableHouse*house;if(thing.PeekTC(&house)){if(house->GetOwnedByPlayer())return 1.0f;}
 CDefPointer<const CThingBaseDef>thingDef;
 GetDefinitionManager().GetDef(thing.GetDefGlobalIndex(),&thingDef);
 CDefPointer<const CShopDef>shopDef;
 if(!thingDef->GetSubDef(&shopDef))return 1.0f;
 const CShopDefFields&fields=*reinterpret_cast<const CShopDefFields*>(shopDef.Get());
 float multiplier=fields.AttitudeSellPriceMultiplierDefault;
 if(PShopKeeper.Get()&&PShopKeeper.Get()->HasTC(static_cast<ETCInterfaceType>(0x4f))){
  const COpinionReactionManager&reactions=PeekWorld().PeekOpinionReactionManager();
  EOpinionAttitudeType attitude=reactions.GetThingAttitudeToPlayer(*PShopKeeper.Get());
  const std::map<EOpinionAttitudeType,float>&map=*reinterpret_cast<const std::map<EOpinionAttitudeType,float>*>(fields.AttitudeSellPriceMultiplier);
  std::map<EOpinionAttitudeType,float>::const_iterator it=map.find(attitude);if(it!=map.end())multiplier=it->second;
 }
 CThingPlayerCreature*hero=PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
 if(hero&&hero->HasTC(static_cast<ETCInterfaceType>(4))){float skill=static_cast<CTCHeroStats&>(hero->GetTC(static_cast<ETCInterfaceType>(4))).GetTradeSkill();multiplier+=(1.0f-multiplier)*(skill*MAXIMUM_TRADE_SKILL);}
 return multiplier;
}

int main(){unsigned int cases=0;
 for(int houseState=0;houseState<3;++houseState)for(int available=0;available<2;++available)for(int keeperState=0;keeperState<3;++keeperState)for(int heroState=0;heroState<3;++heroState)for(int matching=0;matching<4;++matching)for(int skillCase=0;skillCase<4;++skillCase)for(int scaleCase=0;scaleCase<3;++scaleCase){
  CShopDef shop;shop.AttitudeSellPriceMultiplierDefault=1.5f;for(int j=0;j<3;++j){shop.ownedMap.entries[j].first=(EOpinionAttitudeType)j;shop.ownedMap.entries[j].second=(float)j*0.5f;}shop.ownedMap.count=3;
  CThingBaseDef def(&shop,available!=0);CGameDefinitionManager defs(&def);CThing parent;parent.defIndex=217;defs.expectedIndex=217;CTCBuyableHouse house(houseState==2);parent.hasHouse=houseState!=0;parent.house=&house;
  CTCHeroStats stats((float)skillCase*0.5f);CThingCreatureBase keeper;keeper.hasKeeper=keeperState==2;CThingPlayerCreature hero;hero.hasStats=heroState==2;hero.stats=&stats;
  CWorld world;world.reactions.expected=&keeper;world.reactions.attitude=(EOpinionAttitudeType)matching;CPlayerManager players;players.player.hero=heroState?&hero:0;CTCShop receiver(parent,defs,world,players);receiver.PShopKeeper.ptr=keeperState?&keeper:0;
  MAXIMUM_TRADE_SKILL=(float)scaleCase*0.5f;bool early=houseState==2;bool continuing=!early&&available;bool opinion=continuing&&keeperState==2;bool trade=continuing&&heroState==2;
  float expected=1.0f;if(continuing){expected=opinion&&matching<3?(float)matching*0.5f:1.5f;if(trade)expected+=(1.0f-expected)*(stats.skill*MAXIMUM_TRADE_SKILL);}
  float actual=receiver.GetAttitudeSellPriceMultiplier();Check(actual==expected);Check(parent.peeks==1&&house.reads==(houseState?1:0));Check(defs.calls==(!early?1:0)&&def.subcalls==(!early?1:0));
  Check(receiver.defGets==(!early?1:0));Check(def.refs==1&&def.adds==(!early?1:0)&&def.releases==def.adds&&def.deletes==0);Check(shop.refs==1&&shop.adds==(continuing?1:0)&&shop.releases==shop.adds&&shop.deletes==0);
  int keeperCalls=continuing?(keeperState==0?1:keeperState==1?2:3):0;Check(receiver.PShopKeeper.calls==keeperCalls);Check(keeper.keeperChecks==(continuing&&keeperState?1:0));Check(world.reactions.calls==(opinion?1:0)&&world.peeks==(opinion?1:0)&&receiver.worldGets==(opinion?1:0));Check(shop.ownedMap.finds==(opinion?1:0));
  Check(receiver.playerGets==(continuing?1:0)&&players.calls==(continuing?1:0)&&players.player.calls==(continuing?1:0));Check(hero.statsChecks==(continuing&&heroState?1:0)&&hero.gets==(trade?1:0)&&stats.reads==(trade?1:0));++cases;
 }
 Check(cases==2592);if(failures){fprintf(stderr,"failures=%d cases=%u\n",failures,cases);return 1;}puts("00773790_TEST PASS");return 0;
}
