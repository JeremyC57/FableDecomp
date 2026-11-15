#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <list>
#include <algorithm>
#ifdef _MSC_VER
namespace std {template<class T>const T&min(const T&a,const T&b){return _cpp_min(a,b);}}
#endif
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#include "engine/CDefPointer.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/ETCInterfaceType.h"
#ifndef _MSC_VER
#define __declspec(x)
#endif
struct TestState {int stage,terminated,opening,places,items,owner,stockState,stockWrites,moves,looks,performed,constructed,destroyed,allocated,freed,lookupCalls,hitReads,failMove,failLook,dieMove,dieLook,expectedID,currentItem,selectedN;bool invalidate,dieNull;};
static TestState state;
CDefPointeeBase::~CDefPointeeBase(){if(Tracked)++state.freed;}
class CCharString{public:const char*Text;CCharString(const char*s,long n=-1):Text(s){assert(n==-1);}};
class CDefinitionManager;
class CDefClassBase:public CDefPointeeBase {
public:struct CSubDefInfo{long DefIndex;long OriginalParentDefIndex;};
 CDefinitionManager*PDefManager;bool Info;int ID;CSubDefInfo Sub;
 CDefClassBase():PDefManager(0),Info(false),ID(0){Sub.DefIndex=0;Sub.OriginalParentDefIndex=0;}
 template<class T>bool GetSubDef(CDefPointer<const T>*)const;
 virtual const CSubDefInfo*GetPSubDefInfo(const CCharString&s)const{assert(strcmp(s.Text,"CStockItemDef")==0);++state.lookupCalls;return Info?&Sub:0;}
};
class CStockItemDef:public CDefClassBase{public:static CCharString StatGetTypeName(){return CCharString("CStockItemDef",-1);}};
static int DefMode;static CDefPointeeBase* Borrowed;
class CDefinitionManager{public:CDefPointer<CDefClassBase>GetPDefFromGlobalIndex(long n){assert(n>=0&&n<8);CDefPointer<CDefClassBase>v;if(DefMode==2){v.Object=Borrowed;if(v.Object)v.Object->AddRef();}else if(DefMode){CStockItemDef*p=new CStockItemDef;p->ID=(int)n;p->Tracked=true;++state.allocated;v.Object=p;}return v;}};
static CDefinitionManager Manager;
class CTCShop;class CTCShopKeeper;class CTCThingOwner;class CTCCreatedEntity{};
class CThing:public CDefClassBase {
public:int Identity;bool Dead,HasDef,HasCreated;C3DVector Pos;CTCShopKeeper*Keeper;CTCThingOwner*Owner;CTCCreatedEntity Created;
 CThing():Identity(0),Dead(false),HasDef(false),HasCreated(false),Keeper(0),Owner(0){Pos.X=Pos.Y=Pos.Z=0.0f;PDefManager=&Manager;}
 template<class T>bool GetTC(T**) ;
 bool IsDead()const{return Dead;}
 bool HasBaseDef()const{return HasDef;}
 const CDefClassBase&PeekBaseDef()const{return *this;}
 const C3DVector&GetPos()const{return Pos;}
};
class CThingAICreature:public CThing{};
template<class T>class CIntelligentPointer{public:T*P;CIntelligentPointer(T*p=0):P(p){}T*Get()const{return P;}T*operator->()const{return Get();}bool operator==(int n)const{assert(n==0);return P==0;}bool operator!=(int n)const{assert(n==0);return P!=0;}};
template<class T>inline bool IS_THING_ALIVE(const T&thing){if(thing!=0){if(!thing->IsDead())return true;}return false;}
template<class T>bool IS_THING_DEAD(const T&thing){++state.hitReads;return thing==0||thing->IsDead();}
class CTCThingOwner {public:std::list<CIntelligentPointer<CThing> >Objects;std::list<CIntelligentPointer<CThing> >&GetObjectList(){return Objects;}};
class CTCShopKeeper{public:CTCShop*Shop;CTCShop*GetShopTC()const{return Shop;}};
class CTCShop{
public:enum ESTOCK_STATE{STOCKED_YES,STOCKED_NO,STOCKED_DONTKNOW};struct GoodsPlace{C3DVector pos;CRightHandedSet rhset;};
 CThing Parent;std::vector<GoodsPlace>Places;std::vector<long>Items;
 CThing&GetParentThing(){return Parent;}
 void SetStockState(ESTOCK_STATE v){assert(!state.terminated);if(!state.stockWrites)assert(v==STOCKED_DONTKNOW);else assert(v==(state.opening?STOCKED_YES:STOCKED_NO)&&state.stage==0);state.stockState=v;++state.stockWrites;}
 bool IsOpeningHours()const{assert(state.stockWrites==1);return state.opening!=0;}
 std::vector<GoodsPlace>GetFreeSaleItemPlaces(bool occupied){assert(occupied==(state.opening==0));return Places;}
 std::vector<long>GetSaleItemsToPlace()const{assert(state.opening);return Items;}
};
template<>bool CThing::GetTC<CTCShopKeeper>(CTCShopKeeper**p){if(Keeper){*p=Keeper;return true;}return false;}
template<>bool CThing::GetTC<CTCThingOwner>(CTCThingOwner**p){if(Owner){*p=Owner;return true;}return false;}
template<>bool CThing::GetTC<CTCCreatedEntity>(CTCCreatedEntity**p){if(HasCreated){*p=&Created;return true;}return false;}
static CThingAICreature Creature;static CTCShop*Shop;static CThing*Objects[4];static CIntelligentPointer<CThing>*Handles[4];
static int Nearest(const CThing&thing){int best=-1;float dist=0.0f;for(unsigned i=0;i<Shop->Places.size();++i){float delta=Shop->Places[i].pos.X-thing.Pos.X;if(delta<0)delta=-delta;if(best<0||delta<dist){best=(int)i;dist=delta;}}return best;}
static bool Eligible(const CThing*t){return t&&!t->Dead&&t->HasCreated&&t->HasDef&&t->Info;}
static int ExpectedItem(){for(int n=state.expectedID;n<4;++n)if(Eligible(Objects[n]))return n;return -1;}
class CCreatureActionBase{public:int Kind,Item;CCreatureActionBase(int k,int i):Kind(k),Item(i){assert(!state.terminated&&state.stage==2);++state.constructed;}virtual~CCreatureActionBase(){++state.destroyed;}};
class CCreatureAction_ShopkeeperPutAway:public CCreatureActionBase{public:CCreatureAction_ShopkeeperPutAway(CThingAICreature&c,CThing*p):CCreatureActionBase(0,p?p->Identity:-1){assert(&c==&Creature&&p==Objects[state.currentItem]);}};
class CCreatureAction_ShopkeeperGetOut:public CCreatureActionBase{public:CCreatureAction_ShopkeeperGetOut(CThingAICreature&c,CTCShop*s,const C3DVector&pos,const CRightHandedSet&rh,long item):CCreatureActionBase(1,(int)item){assert(&c==&Creature&&s==Shop&&item==Shop->Items[state.performed]);assert(pos.X==Shop->Places[state.performed].pos.X&&memcmp(&rh,&Shop->Places[state.performed].rhset,sizeof(rh))==0);}};
class CAIStateGroup_SetupWares {
public:CThingAICreature&GetCreature()const{return Creature;}
 void Terminate(){assert(!state.terminated);state.terminated=1;}
 EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat repeat,C3DVector pos,float proximity,bool&nav,EMovementType movement,bool a,bool b,bool c,const CThing*other,bool d,float last){
  assert(!state.terminated&&state.stockWrites==1&&state.stockState==CTCShop::STOCKED_DONTKNOW&&state.stage==0&&repeat==ACTION_REPEAT_UNTIL_FINISHED&&proximity==1.4f&&a&&!b&&c&&!other&&d&&last==0.0f);
  if(state.opening){assert(state.moves==state.performed&&state.performed<(int)std::min(Shop->Places.size(),Shop->Items.size()));assert(pos.X==Shop->Places[state.performed].pos.X);assert(movement==(state.performed==0?AI_MOVEMENT_RUNNING:AI_MOVEMENT_WALKING));}
  else{state.currentItem=ExpectedItem();assert(state.currentItem>=0);CThing*t=Objects[state.currentItem];int n=Nearest(*t);assert(pos.X==(n<0?t->Pos.X:Shop->Places[n].pos.X)&&pos.Y==0.0f&&pos.Z==0.0f&&movement==AI_MOVEMENT_WALKING);}
  nav=true;state.stage=1;++state.moves;if(!state.opening&&state.moves==state.dieMove){if(state.dieNull)Handles[state.currentItem]->P=0;else Objects[state.currentItem]->Dead=true;}return state.moves==state.failMove?AI_ACTION_FAILED:AI_ACTION_FINISHED;
 }
 EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat,C3DVector pos,long frames,float speed){assert(!state.terminated&&state.stage==1&&repeat==ACTION_REPEAT_UNTIL_FINISHED&&frames==17&&speed==0.0027777f);if(state.opening)assert(pos.X==Shop->Places[state.performed].pos.X);else{CThing*t=Objects[state.currentItem];int n=Nearest(*t);assert(pos.X==(n<0?t->Pos.X:Shop->Places[n].pos.X));}state.stage=2;++state.looks;if(!state.opening&&state.looks==state.dieLook){if(state.dieNull)Handles[state.currentItem]->P=0;else Objects[state.currentItem]->Dead=true;}return state.looks==state.failLook?AI_ACTION_FAILED:AI_ACTION_FINISHED;}
 EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase&a,bool wait){assert(!state.terminated&&state.stage==2&&!wait&&a.Kind==state.opening);if(state.opening)assert(a.Item==Shop->Items[state.performed]);else{assert(a.Item==state.currentItem);state.expectedID=state.currentItem+1;}++state.performed;state.stage=0;return state.performed%2?AI_ACTION_FAILED:AI_ACTION_IN_PROGRESS;}
 void Update();
};

class CTCBase {};template<class K,class V>class CKeyPairCompareLess {};
template<class K,class V,class CompareType>class CVectorMap {public:typedef std::pair<K,V>value_type;typedef value_type*iterator;value_type Backing[33];int Count;iterator begin(){return Backing;}iterator end(){return Backing+Count;}iterator LowerBound(const K&);};
template<class K,class V,class CompareType>
__declspec(noinline) typename CVectorMap<K,V,CompareType>::iterator CVectorMap<K,V,CompareType>::LowerBound(const K& key)
{
    iterator first=this->begin();
    long count=this->end()-first;
    while(count>0) {
        long half=count/2;
        iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}
// Native external GetSubDef<CStockItemDef>,0059350C..005935AE,162 bytes: size optimization and frame pointer.
// This real specialization retains the shared template's complete original behavior.
#ifdef _MSC_VER
#pragma optimize("s",on)
#pragma optimize("y",off)
#endif
template<> bool CDefClassBase::GetSubDef<CStockItemDef>(CDefPointer<const CStockItemDef>* def)const
{
    const CSubDefInfo* psub_def_info=GetPSubDefInfo(CStockItemDef::StatGetTypeName());
    if(psub_def_info){
        CDefPointer<const CDefClassBase> pbase(PDefManager->GetPDefFromGlobalIndex(psub_def_info->DefIndex));
        def->SetDynamicCast(pbase);
        return true;
    }
    return false;
}
#ifdef _MSC_VER
#pragma optimize("",on)
#endif

void CAIStateGroup_SetupWares::Update()
{
    CTCShop* tc_shop;
    CTCShopKeeper* tc_shopkeeper;
    if (!GetCreature().GetTC(&tc_shopkeeper) || !tc_shopkeeper->GetShopTC()) {
        Terminate();
        return;
    }
    tc_shop = tc_shopkeeper->GetShopTC();
    tc_shop->SetStockState(CTCShop::STOCKED_DONTKNOW);
    if (!tc_shop->IsOpeningHours()) {
        std::vector<CTCShop::GoodsPlace> places = tc_shop->GetFreeSaleItemPlaces(true);
        CTCThingOwner* tc_thing_owner;
        if (tc_shop->GetParentThing().GetTC(&tc_thing_owner)) {
            const std::list<CIntelligentPointer<CThing> >& objects = tc_thing_owner->GetObjectList();
            for (std::list<CIntelligentPointer<CThing> >::const_iterator object_it=objects.begin();object_it!=objects.end();++object_it) {
                CTCCreatedEntity* created;
                CDefPointer<const CStockItemDef> stock_item;
                if (IS_THING_ALIVE(*object_it) && (*object_it)->GetTC(&created) && (*object_it)->HasBaseDef() && (*object_it)->PeekBaseDef().GetSubDef(&stock_item)) {
                    float best_dist=0.0f;
                    EAIActionReturn ret;
                    C3DVector best=(*object_it)->GetPos();
                    bool nav_route_calculated;
                    for (std::vector<CTCShop::GoodsPlace>::iterator i=places.begin();i!=places.end();++i) {
                        float dist=(i->pos-(*object_it)->GetPos()).GetMagnitude();
                        if (i==places.begin() || dist<best_dist) {
                            best_dist=dist;
                            best=i->pos;
                        }
                    }
                    ret=AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED,best,1.4f,nav_route_calculated,AI_MOVEMENT_WALKING,true,false,true,0,true,0.0f);
                    if (ret!=AI_ACTION_FINISHED || IS_THING_DEAD(*object_it)) { Terminate();return; }
                    ret=AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED,best,0x11,0.0027777f);
                    if (ret!=AI_ACTION_FINISHED || IS_THING_DEAD(*object_it)) { Terminate();return; }
                    AIAction_PerformActionAndWait(CCreatureAction_ShopkeeperPutAway(GetCreature(),object_it->Get()),false);
                }
            }
        }
        tc_shop->SetStockState(CTCShop::STOCKED_NO);
    } else {
        std::vector<CTCShop::GoodsPlace> places=tc_shop->GetFreeSaleItemPlaces(false);
        std::vector<long> items=tc_shop->GetSaleItemsToPlace();
        for (int n=0;n<std::min(places.size(),items.size());++n) {
            EMovementType movement=AI_MOVEMENT_WALKING;
            EAIActionReturn ret;
            bool nav_route_calculated;
            if (n==0) movement=AI_MOVEMENT_RUNNING;
            ret=AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED,places[n].pos,1.4f,nav_route_calculated,movement,true,false,true,0,true,0.0f);
            if(ret!=AI_ACTION_FINISHED){Terminate();return;}
            ret=AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED,places[n].pos,0x11,0.0027777f);
            if(ret!=AI_ACTION_FINISHED){Terminate();return;}
            AIAction_PerformActionAndWait(CCreatureAction_ShopkeeperGetOut(GetCreature(),tc_shop,places[n].pos,places[n].rhset,items[n]),false);
        }
        tc_shop->SetStockState(CTCShop::STOCKED_YES);
    }
    Terminate();
}

int main(){unsigned cases=0;int coverage[7]={0,0,0,0,0,0,0};
 for(int seed=0;seed<4096;++seed){memset(&state,0,sizeof(state));state.opening=seed%2;state.places=seed/2%4;state.items=seed/7%4;state.failMove=seed/11%5;state.failLook=seed/17%5;state.dieMove=seed/23%5;state.dieLook=seed/29%5;DefMode=seed/31%2;state.dieNull=seed/53%2!=0;
  CTCShop shop;Shop=&shop;CTCThingOwner owner;CTCShopKeeper keeper;CThing objs[4];keeper.Shop=seed/37%5==0?0:&shop;Creature.Keeper=seed/41%5==0?0:&keeper;shop.Parent.Owner=seed/43%3==0?0:&owner;
  for(int n=0;n<state.places;++n){CTCShop::GoodsPlace p;memset(&p,0,sizeof(p));p.pos.X=(float)(n*4);shop.Places.push_back(p);}for(int n=0;n<state.items;++n)shop.Items.push_back(20+n);
  for(int n=0;n<4;++n){CThing&t=objs[n];t.Identity=n;t.Pos.X=(float)(n*3);t.Dead=(seed/(n+2))%5==0;t.HasCreated=(seed/(n+3))%4!=0;t.HasDef=(seed/(n+5))%3!=0;t.Info=(seed/(n+7))%3!=0;t.Sub.DefIndex=n;Objects[n]=seed/(n+11)%7==0?0:&t;owner.Objects.push_back(CIntelligentPointer<CThing>(Objects[n]));Handles[n]=&owner.Objects.back();}
  CAIStateGroup_SetupWares script;script.Update();assert(state.terminated&&state.allocated==state.freed&&state.constructed==state.destroyed&&state.constructed==state.performed);
  bool valid=Creature.Keeper&&keeper.Shop;if(!valid){assert(!state.stockWrites&&!state.moves&&!state.looks&&!state.performed);++coverage[0];}
  else{assert(state.stockWrites>=1&&state.stockWrites<=2&&state.performed<=state.looks&&state.looks<=state.moves);if(state.stockWrites==2){assert(state.stage==0);if(state.opening)assert(state.performed==(int)std::min(shop.Places.size(),shop.Items.size()));else assert(ExpectedItem()<0||!shop.Parent.Owner);++coverage[1];}else assert(state.stage!=0);if(state.opening&&state.performed)++coverage[2];if(!state.opening&&state.performed)++coverage[3];if(!state.opening&&state.dieMove&&state.moves==state.dieMove)++coverage[4];if(!DefMode&&state.lookupCalls)++coverage[5];if(state.places==0&&state.moves)++coverage[6];}
  ++cases;
 }
 assert(cases==4096u);for(int k=0;k<7;++k)assert(coverage[k]>0);
 // Actual full helper specialization: absent info leaves an existing out handle intact;
 // successful null and nonnull manager returns replace it with balanced reference counts.
 for(int mode=0;mode<3;++mode)for(int info=0;info<2;++info)for(int old=0;old<2;++old){memset(&state,0,sizeof(state));DefMode=mode;CDefClassBase base;base.PDefManager=&Manager;base.Info=info!=0;base.Sub.DefIndex=3;{CDefPointer<const CStockItemDef>out;if(old){CStockItemDef*p=new CStockItemDef;p->Tracked=true;++state.allocated;out.Object=p;}CDefPointeeBase*prior=out.Object;Borrowed=prior;bool ok=base.GetSubDef(&out);assert(ok==(info!=0));if(!info)assert(out.Object==prior);else assert((out.Object!=0)==(mode==1||(mode==2&&old)));if(out.Object)assert(out.Object->IVRefCount==1);}assert(state.allocated==state.freed);}
 unsigned searches=0;for(int count=0;count<=32;++count){CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >map;map.Count=count;for(int n=0;n<count;++n){map.Backing[n].first=(ETCInterfaceType)(n/2*3);map.Backing[n].second=0;}for(int key=0;key<100;++key){ETCInterfaceType k=(ETCInterfaceType)key;int expected=0;while(expected<count&&map.Backing[expected].first<k)++expected;assert(map.LowerBound(k)==map.Backing+expected);++searches;}}assert(searches==3300u);
 puts("00927f30_TEST PASS");return 0;}
