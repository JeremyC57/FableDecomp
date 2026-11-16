// Self-contained full-body test: real shared class models and controlled external operations.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
enum ESex{};enum EAIStateGroupType{};enum EScriptAIPriority{};
#endif
#include "engine/EPlayerMode.h"
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CDEFPOINTER_RESET
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define private public
#define protected public

#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CAIBRAIN_CTOR_PASS23
#define FABLE_RETAIL_CTCSHOPKEEPER_CTOR_PASS23
#define FABLE_RETAIL_CTCTHINGOWNER_CTOR_PASS23
// 00927f30 CAIStateGroup_SetupWares::Update() -- puts away or displays shop wares according to opening hours.
// Retail:0x00927F30,1331 bytes,thiscall,complete body.
// Flags: none
// Leaf: ?Update@CAIStateGroup_SetupWares@@UAEXXZ
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASBASEDEF
#define FABLE_RETAIL_CTCSHOP
#define FABLE_RETAIL_CTCSHOP_STOCKSTATE
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CSTOCKITEMDEF
#define FABLE_RETAIL_IS_THING_DEAD_DECL_ONLY
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/C3DVector.h"
#include "engine/CAIStateGroup_SetupWares.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCShop.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCCreatedEntity.h"
#include "engine/CStockItemDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CCreatureAction_ShopkeeperPutAway.h"
#include "engine/CCreatureAction_ShopkeeperGetOut.h"
// Original IS_THING_DEAD<CIntelligentPointer<CThing> > is external at005CC1B0..005CC1D4 (36 bytes).
#include "engine/ThingAlive.h"
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#endif

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

#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CGame.h"
#include "engine/CMainGameComponent.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CThingManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CNavigatorManager.h"
#include "engine/CDefString.h"

struct TestState {int stage,terminated,opening,places,items,owner,stockState,stockWrites,moves,looks,performed,constructed,destroyed,allocated,freed,lookupCalls,hitReads,failMove,failLook,dieMove,dieLook,expectedID,currentItem,selectedN;bool invalidate,dieNull;};
static TestState state;
struct ObjectState {const CBaseObject*Object;int ID,References;bool Enemy,HasCreated,HasDef,Info;C3DVector Pos;};
static ObjectState States[128];static int StateCount;
static ObjectState& StateOf(const CBaseObject*p){for(int i=0;i<StateCount;++i)if(States[i].Object==p)return States[i];assert(StateCount<128);ObjectState&s=States[StateCount++];s.Object=p;s.ID=-1;s.References=0;s.Enemy=false;s.HasCreated=s.HasDef=s.Info=false;s.Pos=C3DVector(0,0,0);return s;}
static int ID(const CBaseObject*p){return StateOf(p).ID;}static int Refs(const CBaseObject*p){return StateOf(p).References;}static bool Enemy(const CBaseObject*p){return StateOf(p).Enemy;}
static const char*EffectName="HEAL_FIXTURE_EFFECT";
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseObject::CBaseObject():IntelligentPointer(0){} CBaseObject::~CBaseObject(){for(int i=0;i<StateCount;++i)if(States[i].Object==this){assert(States[i].References==0);States[i]=States[--StateCount];return;}}
CInitBaseClass::CInitBaseClass():Valid(false){}CInitBaseClass::~CInitBaseClass(){}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*s,long n){assert(n==-1);PStringData=new CCharStringData;PStringData->Data.PData=const_cast<char*>(s);PStringData->Data.StringLength=(unsigned long)strlen(s);PStringData->NoRefs=1;}
CCharString::CCharString(const CCharString&s):PStringData(s.PStringData){if(PStringData)++PStringData->NoRefs;}
CCharString::~CCharString(){if(PStringData&&--PStringData->NoRefs==0)delete PStringData;}
const CCharString&CCharString::operator=(const CCharString&s){if(PStringData!=s.PStringData){if(PStringData&&--PStringData->NoRefs==0)delete PStringData;PStringData=s.PStringData;if(PStringData)++PStringData->NoRefs;}return *this;}
const char CCharString::NullChar=0;

CWideString::CWideString():PStringData(0){}CWideString::~CWideString(){}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseObject*CBaseIntelligentPointer::GetPItem()const{return reinterpret_cast<CBaseObject*>(PData);}
void CBaseIntelligentPointer::SetPItem(CBaseObject*p){CBaseObject*old=GetPItem();if(old)--StateOf(old).References;PData=reinterpret_cast<CBaseObjectPointer*>(p);if(p)++StateOf(p).References;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){SetPItem(0);}
CTCBase::CTCBase(CThing&t):ParentThing(&t){}CTCBase::~CTCBase(){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){}CResource::~CResource(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CSubDefClassBase::CSubDefClassBase(){}CParentDefClassBase::CParentDefClassBase(){}CParentDefClassBase::~CParentDefClassBase(){}
CDefinitionManager::~CDefinitionManager(){}CFailedAllocationHandler::~CFailedAllocationHandler(){}CResourceList::~CResourceList(){}
void CIVCountedPointeeBase::Delete()const{abort();}
CClassFactory::CClassFactory(unsigned char){}CClassFactory::~CClassFactory(){}
CGameDefinitionManager::CGameDefinitionManager(){}CGameDefinitionManager::~CGameDefinitionManager(){}
CGame::CGame():CurrentGameComponent(0),Quit(false){}CGame::~CGame(){}
CGameComponent::CGameComponent(CGame&g):Quit(false),Running(false),Game(g){}CGameComponent::~CGameComponent(){}
void CGameComponent::Init(){}void CGameComponent::SetQuit(){Quit=true;}
CMainGameComponent::CMainGameComponent(CGame&g,const CMainGameComponentInit&):CGameComponent(g){}CMainGameComponent::~CMainGameComponent(){}
CMainGameComponentInit::CMainGameComponentInit(){}CMainGameComponentInit::~CMainGameComponentInit(){}
CThingManager::CThingManager(const CMainGameComponent&c,const CGameDefinitionManager&d,CWorld&w,CPlayerManager&p,CNavigatorManager&n,unsigned long*seed,unsigned long,unsigned long,unsigned long):CClassFactory(0),Component(c),DefinitionManager(d),World(w),WorldMap(*w.PWorldMap_ptr),WorldSeed(seed),PlayerManager(p),NavigationManager(n){}CThingManager::~CThingManager(){}

static int DefMode;static const CDefPointeeBase*Borrowed;
static CDefinitionManager Manager;
static CThingAICreature*CreaturePtr;static CTCShop*Shop;static CTCShop*KeeperShop;
static std::vector<CTCShop::GoodsPlace>FixturePlaces;static std::vector<long>FixtureItems;
static CThing*Objects[4];static CIntelligentPointer<CThing>*Handles[4];
CAIBrain::CAIBrain(const CMainGameComponent&c,CWorld&w,CThingManager&t,CPlayerManager&p,CThingAICreature&cr,CDefPointer<const CBrainDef>):Component(const_cast<CMainGameComponent*>(&c)),Creature(cr),ThingManager(&t),PlayerManager(&p),World(&w){}
CAIStateGroupBase::CAIStateGroupBase():PBrain(0){}
CAIStateGroup_SetupWares::CAIStateGroup_SetupWares(){}
CTCShop::CTCShop(CThing&t):CTCBase(t){}CTCShop::~CTCShop(){}
CTCShopKeeper::CTCShopKeeper(CThing&t):CTCBase(t){}
CTCThingOwner::CTCThingOwner(CThing&t):CTCBase(t){}
CTCCreatedEntity::CTCCreatedEntity(CThing&t):CTCBase(t){}
CTCShop*CTCShopKeeper::GetShopTC()const{return KeeperShop;}
const C3DVector&CThing::GetPos()const{return StateOf(this).Pos;}
void CTCShop::SetStockState(ESTOCK_STATE v){assert(!state.terminated);if(!state.stockWrites)assert(v==STOCKED_DONTKNOW);else assert(v==(state.opening?STOCKED_YES:STOCKED_NO)&&state.stage==0);state.stockState=v;++state.stockWrites;}
bool CTCShop::IsOpeningHours()const{assert(state.stockWrites==1);return state.opening!=0;}
std::vector<CTCShop::GoodsPlace>CTCShop::GetFreeSaleItemPlaces(bool occupied){assert(occupied==(state.opening==0));return FixturePlaces;}
std::vector<long>CTCShop::GetSaleItemsToPlace()const{assert(state.opening);return FixtureItems;}
template<class T>bool FABLE_FASTCALL IS_THING_DEAD(const T&thing){++state.hitReads;return thing==0||thing->IsDead();}
static int Nearest(const CThing&thing){int best=-1;float dist=0;for(unsigned i=0;i<FixturePlaces.size();++i){float delta=FixturePlaces[i].pos.X-StateOf(&thing).Pos.X;if(delta<0)delta=-delta;if(best<0||delta<dist){best=(int)i;dist=delta;}}return best;}
static bool Eligible(const CThing*t){return t&&!t->Dead&&StateOf(t).HasCreated&&StateOf(t).HasDef&&StateOf(t).Info;}
static int ExpectedItem(){for(int n=state.expectedID;n<4;++n)if(Eligible(Objects[n]))return n;return -1;}
static const CCreatureActionBase*CurrentAction;static int ActionKind,ActionItem;
CCreatureActionBase::~CCreatureActionBase(){++state.destroyed;}
CCreatureAction_ShopkeeperPutAway::~CCreatureAction_ShopkeeperPutAway(){}
CCreatureAction_ShopkeeperPutAway::CCreatureAction_ShopkeeperPutAway(CThingCreatureBase&c,CThing*p):Goods(p){assert(!state.terminated&&state.stage==2&&&c==CreaturePtr&&p==Objects[state.currentItem]);CurrentAction=this;ActionKind=0;ActionItem=StateOf(p).ID;++state.constructed;}
CCreatureAction_ShopkeeperGetOut::CCreatureAction_ShopkeeperGetOut(CThingCreatureBase&c,CTCShop*s,const C3DVector&pos,const CRightHandedSet&rh,long item):TCShop(s),Goods(item),Pos(pos),RHSet(rh){assert(!state.terminated&&state.stage==2&&&c==CreaturePtr&&s==Shop&&item==FixtureItems[state.performed]);assert(pos.X==FixturePlaces[state.performed].pos.X&&memcmp(&rh,&FixturePlaces[state.performed].rhset,sizeof rh)==0);CurrentAction=this;ActionKind=1;ActionItem=(int)item;++state.constructed;}
void CAIStateGroupBase::Terminate(){assert(!state.terminated);state.terminated=1;}
 EAIActionReturn CAIStateGroupBase::AIAction_MoveToPosProximity(EAIActionRepeat repeat,C3DVector pos,float proximity,bool&nav,EMovementType movement,bool a,bool b,bool c,const CThing*other,bool d,float last){
  assert(!state.terminated&&state.stockWrites==1&&state.stockState==CTCShop::STOCKED_DONTKNOW&&state.stage==0&&repeat==ACTION_REPEAT_UNTIL_FINISHED&&proximity==1.4f&&a&&!b&&c&&!other&&d&&last==0.0f);
  if(state.opening){assert(state.moves==state.performed&&state.performed<(int)std::min(FixturePlaces.size(),FixtureItems.size()));assert(pos.X==FixturePlaces[state.performed].pos.X);assert(movement==(state.performed==0?AI_MOVEMENT_RUNNING:AI_MOVEMENT_WALKING));}
  else{state.currentItem=ExpectedItem();assert(state.currentItem>=0);CThing*t=Objects[state.currentItem];int n=Nearest(*t);assert(pos.X==(n<0?StateOf(t).Pos.X:FixturePlaces[n].pos.X)&&pos.Y==0.0f&&pos.Z==0.0f&&movement==AI_MOVEMENT_WALKING);}
  nav=true;state.stage=1;++state.moves;if(!state.opening&&state.moves==state.dieMove){if(state.dieNull)Handles[state.currentItem]->Set(0);else Objects[state.currentItem]->Dead=true;}return state.moves==state.failMove?AI_ACTION_FAILED:AI_ACTION_FINISHED;
 }
 EAIActionReturn CAIStateGroupBase::AIAction_LookAtPos(EAIActionRepeat repeat,C3DVector pos,long frames,float speed){assert(!state.terminated&&state.stage==1&&repeat==ACTION_REPEAT_UNTIL_FINISHED&&frames==17&&speed==0.0027777f);if(state.opening)assert(pos.X==FixturePlaces[state.performed].pos.X);else{CThing*t=Objects[state.currentItem];int n=Nearest(*t);assert(pos.X==(n<0?StateOf(t).Pos.X:FixturePlaces[n].pos.X));}state.stage=2;++state.looks;if(!state.opening&&state.looks==state.dieLook){if(state.dieNull)Handles[state.currentItem]->Set(0);else Objects[state.currentItem]->Dead=true;}return state.looks==state.failLook?AI_ACTION_FAILED:AI_ACTION_FINISHED;}
 EAIActionReturn CAIStateGroupBase::AIAction_PerformActionAndWait(const CCreatureActionBase&a,bool wait){assert(!state.terminated&&state.stage==2&&!wait&&&a==CurrentAction&&ActionKind==state.opening);if(state.opening)assert(ActionItem==FixtureItems[state.performed]);else{assert(ActionItem==state.currentItem);state.expectedID=state.currentItem+1;}++state.performed;state.stage=0;return state.performed%2?AI_ACTION_FAILED:AI_ACTION_IN_PROGRESS;}
class FixtureKeeper:public CTCShopKeeper{public:FixtureKeeper(CThing&t):CTCShopKeeper(t){}ETCInterfaceType GetInterfaceType() const{abort();}CCharString GetClassTypeName() const{abort();}};
class FixtureOwner:public CTCThingOwner{public:FixtureOwner(CThing&t):CTCThingOwner(t){}ETCInterfaceType GetInterfaceType() const{abort();}CCharString GetClassTypeName() const{abort();}};
class FixtureCreated:public CTCCreatedEntity{public:FixtureCreated(CThing&t):CTCCreatedEntity(t){}ETCInterfaceType GetInterfaceType() const{abort();}CCharString GetClassTypeName() const{abort();}};
class FixtureStock:public CStockItemDef{public:FixtureStock(){++state.allocated;}void Delete()const{delete this;}~FixtureStock(){assert(IVRefCount==0);++state.freed;}void Transfer(CPersistContext&) {abort();}void Copy(const CDefClassBase*) {abort();}unsigned long GetSizeofClass() {abort();}};
class FixtureThingDef:public CThingBaseDef{public:bool Available;CSubDefInfo Info;FixtureThingDef(){Available=false;PDefManager=&Manager;Info.DefIndex=0;Info.OriginalParentDefIndex=0;}const CSubDefInfo*GetPSubDefInfo(const CCharString&n)const{assert(strcmp(n.GetAsCharArray(),"CStockItemDef")==0);++state.lookupCalls;return Available?&Info:0;}void Transfer(CPersistContext&) {abort();}void Copy(const CDefClassBase*) {abort();}unsigned long GetSizeofClass() {abort();}};

CDefPointer<CDefClassBase>CDefinitionManager::GetPDefFromGlobalIndex(long n){assert(n>=0&&n<8);if(DefMode==2){if(Borrowed)Borrowed->AddRef();return CDefPointer<CDefClassBase>(const_cast<CDefClassBase*>(static_cast<const CDefClassBase*>(Borrowed)));}return CDefPointer<CDefClassBase>(DefMode?new FixtureStock:0);}
template<class T>static void SeedVector(std::vector<T>&v,unsigned n){assert(!v._M_start);v._M_start=std::allocator<T>().allocate(n);v._M_finish=v._M_start;v._M_end_of_storage._M_data=v._M_start+n;}
static void AddTC(CThing&t,ETCInterfaceType id,CTCBase*p){if(!t.Components._M_start)SeedVector(static_cast<std::vector<std::pair<ETCInterfaceType,CTCBase*> >&>(t.Components),17);bool exists=false;for(unsigned k=0;k<t.Components.size();++k)if(t.Components[k].first==id){t.Components[k].second=p;exists=true;}if(!exists)t.Components.push_back(std::make_pair(id,p));for(unsigned k=1;k<t.Components.size();++k)for(unsigned j=k;j>0&&t.Components[j].first<t.Components[j-1].first;--j){std::pair<ETCInterfaceType,CTCBase*> tmp=t.Components[j];t.Components[j]=t.Components[j-1];t.Components[j-1]=tmp;}unsigned n=(unsigned)id;t.UsingComponents._M_w[n/(8*sizeof(unsigned long))]|=1UL<<(n%(8*sizeof(unsigned long)));}
static void InitThing(CThing&t,int id){StateOf(&t).ID=id;StateOf(&t).References=0;StateOf(&t).Enemy=false;memset(&t.UsingComponents,0,sizeof t.UsingComponents);t.Dead=false;t.ThingTypeFlags=8;}

void CBaseObject::Validate() const { abort(); }
void CClass::Save(CCharString&) { abort(); }
void CThingGameObject::OnHit(CHitParameters&) { abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { abort(); }
void CTCBase::OnSerialise(CPersistContext&, bool) { abort(); }
void CTCBase::OnRemove() { abort(); }
void CTCBase::ConstructFromParams(const CTCParamsBase&) { abort(); }
void CTCBase::OnCreate() { abort(); }
void CTCBase::InitialActivate() { abort(); }
void CTCBase::OnInitialActivate() { abort(); }
void CTCBase::RenderUpdate(float) { abort(); }
bool CTCBase::HasFrameUpdate() const { abort(); }
void CTCBase::FrameUpdate() { abort(); }
void CTCBase::BulletTimeFrameUpdate() { abort(); }
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
void CTCBase::Copy(const CTCBase&) { abort(); }
void CTCBase::OnDie() { abort(); }
void CTCBase::OnKill() { abort(); }
void CTCBase::OnLevelUnload() { abort(); }
void CTCBase::OnSetInLimbo(bool) { abort(); }
bool CTCBase::Validate(CCharString&) const { abort(); }
bool CTCBase::IsFreeForDeletion() const { abort(); }
bool CTCBase::IsSerialiseCreatable() const { abort(); }
bool CTCBase::IsSerialisableForHeroState() const { abort(); }
bool CTCBase::IsComponentSerialisable() const { abort(); }
bool CTCBase::IsDriver() const { abort(); }
bool CTCBase::IsCopyable() const { abort(); }
void CTCBase::GlobalUpdate(bool) { abort(); }
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&) { abort(); }
bool CResource::UnloadResource() { abort(); }
unsigned long CResource::GetUnloadableMemory() const { abort(); }
bool CResource::IsResourceLoaded() const { abort(); }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); }
void CResource::OnResourceLoadAborted() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); }
void CResource::ResetDebugMemoryInUse() { abort(); }
bool CResourceList::HandleFailedAllocation(unsigned long) { abort(); }
bool CDefClassBase::UnloadResource() { abort(); }
CDefString CDefClassBase::GetInstantiationName() const { abort(); }
void CDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CDefClassBase::IsParentDef() const { abort(); }
void CDefClassBase::PostTransfer() { abort(); }
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap() { abort(); }
CDefString CParentDefClassBase::GetInstantiationName() const { abort(); }
void CParentDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CParentDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CParentDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CParentDefClassBase::IsParentDef() const { abort(); }
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap() { abort(); }
void CClassFactory::AddNewClassType(const CClassTypeInit&) { abort(); }
void CClassFactory::OnCreateClass(CClass&) { abort(); }
void CClassFactory::OnDeleteClass(CClass&) { abort(); }
void CThingManager::OnCreateClass(CClass&) { abort(); }
void CThingManager::OnDeleteClass(CClass&) { abort(); }
void CGameDefinitionManager::InitAndCompile() { abort(); }
bool CMainGameComponent::Run(CGameComponent**) { abort(); }
void CMainGameComponent::ChangeTextureColourDepth(long) { abort(); }
void CMainGameComponent::OnPreDeviceReset() { abort(); }
bool CMainGameComponent::OnPostDeviceReset() { abort(); }
void CAIStateGroupBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name) { abort(); }
void CAIStateGroupBase::OnActivate() { abort(); }
bool CAIStateGroupBase::InitCreaturePosition() { abort(); }
bool CAIStateGroupBase::AllowPriorityOverlap() { abort(); }
bool CAIStateGroupBase::IsStillValid() const { abort(); }
const CCharString* CAIStateGroupBase::GetDebugText() const { abort(); }
bool CAIStateGroupBase::IsTimingCritical() const { abort(); }
EAIStateGroupType CAIStateGroupBase::GetStateGroupType() const { abort(); }
void CAIStateGroupBase::Cleanup() { abort(); }
void CAIStateGroupBase::OnReplace() { abort(); }
bool CAIStateGroupBase::IsRunnable() { abort(); }
void CAIStateGroupBase::InitStateGroup(const CWorld& world, CThingAICreature& creature) { abort(); }
bool CAIStateGroup_SetupWares::IsRunnable() { abort(); }
bool CAIStateGroup_SetupWares::InitCreaturePosition() { abort(); }
ETCInterfaceType CTCShop::GetInterfaceType() const { abort(); }
CCharString CTCShop::GetClassTypeName() const { abort(); }
bool CTCShop::HasFrameUpdate() const { abort(); }
void CTCShop::FrameUpdate() { abort(); }
void CTCShop::OnCreate() { abort(); }
void CTCShop::OnInitialActivate() { abort(); }
bool CTCShop::IsComponentSerialisable() const { abort(); }
void CTCShop::OnSerialise(CPersistContext&,bool) { abort(); }
ETCInterfaceType CTCCreatedEntity::GetInterfaceType() const { abort(); }
CCharString CTCCreatedEntity::GetClassTypeName() const { abort(); }
void CCreatureActionBase::EditUpdate(float) { abort(); }
void CCreatureActionBase::FinishAction() { abort(); }
bool CCreatureActionBase::IsPlayingAnimation() const { abort(); }
long CCreatureActionBase::GetAnimDelayTime() const { abort(); }
long CCreatureActionBase::GetAnimationIndex() const { abort(); }
long CCreatureActionBase::GetAnimationTransitionInTime() const { abort(); }
long CCreatureActionBase::GetAnimationTransitionOutTime() const { abort(); }
unsigned long CCreatureActionBase::GetAnimExtraFlags() const { abort(); }
void CCreatureActionBase::Initialise() { abort(); }
bool CCreatureActionBase::IsStrikeEvent(long) const { abort(); }
bool CCreatureActionBase::IsStrikeEventActive() const { abort(); }
bool CCreatureActionBase::IsStrikeAction() const { abort(); }
bool CCreatureActionBase::IsPartOfComboChain() const { abort(); }
bool CCreatureActionBase::IsStrikeEventFinishedBeingActive() const { abort(); }
bool CCreatureActionBase::IsVitalStrike() const { abort(); }
bool CCreatureActionBase::IsKnockdownStrike() const { abort(); }
bool CCreatureActionBase::IsBlockCounter() const { abort(); }
bool CCreatureActionBase::IsMeleeFlourish() const { abort(); }
bool CCreatureActionBase::IsActionWithStrikeEventHelperName() const { abort(); }
CCharString CCreatureActionBase::GetStrikeEventHelperName() const { abort(); }
bool CCreatureActionBase::IsOverrideableByActionOfSamePriority() const { abort(); }
void CCreatureActionBase::OnFrameUpdate(float) { abort(); }
bool CCreatureActionBase::IsQueuable() const { abort(); }
bool CCreatureActionBase::IsActionReadyForInterruption() const { abort(); }
bool CCreatureActionBase::IsActionReadyForInterruptionByMovement(const C3DVector &) const { abort(); }
bool CCreatureActionBase::IsBlockable() const { abort(); }
bool CCreatureActionBase::IsAffectedByHaste() const { abort(); }
bool CCreatureActionBase::IsAffectedByHeroStats() const { abort(); }
bool CCreatureActionBase::IsPhysicsPushableByHero() const { abort(); }
bool CCreatureActionBase::IsAlwaysPhysicsPushableBySelfAvoidance() const { abort(); }
bool CCreatureActionBase::IsAlwaysUseAnimationMovement() const { abort(); }
bool CCreatureActionBase::IsClearingMovementVector() const { abort(); }
bool CCreatureActionBase::IsToHaveEmptyHands() const { abort(); }
bool CCreatureActionBase::IsToReplaceItemIfRemoved() const { abort(); }
bool CCreatureActionBase::IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const { abort(); }
bool CCreatureActionBase::IsPreventingQuickAccessItemUsage() const { abort(); }
void CCreatureActionBase::SetHitter(const CThing &) { abort(); }
void CCreatureActionBase::SetHitDirection(C3DVector) { abort(); }
void CCreatureActionBase::SetHitPosition(C3DVector) { abort(); }
CTCWeaponTrail * CCreatureActionBase::GetWeaponTrailUsedInStrikeAction(long &) const { abort(); }
float CCreatureActionBase::GetTotalTimeSeconds() const { abort(); }
bool CCreatureActionBase::IsToEvadeAllHits() const { abort(); }
bool CCreatureActionBase::IsToCauseRecoil() const { abort(); }
bool CCreatureActionBase::IsAbleToBeAttacked() const { abort(); }
bool CCreatureActionBase::IsPlayHitResponse() const { abort(); }
bool CCreatureActionBase::IsPreventingNormalMovement() const { abort(); }
bool CCreatureActionBase::IsPreventingLookTurning() const { abort(); }
void CCreatureActionBase::OnBlowBlocked(bool) { abort(); }
void CCreatureActionBase::OnFollowUpActionFailedToAdd() { abort(); }
void CCreatureActionBase::UpdateActionRunningTime() { abort(); }
bool CCreatureActionBase::IsDecapitatingAttack() const { abort(); }
bool CCreatureActionBase::PreventSneaking() const { abort(); }
void CCreatureActionBase::FrameUpdate() { abort(); }
bool CCreatureActionBase::IsUsingAnimationMovement() const { abort(); }
bool CCreatureActionBase::IsUsingForcedMovement() const { abort(); }
bool CCreatureActionBase::IsMovementUsingPhysics() const { abort(); }
void CCreatureActionBase::UpdateActionMoveToPos() { abort(); }
void CCreatureActionBase::InitialiseActionMovement() { abort(); }
float CCreatureActionBase::GetFrameUpdateTime() const { abort(); }
float CCreatureActionBase::GetActionSpeedParameter() const { abort(); }
float CCreatureActionBase::GetActionStartRunningTimeSeconds() const { abort(); }
bool CCreatureActionBase::OverrideFaceWithDefault() const { abort(); }
bool CCreatureActionBase::AllowsCombinationAnims() const { abort(); }
bool CCreatureActionBase::CanAnimationTransitionOut() const { abort(); }
void CCreatureActionBase::OnAnimationLoaded() { abort(); }
bool CCreatureActionBase::WaitForMeshToLoad() const { abort(); }
void CCreatureAction_ShopkeeperPutAway::PerformAction() { abort(); }
CCreatureActionBase* CCreatureAction_ShopkeeperPutAway::Clone() const { abort(); }
void CCreatureAction_ShopkeeperGetOut::PerformAction() { abort(); }
CCreatureActionBase* CCreatureAction_ShopkeeperGetOut::Clone() const { abort(); }
int main(){CGame game;CMainGameComponentInit ci;CMainGameComponent component(game,ci);CWorld world;CWorldMap map;world.PWorldMap_ptr=&map;CGameDefinitionManager definitions;CPlayerManager players;CNavigatorManager navigation;unsigned long randomSeed=0;CThingManager things(component,definitions,world,players,navigation,&randomSeed,0,0,0);CThingAICreature creature;CreaturePtr=&creature;CAIBrain brain(component,world,things,players,creature,CDefPointer<const CBrainDef>());unsigned cases=0;int coverage[7]={0,0,0,0,0,0,0};
 for(int seed=0;seed<4096;++seed){memset(&state,0,sizeof(state));state.opening=seed%2;state.places=seed/2%4;state.items=seed/7%4;state.failMove=seed/11%5;state.failLook=seed/17%5;state.dieMove=seed/23%5;state.dieLook=seed/29%5;DefMode=seed/31%2;state.dieNull=seed/53%2!=0;
  CThing parent,objs[4];CTCShop shop(parent);Shop=&shop;FixtureOwner owner(parent);FixtureKeeper keeper(creature);FixtureCreated c0(objs[0]),c1(objs[1]),c2(objs[2]),c3(objs[3]);FixtureCreated*created[4]={&c0,&c1,&c2,&c3};FixtureThingDef defs[4];KeeperShop=seed/37%5==0?0:&shop;bool hasKeeper=seed/41%5!=0,hasOwner=seed/43%3!=0;InitThing(creature,10);InitThing(parent,11);creature.Components.clear();if(hasKeeper)AddTC(creature,TCI_SHOP_KEEPER,&keeper);if(hasOwner)AddTC(parent,TCI_THING_OWNER,&owner);FixturePlaces.clear();FixtureItems.clear();
  for(int n=0;n<state.places;++n){CTCShop::GoodsPlace p;memset(&p,0,sizeof(p));p.pos.X=(float)(n*4);FixturePlaces.push_back(p);}for(int n=0;n<state.items;++n)FixtureItems.push_back(20+n);
  for(int n=0;n<4;++n){CThing&t=objs[n];InitThing(t,n);StateOf(&t).Pos=C3DVector((float)(n*3),0,0);t.Dead=(seed/(n+2))%5==0;StateOf(&t).HasCreated=(seed/(n+3))%4!=0;StateOf(&t).HasDef=(seed/(n+5))%3!=0;StateOf(&t).Info=(seed/(n+7))%3!=0;defs[n].Available=StateOf(&t).Info;defs[n].Info.DefIndex=n;if(StateOf(&t).HasCreated)AddTC(t,TCI_CREATED_ENTITY,created[n]);if(StateOf(&t).HasDef)t.PBaseDef.Reset(&defs[n]);Objects[n]=seed/(n+11)%7==0?0:&t;owner.ObjectList.push_back(CIntelligentPointer<CThing>(Objects[n]));std::list<CIntelligentPointer<CThing> >::iterator last=owner.ObjectList.begin();for(int step=0;step<n;++step)++last;Handles[n]=&*last;}
  CAIStateGroup_SetupWares script;script.PBrain=&brain;script.Update();assert(state.terminated&&state.allocated==state.freed&&state.constructed==state.destroyed&&state.constructed==state.performed);
  bool valid=hasKeeper&&KeeperShop;if(!valid){assert(!state.stockWrites&&!state.moves&&!state.looks&&!state.performed);++coverage[0];}
  else{assert(state.stockWrites>=1&&state.stockWrites<=2&&state.performed<=state.looks&&state.looks<=state.moves);if(state.stockWrites==2){assert(state.stage==0);if(state.opening)assert(state.performed==(int)std::min(FixturePlaces.size(),FixtureItems.size()));else assert(ExpectedItem()<0||!hasOwner);++coverage[1];}else assert(state.stage!=0);if(state.opening&&state.performed)++coverage[2];if(!state.opening&&state.performed)++coverage[3];if(!state.opening&&state.dieMove&&state.moves==state.dieMove)++coverage[4];if(!DefMode&&state.lookupCalls)++coverage[5];if(state.places==0&&state.moves)++coverage[6];}
  owner.ObjectList.clear();for(int n=0;n<4;++n)objs[n].PBaseDef.Reset(0);
  ++cases;
 }
 assert(cases==4096u);for(int k=0;k<7;++k)assert(coverage[k]>0);

 for(int mode=0;mode<3;++mode)for(int info=0;info<2;++info)for(int previous=0;previous<2;++previous){memset(&state,0,sizeof state);DefMode=mode;FixtureThingDef base;base.Available=info!=0;base.Info.DefIndex=3;{CDefPointer<const CStockItemDef>out;if(previous)out.Object=new FixtureStock;const CDefPointeeBase*prior=out.Object;Borrowed=prior;bool ok=base.GetSubDef(&out);assert(ok==(info!=0));if(!info)assert(out.Object==prior);else assert((out.Object!=0)==(mode==1||(mode==2&&previous)));if(out.Object)assert(out.Object->IVRefCount==1);}assert(state.allocated==state.freed);}
 unsigned searches=0;for(int count=0;count<=32;++count){CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >map;SeedVector(static_cast<std::vector<std::pair<ETCInterfaceType,CTCBase*> >&>(map),33);for(int n=0;n<count;++n)map.push_back(std::make_pair((ETCInterfaceType)(n/2*3),(CTCBase*)0));for(int key=0;key<100;++key){ETCInterfaceType k=(ETCInterfaceType)key;int expected=0;while(expected<count&&map[expected].first<k)++expected;assert(map.LowerBound(k)==map.begin()+expected);++searches;}}assert(searches==3300u);
 puts("00927f30_TEST PASS");return 0;}