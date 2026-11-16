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

#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_TREE_INDEX
#define FABLE_RETAIL_CCHARSTRING_COMPARE_DECL

// 00d29880 NScript::CQ_EndGameScript::CHookCoastTeleporter::Main (assigned tail 00d2988b)
// Inline-budget: ordinary original members expand at T1000; scoped depth0 preserves native called constructors/destructors. No forced-inline member opt-in is used.
// Flags: /d2inlT1000
// Leaf: ?Main@CHookCoastTeleporter@CQ_EndGameScript@NScript@@EAEXXZ
// Retail: 0x00D29880, 1319 bytes, thiscall; authorized 0x00D2988B slice omits the proven eleven-byte head.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CRGBCOLOUR
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_EndGameScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_cutscene.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CRGBColour.h"
extern CGameScriptInterfaceBase* pDSTGame;
void FABLE_FASTCALL AddLogbookStoryEntry(long);
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer::CTimer() { TimerIndex=pDSTGame->RegisterTimer(); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time);return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time)const { return GetTime()==time; }
void NScript::CQ_EndGameScript::CHookCoastTeleporter::Main()
{
    TRetVoid _Fail_Val;
    bool PreviouslyKilledEnoughUndead=ParentClass.UndeadBar>=pGlobalScriptDef->EndGame_UpperBarLimit;
    if(ParentClass.UndeadBar<pGlobalScriptDef->EndGame_BarDecrementAmount) {
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
        Game.HeroReceiveMessageFromGuildMaster("TEXT_QST_025_MOTHER_INSTRUCTIONS_10","",true,true);
        Game.Pause(3.0f);
        Game.HeroReceiveMessageFromGuildMaster("TEXT_QST_025_MOTHER_INSTRUCTIONS_20","",true,true);
        Game.Pause(4.0f);
        ParentClass.ShowUndeadBar=true;
    }
    {
        CTimer DecrementTimer;
        DecrementTimer=1;
        for(;ParentClass.UndeadBar<pGlobalScriptDef->EndGame_UpperBarLimit;) {
            Game.NewScriptFrame();
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            if(DecrementTimer==0) {
                if(IsActiveThreadTerminating())return _Fail_Val.Return();
                if(ParentClass.UndeadBar<pGlobalScriptDef->EndGame_BarDecrementAmount) {
                    if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    ParentClass.UndeadBar=0;
                } else {
                    if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    ParentClass.UndeadBar-=pGlobalScriptDef->EndGame_BarDecrementAmount;
                }
                DecrementTimer=pGlobalScriptDef->EndGame_BarDecrementInterval;
            }
            Game.UpdateQuestInfoBar(ParentClass.GuiCounter,ParentClass.UndeadBar,-1.0f,-1.0f);
        }
        if(IsActiveThreadTerminating())goto Terminating;
        Game.RemoveQuestInfoElement(ParentClass.GuiCounter);
        Game.MiniMapRemoveMarker(Me);
        if(!PreviouslyKilledEnoughUndead) {
            if(IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
            {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                CScriptGameResourceObjectScriptedThingBase seh_hero;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                if(seh_hero.IsValid())seh_hero.Release();
                while(!Game.StartScriptingEntity(Game.GetHero(),seh_hero,(EScriptAIPriority)4)) {
                    Game.NewScriptFrame();
                    if(IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                        return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                    }
                }
                if(IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                }
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"]=seh_hero;
                {
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    RunCutsceneMacro("CS_GATEWAY_OPEN",mymap,0,0,false,true);
                }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            Game.FadeScreenOut(0.5f,0.0f,CRGBColour(0,0,0));
            Game.ActivateQuestWithoutLoadingResources("Q_EndGame_Barriers");
            Game.EntityTeleportToThing(Game.GetHero(),Game.GetThingWithScriptName("MG_EndGameTeleport"),false);
            AddLogbookStoryEntry(240);
        }
        while(true) {
            Game.NewScriptFrame();
            if(IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
        }
        if(IsActiveThreadTerminating()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        }
    }
    Terminating:
    return _Fail_Val.Return();
}

#include "engine/C3DVector.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CDefString.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CResourceList.h"

struct Event { int kind; unsigned value; };
struct TestState {
    int stop,checks,frames,finishFrame,failStarts,starts,registered,deregistered,setCalls,getCalls,updates;
    int messages,pauses,removed,markers,movies,liveHandles,allocated,freed,pauseStage,cutscenes,fades,quests,teleports,stories;
    long timer;
    float amount,upper,interval,initial,frameBar;
    float* bar;
    bool initialEnough,gotZero,resetAfterZero,finished;
    Event events[2048];int eventCount;
};
static TestState state;
static unsigned Bits(float f){unsigned x;assert(sizeof(x)==4&&sizeof(f)==4);memcpy(&x,&f,4);return x;}
static void EventAt(int k,unsigned v=0){assert(state.eventCount<2048);state.events[state.eventCount].kind=k;state.events[state.eventCount++].value=v;}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*s,long n){assert(n==-1);PStringData=new CCharStringData;PStringData->Data.PData=const_cast<char*>(s);PStringData->Data.StringLength=(unsigned long)strlen(s);PStringData->NoRefs=1;}
CCharString::CCharString(const CCharString&s):PStringData(s.PStringData){if(PStringData)++PStringData->NoRefs;}
CCharString::~CCharString(){if(PStringData&&--PStringData->NoRefs==0)delete PStringData;}
const CCharString&CCharString::operator=(const CCharString&s){if(PStringData!=s.PStringData){if(PStringData&&--PStringData->NoRefs==0)delete PStringData;PStringData=s.PStringData;if(PStringData)++PStringData->NoRefs;}return *this;}
const char CCharString::NullChar=0;

CWideString::CWideString():PStringData(0){}CWideString::~CWideString(){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){}CResource::~CResource(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CSubDefClassBase::CSubDefClassBase(){}CParentDefClassBase::CParentDefClassBase(){}CParentDefClassBase::~CParentDefClassBase(){}
CDefinitionManager::~CDefinitionManager(){}CFailedAllocationHandler::~CFailedAllocationHandler(){}CResourceList::~CResourceList(){}
void CIVCountedPointeeBase::Delete()const{abort();}


// Controlled external STLport tree operation: this script inserts exactly one HERO node.
namespace std {template<class Dummy>void _Rb_global<Dummy>::_Rebalance(_Rb_tree_node_base*x,_Rb_tree_node_base*&root){assert(x==root&&!x->_M_left&&!x->_M_right);x->_M_color=false;}}
static CBaseObject*LiveObjects[128];static int LiveKinds[128];static int LiveObjectCount;
static int&KindOf(const CBaseObject*p){for(int i=0;i<LiveObjectCount;++i)if(LiveObjects[i]==p)return LiveKinds[i];abort();}
static void Born(CBaseObject*p){assert(LiveObjectCount<128);LiveObjects[LiveObjectCount]=p;LiveKinds[LiveObjectCount++]=0;}
CBaseObject::CBaseObject():IntelligentPointer(0){Born(this);}CBaseObject::CBaseObject(const CBaseObject&o):IntelligentPointer(0){Born(this);KindOf(this)=KindOf(&o);}
CBaseObject::~CBaseObject(){for(int i=0;i<LiveObjectCount;++i)if(LiveObjects[i]==this){--LiveObjectCount;LiveObjects[i]=LiveObjects[LiveObjectCount];LiveKinds[i]=LiveKinds[LiveObjectCount];return;}assert(false);}
CBaseObject&CBaseObject::operator=(const CBaseObject&o){KindOf(this)=KindOf(&o);return *this;}
bool CCharString::operator<(const CCharString&o)const{return strcmp(GetAsCharArray(),o.GetAsCharArray())<0;}
CGameScriptInterfaceBase::~CGameScriptInterfaceBase(){}
CScriptThing::CScriptThing(){}CScriptThing::CScriptThing(const CScriptThing&o):PImp(o.PImp){}CScriptThing::~CScriptThing(){}CScriptThing&CScriptThing::operator=(const CScriptThing&o){PImp=o.PImp;return *this;}
CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase(){}
bool CScriptGameResourceObjectScriptedThingBase::IsValid()const{KindOf(this)=1;return !PImp.IsNull();}
void CScriptGameResourceObjectScriptedThingBase::Release(){assert(false);}
CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase(){if(PImp.Data){assert(state.pauseStage==2);--state.movies;}}
CScriptBase::CScriptBase(){}CScriptBase::~CScriptBase(){}CEntityScriptBase::~CEntityScriptBase(){}
namespace std {template<> _Vector_base<CHeroSoulsWaveDef,allocator<CHeroSoulsWaveDef> >::~_Vector_base(){assert(!_M_start);}template<> vector<CHeroSoulsWaveDef>::~vector(){assert(!_M_start&&!_M_finish);}template<> _Vector_base<CArenaWaveDef,allocator<CArenaWaveDef> >::~_Vector_base(){assert(!_M_start);}template<> vector<CArenaWaveDef>::~vector(){assert(!_M_start&&!_M_finish);}}
CScriptDef::CScriptDef(){}
CFeatDef::CFeatDef(){}CFeatDef::~CFeatDef(){}
CFireHeartPatternDef::CFireHeartPatternDef(){}CFireHeartPatternDef::~CFireHeartPatternDef(){}
CHeroSoulsRoundDef::CHeroSoulsRoundDef(){}CHeroSoulsRoundDef::~CHeroSoulsRoundDef(){}
CArenaRoundDef::CArenaRoundDef(){}CArenaRoundDef::~CArenaRoundDef(){}
CConversation::CConversation(){}
CConversation::~CConversation(){}
class FixtureThing:public CScriptThing{public:int Identity;FixtureThing(int n):Identity(n){}};
static CScriptThing MakeThing(int n){CScriptThing t;t.PImp.Reset(new FixtureThing(n));return t;}
static int Identity(const CScriptThing&t){assert(t.PImp.Data);return static_cast<FixtureThing*>(t.PImp.Data)->Identity;}
static CScriptThing Hero,Marker,Teleport;
class FixtureScriptedThing:public CScriptGameResourceObjectScriptedThingBase{public:int Kind;FixtureScriptedThing(int k):Kind(k){++state.allocated;}~FixtureScriptedThing(){++state.freed;}};
class FixtureMovie:public CScriptGameResourceObjectMovieBase{public:FixtureMovie(){++state.allocated;}~FixtureMovie(){++state.freed;}};
static int LiveHandles(){int n=0;for(int i=0;i<LiveObjectCount;++i)if(LiveKinds[i]==1)++n;return n;}
static CScriptGameResourceObjectScriptedThingBase*ActiveImplementation;
static CScriptDef Definition;
CDefPointer<const CScriptDef> pGlobalScriptDef;
class FixtureInterface:public CGameScriptInterfaceBase {public:
void Error(const CCharString&, const CCharString&, const CCharString&, unsigned long) const{abort();}
void TraceMessage(const CCharString&) const{abort();}
void Validate() const{abort();}
void SetDebugCameraType(long) const{abort();}
void DeactivateBoastUi() const{abort();}
bool IsXbox() const{abort();}
void NewScriptFrame() const{++state.frames;if(state.timer>0)--state.timer;if(state.frames==state.finishFrame)*state.bar=state.upper;state.frameBar=*state.bar;state.gotZero=false;state.resetAfterZero=false;EventAt(1,Bits(*state.bar));}
bool StartScriptingEntity(const CScriptThing& h,CScriptGameResourceObjectScriptedThingBase& x,EScriptAIPriority p){
        assert(!state.initialEnough&&state.markers==1&&Identity(h)==1&&p==4&&!x.IsValid());++state.starts;EventAt(9,state.starts);
        if(state.starts<=state.failStarts)return false;x.PImp.Reset(new FixtureScriptedThing(7));ActiveImplementation=x.PImp.Data;return true;
    }
bool IsEntityUnderScriptedControl(const CScriptThing&) const{abort();}
bool IsActiveThreadTerminating() const{abort();}
bool IsLevelLoaded(const CCharString&) const{abort();}
bool IsRegionLoaded(const CCharString&) const{abort();}
bool IsRegionLoadedAndPreloaded(const CCharString&) const{abort();}
bool IsRegionDefLoaded(const CCharString&) const{abort();}
const CCharString& GetRegionName() const{abort();}
bool MsgIsLevelLoaded(const CCharString&) const{abort();}
bool MsgIsLevelUnloaded(const CCharString&) const{abort();}
bool MsgOnLevelLoaded(std::list<CCharString,std::allocator<CCharString> >&) const{abort();}
bool MsgOnLevelUnloaded(std::list<CCharString,std::allocator<CCharString> >&) const{abort();}
bool MsgIsRegionLoaded(const CCharString&) const{abort();}
bool MsgIsRegionUnloaded(const CCharString&) const{abort();}
bool MsgOnRegionLoaded(CCharString&) const{abort();}
bool MsgOnRegionUnloaded(CCharString&) const{abort();}
bool MsgOnRegionPreunload(CCharString&) const{abort();}
bool MsgOnQuestCompleted(const CCharString&) const{abort();}
bool MsgOnAnyQuestCompleted(CCharString&) const{abort();}
bool MsgOnQuestFailed(const CCharString&) const{abort();}
bool MsgOnQuestCompletedBeforeScreenShown(const CCharString&) const{abort();}
bool MsgOnQuestFailedBeforeScreenShown(const CCharString&) const{abort();}
bool MsgOnQuestAccepted(CCharString&) const{abort();}
bool MsgOnFeatAccepted(long&) const{abort();}
bool MsgIsBoastMade(long) const{abort();}
bool MsgOnBoastMade(long&, CCharString&) const{abort();}
bool MsgOnBoastsMade(std::vector<std::pair<long,CCharString>,std::allocator<std::pair<long,CCharString> > >&) const{abort();}
void RemoveBoastMessage() const{abort();}
bool IsQuestStartScreenActive(CCharString&) const{abort();}
bool MsgOnLeavingQuestStartScreen(CCharString&) const{abort();}
bool MsgOnLeavingExperienceSpendingScreen() const{abort();}
long MsgIsQuestionAnsweredYesOrNo() const{abort();}
bool MsgIsGameInfoClickedPast() const{abort();}
bool MsgIsTutorialClickedPast() const{abort();}
bool MsgIsActionModeButtonPressed() const{abort();}
bool MsgOnExpressionPerformed(CCharString&) const{abort();}
bool MsgIsCutSceneSkipped() const{abort();}
void RemoveAllCutSceneSkippedMessages() const{abort();}
bool MsgOnHeroHairTypeChanged(EClothingCoversArea, CCharString&) const{abort();}
bool MsgOnHeroUsedTeleporter(CCharString&) const{abort();}
bool MsgOnHeroUsedGuildSeal() const{abort();}
bool MsgOnGameSavedManually() const{abort();}
bool MsgOnHeroSlept(bool&) const{abort();}
bool MsgOnHeroFiredRangedWeapon() const{abort();}
bool MsgOnHeroCastSpell(EHeroAbility&) const{abort();}
bool MsgOnHeroPickedPocket(CScriptThing&) const{abort();}
bool MsgOnHeroPickedLock(CScriptThing&) const{abort();}
bool MsgOnFishingGameFinished(CScriptThing&) const{abort();}
bool MsgOnTavernGameFinished(const CScriptThing&) const{abort();}
bool MsgOnHeroRewardedWithItemsFrom(CCharString&) const{abort();}
bool MsgOnChestOpeningCancelled() const{abort();}
void DontPopulateNextLoadedRegion() const{abort();}
void GetWanderingPopulationScriptDefNameInCurrentRegion(CCharString&) const{abort();}
void GetWanderingPopulationScriptDefNameInRegion(const CCharString&, CCharString&) const{abort();}
bool IsHeroAllowedHenchmenInCurrentRegion() const{abort();}
bool IsHeroAllowedHenchmenInRegion(const CCharString&) const{abort();}
void PostAddScriptedEntities() const{abort();}
bool IsPlayerHoldingUnsheatheRangedWeaponButton() const{abort();}
bool IsPlayerHoldingLockTargetButton() const{abort();}
bool IsPlayerHoldingFireRangedWeaponButton() const{abort();}
bool IsPlayerHoldingFirstPersonTargetingButton() const{abort();}
bool IsHeroInProjectileWeaponMode() const{abort();}
CScriptThing& GetHero(){return Hero;}
CScriptThing GetHeroTargetedThing() const{abort();}
CScriptThing GetThingWithScriptName(const CCharString&, const CCharString&) const{abort();}
CScriptThing GetThingWithScriptName(const CCharString& n) const{assert(strcmp(n.GetAsCharArray(),"MG_EndGameTeleport")==0&&state.quests==1);return Teleport;}
CScriptThing GetRandomThingWithScriptName(const CCharString&) const{abort();}
long GetAllThingsWithScriptName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const{abort();}
long GetAllCreaturesInAreaWithScriptName(const CCharString&, const C3DVector&, float, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const{abort();}
CScriptThing GetNearestWithScriptName(const CScriptThing&, const CCharString&) const{abort();}
CScriptThing GetFurthestWithScriptName(const CScriptThing&, const CCharString&) const{abort();}
long GetAllThingsWithDefNameByDistanceFrom(const CScriptThing&, const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const{abort();}
long GetAllThingsWithDefName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const{abort();}
CScriptThing GetNearestWithDefName(const CScriptThing&, const CCharString&) const{abort();}
CScriptThing GetFurthestWithDefName(const CScriptThing&, const CCharString&) const{abort();}
CScriptThing GetThingWithUID(unsigned long long) const{abort();}
long GetAllCreaturesExcludingHero(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const{abort();}
long GetAllThingsInLevel(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const{abort();}
bool IsThingWithThisUIDAlive(unsigned long long) const{abort();}
long RegisterTimer(){assert(!state.registered);++state.registered;EventAt(2);return 19;}
void DeregisterTimer(long i){assert(i==19&&state.registered==1&&!state.deregistered);++state.deregistered;}
void SetTimer(long i,long t){assert(i==19);if(!state.setCalls)assert(t==1);else{assert(state.gotZero&&t==(long)state.interval);state.resetAfterZero=true;}++state.setCalls;state.timer=t;EventAt(3,(unsigned)t);}
long GetTimer(long i) const{assert(i==19);++state.getCalls;state.gotZero=state.timer==0;return state.timer;}
CScriptThing CreateCreature(const CCharString&, const C3DVector&, const CCharString&, bool) const{abort();}
CScriptThing CreateCreatureNearby(const CCharString&, const C3DVector&, float, const CCharString&, bool) const{abort();}
CScriptThing CreateCreatureOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const{abort();}
CScriptThing TurnCreatureInto(const CScriptThing&, const CCharString&) const{abort();}
void SetCreatureCreationDelayFrames(long) {abort();}
void ResetCreatureCreationDelayFrames() {abort();}
CScriptThing CreateObject(const CCharString&, const C3DVector&, const CCharString&) const{abort();}
CScriptThing CreateObject(const CCharString&, const C3DVector&, float, const CCharString&) const{abort();}
CScriptThing CreateObjectOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const{abort();}
CScriptThing CreateEffect(const CCharString&, const CScriptThing&, const CCharString&, const CCharString&, bool, bool) const{abort();}
CScriptThing CreateEffect(const CCharString&, const C3DVector&, const CCharString&, float, bool, bool) const{abort();}
CScriptThing CreateLight(const C3DVector&, const CRGBColour&, const CCharString&, float, float, bool) const{abort();}
CScriptThing CreateExperienceOrb(const C3DVector&, long) const{abort();}
CScriptThing CreateExplosion(const CCharString&, const C3DVector&, const C3DVector*, CCharString) const{abort();}
CScriptThing CreatePhysicalBarrier(float, const C3DVector&, const C3DVector*, CCharString) const{abort();}
CScriptThing CreateRumble(const C3DVector&, float, float, CCharString) const{abort();}
void ClearAllRumbles() const{abort();}
void RemoveThing(const CScriptThing&, bool, bool) const{abort();}
void ShowOnScreenMessage(const CCharString&, float) const{abort();}
void ShowOnScreenMessage(const CCharString&, const CCharString&, const CCharString&, float) const{abort();}
void ShowOnScreenMessage(const C2DVector&, const CCharString&, const CRGBColour&, const CCharString&) const{abort();}
void AddScreenMessage(const CCharString&, ETextGroupSelectionMethod) const{abort();}
void AddScreenTitleMessage(const CCharString&, float, bool) const{abort();}
void GiveHeroYesNoQuestion(const CCharString&, const CCharString&, const CCharString&, const CCharString&, bool) const{abort();}
void DisplayGameInfo(const CCharString&) const{abort();}
void DisplayGameInfoText(const CWideString&) const{abort();}
bool IsSafeToDisplayGameInfo() const{abort();}
bool DisplayTutorial(ETutorialCategory) const{abort();}
bool IsTutorialSystemEnabled() const{abort();}
void GiveHeroWeapon(const CCharString&, bool) const{abort();}
void GiveHeroObject(const CCharString&, long, bool) const{abort();}
void SetWeaponAsHerosActiveWeapon(const CCharString&) const{abort();}
void GiveHeroItem(const CScriptThing&) const{abort();}
bool GiveHeroItemsFromContainer(const CScriptThing&, bool) const{abort();}
void TakeObjectFromHero(const CCharString&) const{abort();}
void GiveHeroGold(long) const{abort();}
long GetHeroGold() const{abort();}
void GiveHeroExperience(long) const{abort();}
void SetHeroAbleToGainExperience(bool) const{abort();}
void SheatheHeroWeapons() const{abort();}
void SetHeroWillAsUsable(bool) const{abort();}
void SetHeroWeaponsAsUsable(bool) const{abort();}
void SetWeaponOutCrimeEnabled(bool) const{abort();}
void SetGuardsIgnoreCrimes(bool) const{abort();}
void RemoveAllHeroWeapons() const{abort();}
bool IsReportedOrUnreportedCrimeKnown(const CScriptThing&) const{abort();}
void ConfiscateAllHeroItems() const{abort();}
void ConfiscateAllHeroWeapons() const{abort();}
void ConfiscateItemsOfTypeFromHero(const CCharString&) const{abort();}
void ReturnAllConfiscatedItemsToHero() const{abort();}
bool GiveHeroTutorial(ETutorialCategory) const{abort();}
void MakeHeroCarryItemInHand(const CCharString&) const{abort();}
void MakeHeroCarryItemInHand(const CScriptThing&, bool, bool) const{abort();}
void AddTattooToHero(const CCharString&) const{abort();}
void GiveHeroAbility(EHeroAbility, bool) const{abort();}
bool IsPlayerZTargetingThing(const CScriptThing&) const{abort();}
bool IsPlayerCreatureBlocking() const{abort();}
bool IsPlayerCreatureReadyToFireProjectileWeapon(float&) const{abort();}
long GetPlayerCreatureCombatMultiplier() const{abort();}
long GetPlayerCreatureCombatMultiplierRunningNumHits() const{abort();}
void ResetPlayerCreatureCombatMultiplier() const{abort();}
bool IsPlayerCreatureFlourishEnabled() const{abort();}
void SetPlayerCreatureOnlyTarget(const CScriptThing&) const{abort();}
void ResetPlayerCreatureOnlyTarget() const{abort();}
void RespawnHero(bool) const{abort();}
void GiveHeroMorality(float) const{abort();}
float GetHeroMorality() const{abort();}
EMorality GetHeroMoralityCategory() const{abort();}
void ChangeHeroMoralityDueToTheft() const{abort();}
void ChangeHeroMoralityDueToPicklock() const{abort();}
void GiveHeroRenownPoints(long) const{abort();}
long GetHeroRenownLevel() const{abort();}
bool IsHeroRenownLevelFull() const{abort();}
void IncreaseHeroRenownLevel() const{abort();}
long GetHeroStrengthLevel() const{abort();}
long GetHeroSkillLevel() const{abort();}
long GetHeroWillLevel() const{abort();}
long GetHeroStatLevel(EHeroTrainableStatType) const{abort();}
long GetHeroStatMax(EHeroTrainableStatType) const{abort();}
void SetHeroAge(float) const{abort();}
float GetHeroAge() const{abort();}
void SetHeroAsTeenager(bool) const{abort();}
void SetHeroAsApprentice(bool) const{abort();}
float GetDistanceHeroCanBeHeardFrom() const{abort();}
long GetHeroRoughExperienceLevel() const{abort();}
long GetHeroExperienceAvailableToSpend() const{abort();}
float GetHeroFatness() const{abort();}
float GetHeroScariness() const{abort();}
float GetHeroAttractiveness() const{abort();}
float GetHeroWillEnergyLevel() const{abort();}
void SetHeroWillEnergyLevel(float) const{abort();}
void SetHeroWillEnergyAsAbleToRefill(bool) const{abort();}
long GetNumberOfItemsOfTypeInInventory(const CCharString&) const{abort();}
bool IsObjectInThingsPossession(const CCharString&, const CScriptThing&) const{abort();}
bool IsHeroHandLampLit() const{abort();}
void SetHeroHandLampAsLit(bool) const{abort();}
bool IsWearingClothingItem(const CScriptThing&, const CCharString&) const{abort();}
bool IsHeroNaked() const{abort();}
void RemoveHeroClothing() const{abort();}
void SetHeroAsWearing(const CCharString&) const{abort();}
void ChangeHeroHairstyle(const CCharString&) const{abort();}
void RemoveHeroHairstyle() const{abort();}
bool IsWearingHairstyle(CScriptThing&, const CCharString&) const{abort();}
bool IsPlayerCarryingItemOfType(const CCharString&) const{abort();}
bool IsPlayerWieldingWeapon() const{abort();}
bool IsEntityWieldingWeapon(const CScriptThing&) const{abort();}
bool IsEntityWieldingMeleeWeapon(const CScriptThing&) const{abort();}
bool IsEntityWieldingRangedWeapon(const CScriptThing&) const{abort();}
bool GetPreviouslyWieldedMeleeWeaponName(CCharString&) const{abort();}
bool GetPreviouslyWieldedRangedWeaponName(CCharString&) const{abort();}
bool IsEntityAbleToAttack(const CScriptThing&) const{abort();}
CScriptThing EntityGetThingInPrimarySlot(const CScriptThing&) const{abort();}
void ApplyHeroPenaltyForDeath() const{abort();}
void GiveHeroTitle(const CCharString&) const{abort();}
EHeroTitle GetHeroTitle() const{abort();}
void EntitySetAsMarryable(const CScriptThing&, bool) const{abort();}
void EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing&, bool) const{abort();}
void EntityForceMarriageToHero(const CScriptThing&, bool) const{abort();}
bool IsEntityMarriedToHero(const CScriptThing&) const{abort();}
bool IsEntityMarriable(const CScriptThing&) const{abort();}
bool GetHeroHasMarried() const{abort();}
bool GetHeroHasCurrentMarriage() const{abort();}
bool GetHeroHasDivorcedMarriage() const{abort();}
bool GetHeroHasChildren() const{abort();}
bool GetHeroHasMurderedWife() const{abort();}
bool IsHeroChild() const{abort();}
void CancelHeroTeleportEffects() const{abort();}
long GetNumberOfTimesHeroHasHadSex() const{abort();}
void SetNumberOfTimesHeroHasHadSex(long) const{abort();}
void SetHeroAsHavingHadSex(bool) const{abort();}
void SetHeroAsHavingHadGaySex(bool) const{abort();}
void GiveThingHeroRewardItem(const CScriptThing&, const CCharString&, const CCharString&) const{abort();}
void GiveThingItemInHand(const CScriptThing&, const CCharString&, bool) const{abort();}
void GiveThingItemInSlot(const CScriptThing&, const CCharString&, const CCharString&) const{abort();}
void GiveHeroExpression(const CCharString&, long, bool) const{abort();}
bool HeroHasExpression(const CCharString&) const{abort();}
bool IsHeroPerformingExpression(const CCharString&) const{abort();}
bool IsDPadButtonHeldForExpression(const CCharString&) const{abort();}
void EntityFollowThing(const CScriptThing&, const CScriptThing&, float, bool) const{abort();}
void EntityStopFollowing(const CScriptThing&) const{abort();}
bool GetFollowingEntityList(const CScriptThing&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const{abort();}
bool GetHeroSummonedCreaturesList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const{abort();}
bool GetPerceivingHeroEntityList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const{abort();}
bool IsEntityFollowingHero(const CScriptThing&) const{abort();}
void EntitySetAsAllowedToFollowHero(const CScriptThing&, bool) const{abort();}
void EntitySetAsAllowedToChangeRegionFollowingState(const CScriptThing&, bool) const{abort();}
void EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing&, bool) const{abort();}
void EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(const CScriptThing&, bool) const{abort();}
void TeleportAllFollowersToHeroPosition() const{abort();}
void EntityTeleportToHeroPosition(const CScriptThing&) const{abort();}
void SendEntityEvent(NEntityEvents::EEventType, const CScriptThing&, const CScriptThing&, CThing*) const{abort();}
float GetWaterHeightAtPosition(const C3DVector&) const{abort();}
bool IsFishingSpotEnabled(const CScriptThing&) const{abort();}
void DisableFishingSpot(const CScriptThing&) const{abort();}
void UpdateFishWeight(const CScriptThing&) const{abort();}
float GetBestFishWeight(const CCharString&) const{abort();}
void HeroGoFishing(bool) const{abort();}
long GetHeroFishingLevel() const{abort();}
void HeroGoDigging() const{abort();}
void HeroStopDigging() const{abort();}
void HeroPlayOracleMinigame() const{abort();}
bool IsHeroPlayingOracleMinigame() const{abort();}
bool HasHeroWonOracleMinigame() const{abort();}
void HeroPlayFireheartMinigame() const{abort();}
void HeroQuitFireheartMinigame() const{abort();}
bool HasHeroForceQuitFireheartMinigame() const{abort();}
float GetHeroHealth() const{abort();}
float GetHeroHealthMax() const{abort();}
float GetHeroHealthPercentage() const{abort();}
long GetHeroWillEnergy() const{abort();}
long GetHeroWillEnergyMax() const{abort();}
long GetHeroWillEnergyPercentage() const{abort();}
void ChangeHeroHealthBy(float, bool, bool) const{abort();}
float GetHealth(const CScriptThing&) const{abort();}
void ModifyThingHealth(const CScriptThing&, float, bool) const{abort();}
void EntitySetMaxHealth(const CScriptThing&, float, bool) const{abort();}
void SetThingAsKilled(const CScriptThing&) const{abort();}
void GiveHeroNewQuestObjective(const CCharString&, unsigned long) const{abort();}
void TellHeroQuestObjectiveCompleted(unsigned long) const{abort();}
void TellHeroQuestObjectiveFailed(unsigned long) const{abort();}
void AddQuestRegion(const CCharString&, const CCharString&) const{abort();}
void SetQuestWorldMapOffset(const CCharString&, const C2DCoordI&) const{abort();}
bool IsHeroOnQuest() const{abort();}
void HeroReceiveMessageFromGuildMaster(const CCharString& s,const CCharString& empty,bool a,bool b) const{
        assert(state.initial<state.amount&&empty.GetLength()==0&&a&&b);
        const char* names[]={"TEXT_QST_025_MOTHER_INSTRUCTIONS_10","TEXT_QST_025_MOTHER_INSTRUCTIONS_20"};
        assert(state.messages<2&&strcmp(s.GetAsCharArray(),names[state.messages])==0);assert(state.pauses==state.messages);++state.messages;EventAt(4,state.messages);
    }
void SetGuildMasterMessages(bool) const{abort();}
void ActivateQuest(const CCharString&) const{abort();}
void ActivateMultipleQuests(const CArray<CCharString>&) const{abort();}
void ActivateQuestWithoutLoadingResources(const CCharString& n) const{assert(strcmp(n.GetAsCharArray(),"Q_EndGame_Barriers")==0&&state.fades==1);++state.quests;EventAt(14);}
void ActivateMultipleQuestsWithoutLoadingResources(const CArray<CCharString>&) const{abort();}
void DeactivateQuest(const CCharString&, unsigned long) const{abort();}
void DeactivateQuestLater(const CCharString&, unsigned long) const{abort();}
void PrepareQuestsWhenFinalQuestIsActivated(const CCharString&) const{abort();}
void PrepareQuestsWhenFinalQuestIsCompleted() const{abort();}
bool IsQuestActive(const CCharString&) const{abort();}
bool IsQuestRegistered(const CCharString&) const{abort();}
bool IsQuestCompleted(const CCharString&) const{abort();}
bool IsQuestFailed(const CCharString&) const{abort();}
void SetQuestAsCompleted(const CCharString&, bool, bool, bool) const{abort();}
void SetQuestAsFailed(const CCharString&, bool, const CWideString&, bool) const{abort();}
void SetQuestAsPersistent(const CCharString&, bool) const{abort();}
const CCharString& GetExclusiveQuestScriptName() const{abort();}
void AddQuestCard(const CCharString&, const CCharString&, bool, bool) const{abort();}
void RemoveQuestCardFromGuild(const CCharString&) const{abort();}
void RemoveQuestCardFromHero(const CCharString&) const{abort();}
void GiveHeroQuestCardDirectly(const CCharString&, const CCharString&, bool) const{abort();}
void SetQuestCardObjective(const CCharString&, const CCharString&, const CCharString&, const CCharString&) const{abort();}
void SetQuestCardGoldReward(const CCharString&, long) const{abort();}
void SetQuestCardRenownReward(const CCharString&, long) const{abort();}
void RemoveAllAvailableQuestCardsFromGuild() const{abort();}
void FailAllActiveQuests() const{abort();}
void GetAllActiveQuestInfo(std::vector<CCharString,std::allocator<CCharString> >&, std::vector<CCharString,std::allocator<CCharString> >&) const{abort();}
void AddFeatCard(long, const CCharString&, const CCharString&) const{abort();}
void AddBoast(const CCharString&, long, long, long, bool, const CCharString&, long) const{abort();}
void RemoveBoast(long, const CCharString&) const{abort();}
void SetBoastAsFailed(long, const CCharString&) const{abort();}
void SetBoastAsCompleted(long, const CCharString&) const{abort();}
bool IsBoastTaken(long, const CCharString&) const{abort();}
void AddLogBookEntry(const CWideString&, const CWideString&, const CWideString&, long) const{abort();}
void KickOffQuestStartScreen(const CCharString&, bool, bool) const{abort();}
void KickOffDeathScreen() const{abort();}
void KickOffCreditsScreen(CCharString&) const{abort();}
void SetPreferredQuickAccessItem(const CCharString&, long, long) const{abort();}
CCharString GetDeathRecoveryMarkerName() const{abort();}
void SetDeathRecoveryMarkerName(const CCharString&) const{abort();}
void ResetDeathRecoveryMarkerNameToDefault() const{abort();}
bool IsToFailQuestOnDeath() const{abort();}
void SetWhetherToFailQuestOnDeath(bool) const{abort();}
void ResetWhetherToFailQuestOnDeathToDefault() const{abort();}
CScriptThing GetMostRecentValidUsedTarget() const{abort();}
CCharString GetMostRecentValidUsedTargetName() const{abort();}
void DisplayQuestInfo(bool) const{abort();}
void SetQuestInfoName(const char*) const{abort();}
void SetQuestInfoText(const char*) const{abort();}
long AddQuestInfoBar(float, float, const CRGBColour&, const CRGBColour&, const CCharString&, const CCharString&, float) const{abort();}
long AddQuestInfoBarHealth(const CScriptThing&, const CRGBColour&, const CCharString&, float) const{abort();}
long AddQuestInfoTimer(const CTimer&, const CCharString&, float) const{abort();}
long AddQuestInfoCounter(const CCounter&, const CCharString&, float) const{abort();}
long AddQuestInfoCounter(const CCharString&, long, float) const{abort();}
long AddQuestInfoCounterList(const CCharString&, long, float) const{abort();}
long AddQuestInfoTick(const CCharString&, bool, float) const{abort();}
long AddQuestInfoTick(EGameAction, bool, float) const{abort();}
void UpdateQuestInfoBar(long i,float v,float a,float b) const{
        assert(i==71&&a==-1.0f&&b==-1.0f);float expected=state.frameBar;
        if(state.gotZero){expected=expected<state.amount?0.0f:expected-state.amount;assert(state.resetAfterZero);}else assert(!state.resetAfterZero);
        assert(Bits(v)==Bits(expected)&&Bits(*state.bar)==Bits(expected));if(!(v<state.upper))state.finished=true;++state.updates;EventAt(6,Bits(v));
    }
void ChangeQuestInfoBarColour(long, const CRGBColour&, const CRGBColour&) const{abort();}
void UpdateQuestInfoTimer(long, float) const{abort();}
void UpdateQuestInfoCounter(long, long, long) const{abort();}
void UpdateQuestInfoCounterList(long, long, long) const{abort();}
void UpdateQuestInfoTick(long, bool) const{abort();}
void RemoveQuestInfoElement(long i) const{assert(i==71&&!(*state.bar<state.upper)&&!state.removed);++state.removed;EventAt(7);}
void RemoveAllQuestInfoElements() const{abort();}
void DisplayTime(bool) const{abort();}
void DisplayMoneyBag(bool) const{abort();}
void DisplayMiniGameInfo(bool, EMiniGameType) const{abort();}
void UpdateMiniGameInfoBar(float) const{abort();}
bool IsEntityPickPocketable(const CScriptThing&) const{abort();}
bool IsEntityPickLockable(const CScriptThing&) const{abort();}
bool IsEntityStealable(const CScriptThing&) const{abort();}
void EntitySetAsPickPocketed(const CScriptThing&) const{abort();}
void EntitySetAsPickLocked(const CScriptThing&) const{abort();}
void EntitySetAsStolen(const CScriptThing&) const{abort();}
void MiniMapAddMarker(const CScriptThing&, const CCharString&) const{abort();}
void MiniMapSetMarkerGraphic(const CScriptThing&, const CCharString&) const{abort();}
void MiniMapRemoveMarker(const CScriptThing& x) const{assert(Identity(x)==2&&state.removed==1&&!state.markers);++state.markers;EventAt(8);}
void MiniMapRemoveAllMarkers() const{abort();}
void MiniMapAllowRouteBetweenRegions(const CCharString&, const CCharString&, bool) const{abort();}
void MiniMapSetAsEnabled(bool) const{abort();}
void EntitySetAsHiddenOnMiniMap(const CScriptThing&, bool) const{abort();}
void SetHUDEnabled(bool) const{abort();}
bool TextEntryExists(const CCharString&) const{abort();}
CCharString GetValidTextEntryNameWithAttitude(const CScriptThing&, const CCharString&) const{abort();}
void SetThingHasInformation(const CScriptThing&, bool, bool, bool) const{abort();}
void ClearThingHasInformation(const CScriptThing&) const{abort();}
void EntitySetWillBeUsingNarrator(const CScriptThing&, const CCharString&) const{abort();}
void EntityResetAsPureAINarrator(const CScriptThing&) const{abort();}
long AddNewConversation(const CScriptThing&, bool, bool) const{abort();}
void AddPersonToConversation(long, const CScriptThing&) const{abort();}
void AddLineToConversation(long, const CCharString&, bool, const CScriptThing&, const CScriptThing&) const{abort();}
void RemoveConversation(long, bool) const{abort();}
bool IsConversationActive(long) const{abort();}
void PlayAVIMovie(const CCharString&) const{abort();}
void StartMovieSequence(const CCharString& n,CScriptGameResourceObjectMovieBase& m){assert(n.GetLength()==0&&!state.movies&&LiveHandles()>=2);m.PImp.Reset(new FixtureMovie);++state.movies;EventAt(10);}
void FixMovieSequenceCamera(bool) const{abort();}
void FadeScreenOutUntilNextCallToFadeScreenIn(float, float) {abort();}
bool FadeScreenOut(float a,float b,CRGBColour c){assert(a==0.5f&&b==0.0f&&c.R==0&&c.G==0&&c.B==0&&c.A==255);assert(!state.movies&&!LiveHandles()&&state.allocated==state.freed&&state.cutscenes==1);++state.fades;EventAt(13);return true;}
void FadeScreenIn() {abort();}
bool IsScreenFadingOut() const{abort();}
void Pause(float s) const{assert(state.messages==state.pauses+1);assert(s==(state.pauses?4.0f:3.0f));++state.pauses;EventAt(5,Bits(s));}
void EndCutFade() {abort();}
void EndLetterBox() {abort();}
void PauseAllNonScriptedEntities(bool p){if(p){assert(state.pauseStage==0&&state.movies==1);state.pauseStage=1;}else{assert(state.pauseStage==1&&state.cutscenes==1);state.pauseStage=2;}EventAt(11,p?1:0);}
void PauseAllEntities(bool) {abort();}
void SetAllowScreenFadingOnNextRegionChange(bool) {abort();}
void SetAllowScreenFadingIfAlreadyFaded(bool) {abort();}
void SetToKeepHeroAbilitiesDuringCutscenes(bool) const{abort();}
void SetToDisplayTutorialsDuringCutscenes(bool) const{abort();}
void SetCutsceneMode(bool, bool) const{abort();}
bool IsInCutscene() const{abort();}
void SetCutsceneSkippable(bool) const{abort();}
void SetCutsceneSkippableWhilePaused(bool) const{abort();}
void SetCutsceneActionMode(bool, const CCharString&) const{abort();}
void PreloadNewScene(float) const{abort();}
void StartProgressDisplay() const{abort();}
void StopProgressDisplay() const{abort();}
void SetScreenMessagesEnabled(bool) const{abort();}
bool IsHeroControlledByPlayer() const{abort();}
bool IsInMovieSequence() const{abort();}
void CancelAbilitiesForCutscene() const{abort();}
void ResumeAbilitiesForCutscene() const{abort();}
void CancelUsingAbility(EHeroAbility) const{abort();}
void SetAbilityAvailability(EHeroAbility, bool) const{abort();}
void SetEnvironmentalEffectsAlwaysUpdate(bool) const{abort();}
void SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool) const{abort();}
void RemoveDeadCreature(const CCharString&) const{abort();}
void CameraSetCameraPreloadFlag(bool) const{abort();}
void CameraCircleAroundThing(const CScriptThing&, const C3DVector&, float) const{abort();}
void CameraCircleAroundPos(const C3DVector&, const C3DVector&, float) const{abort();}
void CameraMoveToPosAndLookAtPos(const C3DVector&, const C3DVector&, float) const{abort();}
void CameraMoveToPosAndLookAtThing(const C3DVector&, const CScriptThing&, float) const{abort();}
void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const C3DVector&, float, float) const{abort();}
void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const CScriptThing&, float, float) const{abort();}
void CameraMoveBetweenLookFromAndLookTo(const C3DVector&, const C3DVector&, const C3DVector&, const C3DVector&, float) const{abort();}
void CameraUseCameraPoint(const CCharString&, const CScriptThing&, float, long, long) const{abort();}
void CameraUseCameraPoint(const CScriptThing&, const CScriptThing&, float, long, long) const{abort();}
void CameraUseCameraPoint(const CCharString&, const C3DVector*, const CRightHandedSet*, float, long, long) const{abort();}
void CameraUseCameraPoint(const CScriptThing&, const C3DVector*, const CRightHandedSet*, float, long, long) const{abort();}
void CameraDoConversation(const CScriptThing&, const CScriptThing&, ECameraOp, bool) const{abort();}
void CameraDefault() {abort();}
void CameraResetToViewBehindHero(float) {abort();}
bool IsCameraInScriptedMode() const{abort();}
void CameraUseScreenEffect(float, float, float) const{abort();}
void CameraCancelScreenEffect() const{abort();}
bool IsCameraPosOnScreen(const C3DVector&) const{abort();}
float GetGameAngleXY() const{abort();}
void CameraEarthquakeIntensityAtPos(const C3DVector&, float, float) const{abort();}
void CameraShake(float, float) const{abort();}
void OpenDoor(const CScriptThing&) const{abort();}
void CloseDoor(const CScriptThing&) const{abort();}
void OpenHouseDoors(const CScriptThing&) const{abort();}
void CloseHouseDoors(const CScriptThing&) const{abort();}
void JamDoor(const CScriptThing&) const{abort();}
void SetDoorTriggerType(const CScriptThing&, EDoorTriggerType) const{abort();}
void OverrideAutomaticHouseLocking(const CScriptThing&, bool) const{abort();}
void SetHouseOwnedByPlayer(const CScriptThing&, bool, bool) const{abort();}
void SetBuyableHouseAsScripted(const CScriptThing&, bool) const{abort();}
bool IsChestOpen(const CScriptThing&) const{abort();}
bool OpenChest(const CScriptThing&, bool) const{abort();}
void CloseChest(const CScriptThing&) const{abort();}
long GetNumberOfKeysNeededToUnlockChest(const CScriptThing&, CCharString&) const{abort();}
void DisplayLockedChestMessage(const CScriptThing&) const{abort();}
void SetTrophyAsMountable(const CScriptThing&, bool) const{abort();}
void SetVillageLimbo(const CScriptThing&, bool) const{abort();}
void SetCreatureNotReload(const CScriptThing&) const{abort();}
bool IsSleepingTime(const CScriptThing&) const{abort();}
void EnableGuards(const CScriptThing&, bool) const{abort();}
void EnableVillagerDefTypes(const CScriptThing&, bool, const CCharString&) const{abort();}
CScriptThing TryToRespawnDefNamed(const CScriptThing&, const CCharString&, const C3DVector&) const{abort();}
void ClearHeroEnemyOfGuards(const CScriptThing&) const{abort();}
void SetThingAsUsable(const CScriptThing&, bool) const{abort();}
void SetThingHomeBuilding(const CScriptThing&, const CScriptThing&) const{abort();}
void SetVillageAttitude(const CScriptThing&, EScriptVillageAttitude) const{abort();}
void AddCrimeCommitted(const CScriptThing&, ECrime, bool, const CScriptThing*, const CScriptThing*, EOpinionPostDeedType) const{abort();}
void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing&) const{abort();}
void ClearThingBestEnemyTarget(const CScriptThing&) const{abort();}
void EntitySetInLimbo(const CScriptThing&, bool, bool) const{abort();}
bool IsEntityInLimbo(const CScriptThing&) const{abort();}
bool EntityGetShotStrikePos(const CScriptThing&, C3DVector&) const{abort();}
void EntitySetNegateAllHits(const CScriptThing&, bool) const{abort();}
void EntitySetEvadeAllHits(const CScriptThing&, bool) const{abort();}
void EntitySetAbleToBeEngagedInCombat(const CScriptThing&, bool) const{abort();}
void EntitySetAlwaysBlockAttacksFromThing(const CScriptThing&, const CScriptThing&, bool) const{abort();}
void EntitySetAttackThingImmediately(const CScriptThing&, const CScriptThing&, bool, bool) const{abort();}
void EntitySetCombatType(const CScriptThing&, const CCharString&) const{abort();}
void EntityResetCombatTypeToDefault(const CScriptThing&) const{abort();}
void EntitySetMaxNumberOfAttackers(const CScriptThing&, long) const{abort();}
void EntityClearMaxNumberOfAttackers(const CScriptThing&) const{abort();}
void EntityAttachToScript(const CScriptThing&, const CCharString&) const{abort();}
void EntitySetCombatAbility(const CScriptThing&, float) const{abort();}
void EntitySetRangedTarget(const CScriptThing&, const CScriptThing&) const{abort();}
void EntityClearRangedTarget(const CScriptThing&) const{abort();}
void EntitySetTargetable(const CScriptThing&, bool) const{abort();}
void EntitySetTargetingType(const CScriptThing&, ETargetingType) const{abort();}
void EntitySetTargetingValidTargetWithoutLOS(const CScriptThing&, bool) const{abort();}
void EntityTeleportToThing(const CScriptThing& a,const CScriptThing& b,bool p) const{assert(Identity(a)==1&&Identity(b)==3&&!p&&state.quests==1);++state.teleports;EventAt(15);}
void EntityTeleportToPosition(const CScriptThing&, const C3DVector&, float, bool, bool) const{abort();}
void EntitySetFacingAngle(const CScriptThing&, float, bool) const{abort();}
void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool) const{abort();}
void EntitySetPerceptionVariables(const CScriptThing&, float, float, float) const{abort();}
void SetThingPersistent(const CScriptThing&, bool) const{abort();}
void EntitySetThingAsWantingMoney(const CScriptThing&, long) const{abort();}
void EntitySetAppearanceMorphSeed(const CScriptThing&, long) const{abort();}
void SetEntityAsRegionFollowing(const CScriptThing&, const CScriptThing&, bool) const{abort();}
void SetEntityAsFollowingHeroThroughTeleporters(const CScriptThing&, bool) const{abort();}
void EntitySetAppearanceSeed(const CScriptThing&, unsigned long) const{abort();}
void EntityGetAppearanceSeed(const CScriptThing&, unsigned long&) const{abort();}
void EntitySetAsForSale(const CScriptThing&, bool) const{abort();}
void EntitySetStockItemPrice(const CScriptThing&, long) const{abort();}
long EntityGetStockItemPrice(const CScriptThing&) const{abort();}
void EntityPlayObjectAnimation(const CScriptThing&, const CCharString&, bool) const{abort();}
void EntitySetMaxRunningSpeed(const CScriptThing&, float) const{abort();}
void EntitySetMaxWalkingSpeed(const CScriptThing&, float) const{abort();}
void EntityResetMaxRunningSpeed(const CScriptThing&) const{abort();}
void EntityResetMaxWalkingSpeed(const CScriptThing&) const{abort();}
void EntityAttachToVillage(const CScriptThing&, const CScriptThing&) const{abort();}
void EntitySetAsSittingOnFloor(const CScriptThing&, bool) const{abort();}
void EntitySetAsHavingBoundHands(CScriptThing&, bool) const{abort();}
void EntitySetAsRemoveAllMovementBlockingModes(CScriptThing&) const{abort();}
void EntitySetAsScared(const CScriptThing&, bool) const{abort();}
void EntitySetAsDrunk(CScriptThing&, bool) const{abort();}
void EntityForceToLookAtThing(const CScriptThing&, const CScriptThing&) const{abort();}
void EntityForceToLookAtCamera(const CScriptThing&, bool) const{abort();}
void EntityForceToLookAtNothing(const CScriptThing&) const{abort();}
void EntityResetForceToLookAt(const CScriptThing&) const{abort();}
void EntitySetShotAccuracyPercentage(const CScriptThing&, long) const{abort();}
CScriptThing EntityGetStandingOnThing(const CScriptThing&) const{abort();}
CScriptThing EntityGetStandingInsideBuilding(const CScriptThing&) const{abort();}
void EntityDropGenericBox(const CScriptThing&) const{abort();}
void EntitySheatheWeapons(const CScriptThing&, bool) const{abort();}
void EntityUnsheatheWeapons(const CScriptThing&, bool) const{abort();}
void EntityUnsheatheMeleeWeapon(const CScriptThing&, bool) const{abort();}
void EntityUnsheatheRangedWeapon(const CScriptThing&, bool) const{abort();}
void EntitySetAlpha(const CScriptThing&, float, bool) const{abort();}
void EntitySetAsDrawable(const CScriptThing&, bool) const{abort();}
void EntitySetCutsceneBehaviour(const CScriptThing&, ECutsceneBehaviour) const{abort();}
ESex EntityGetSex(const CScriptThing&) const{abort();}
void EntitySetAsAbleToWalkThroughSolidObjects(const CScriptThing&, bool) const{abort();}
void EntitySetAsRespondToHit(const CScriptThing&, bool) const{abort();}
void EntitySetAsDamageable(const CScriptThing&, bool) const{abort();}
void EntitySetAsKillable(const CScriptThing&, bool, bool) const{abort();}
void EntitySetAsLocked(const CScriptThing&, bool) const{abort();}
void EntityDecapitate(const CScriptThing&) const{abort();}
void EntityGiveGold(const CScriptThing&, long) const{abort();}
void EntitySetAllowBossPhaseChanges(const CScriptThing&, bool) const{abort();}
long EntityGetBossPhase(const CScriptThing&) const{abort();}
void EntitySetBossPhase(const CScriptThing&, long) const{abort();}
void EntityResetCreatureMode(const CScriptThing&) const{abort();}
bool EntitySetAsReceivingEvents(const CScriptThing&, bool) const{abort();}
void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&, bool) const{abort();}
void EntitySetAsToAddToStatChangesWhenHit(const CScriptThing&, bool) const{abort();}
void EntityLeaveCombatStance(const CScriptThing&) const{abort();}
void EntitySetAsUseMovementInActions(const CScriptThing&, bool) const{abort();}
void EntitySetAsDisplayingEmoteIcon(const CScriptThing&, bool) const{abort();}
void EntitySetAsCollidableToThings(const CScriptThing&, bool) const{abort();}
void EntityEnableGravity(const CScriptThing&, bool) const{abort();}
void EntitySetLightAsOn(const CScriptThing&, bool) const{abort();}
void EntityFadeOut(const CScriptThing&, float) const{abort();}
void EntityFadeIn(const CScriptThing&, float) const{abort();}
void EntityBeginLoadingAnimation(const CScriptThing&, const CCharString&) const{abort();}
void EntityBeginLoadingBasicAnimations(const CScriptThing&) const{abort();}
bool EntityCastForcePush(const CScriptThing&, bool) const{abort();}
void EntityCastLightningAtTarget(const CScriptThing&, const CScriptThing&) const{abort();}
void BeginLoadingMesh(const CCharString&) const{abort();}
bool EntityWillTeleportToArea(const CScriptThing&, C3DVector, float, float) const{abort();}
void EntityStartScreamerSuperAttackThing(const CScriptThing&, const CScriptThing&) const{abort();}
void EntityEndScreamerSuperAttackThing(const CScriptThing&) const{abort();}
void SetHeroGuideToShowQuestCardsWhenSpokenTo(bool) {abort();}
void SetLightColour(const CScriptThing&, const CRGBColour&) const{abort();}
void CreatureGeneratorSetFamily(const CScriptThing&, const CCharString&) const{abort();}
void CreatureGeneratorTrigger(const CScriptThing&) const{abort();}
void CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger(const CScriptThing&, bool) const{abort();}
bool CreatureGeneratorIsDepleted(const CScriptThing&) const{abort();}
bool CreatureGeneratorIsDestroyed(const CScriptThing&) const{abort();}
void CreatureGeneratorSetGeneratedCreatureScriptName(const CScriptThing&, const CCharString&) const{abort();}
void CreatureGeneratorSetNumTriggers(const CScriptThing&, long) const{abort();}
long CreatureGeneratorGetNumGeneratedCreatures(const CScriptThing&) const{abort();}
bool CreatureGeneratorAreAllCreaturesAlive(const CScriptThing&) const{abort();}
void CreatureGeneratorAddTriggerer(const CScriptThing&, const CScriptThing&) const{abort();}
void CreatureGeneratorRemoveTriggerer(const CScriptThing&, const CScriptThing&) const{abort();}
void SetCreatureGeneratorEnabled(const CScriptThing&, bool) const{abort();}
void SetCreatureGeneratorsEnabled(const CCharString&, bool) const{abort();}
void SetCreatureGeneratorsEnabledDuringScript(const CCharString&, bool) const{abort();}
void SetCreatureGeneratorsCreatureGroupAsEnabled(ECreatureGroup, bool) const{abort();}
bool IsCreatureGenerationEnabledForRegion(const CCharString&) const{abort();}
bool IsCreatureFlying(const CScriptThing&) const{abort();}
void SetTeleporterAsActive(const CScriptThing&, bool) const{abort();}
bool IsTeleporterActive(const CScriptThing&) const{abort();}
void SetTeleportingAsActive(bool) const{abort();}
bool IsTeleportingActive() const{abort();}
void SetRegionExitAsActive(const CScriptThing&, bool) const{abort();}
void SetRegionEntranceAsActive(const CScriptThing&, bool) const{abort();}
void SetRegionTextDisplayAsActive(bool) const{abort();}
void SetHeroSleepingAsEnabled(bool) const{abort();}
bool IsHeroSleepingEnabled() const{abort();}
void SetExperienceSpendingAsEnabled(bool) const{abort();}
void SetMoralityChangingAsEnabled(bool) const{abort();}
void SetSummonerDeathExplosionAffectsHero(bool) const{abort();}
CScriptThing GetNearestEnabledDiggingSpot(const CScriptThing&) const{abort();}
bool IsDiggingSpotEnabled(const CScriptThing&) const{abort();}
bool IsDiggingSpotHidden(const CScriptThing&) const{abort();}
void SetDiggingSpotAsHidden(const CScriptThing&, bool) const{abort();}
bool CheckForCameraMessage(const CCharString&) const{abort();}
void WaitForCameraMessage(const CCharString&) const{abort();}
void SetThingAsConscious(const CScriptThing&, bool, const CCharString&) const{abort();}
void SetFireToThing(const CScriptThing&) const{abort();}
void ExtinguishFiresOnThing(const CScriptThing&) const{abort();}
bool IsThingOnFire(const CScriptThing&) const{abort();}
void AddItemToContainer(const CScriptThing&, const CCharString&) const{abort();}
void RemoveItemFromContainer(const CScriptThing&, const CCharString&) const{abort();}
void EntitySetDeathContainerAsEnabled(const CScriptThing&, bool) const{abort();}
void GetItemDefNamesFromContainer(const CScriptThing&, std::vector<CCharString,std::allocator<CCharString> >&) const{abort();}
void SetCreatureBrain(const CScriptThing&, const CCharString&) const{abort();}
void EntitySetStategroupEnabled(const CScriptThing&, const CCharString&, bool) const{abort();}
void EntitySetAllStategroupsEnabled(const CScriptThing&, bool) const{abort();}
void EntitySetCombatEnabled(const CScriptThing&, bool) const{abort();}
void EntitySetSleepEnabled(const CScriptThing&, bool) const{abort();}
void EntitySetOpinionReactionsEnabled(const CScriptThing&, bool) const{abort();}
void EntitySetDeedReactionsEnabled(const CScriptThing&, bool) const{abort();}
void DebugGetAllTextEntriesForTargetedThing(std::set<unsigned long,std::less<unsigned long>,std::allocator<unsigned long> >&) const{abort();}
void EntitySetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const{abort();}
void EntityUnsetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const{abort();}
void EntitySetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const{abort();}
void EntityUnsetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const{abort();}
void EntitySetInFaction(const CScriptThing&, const CCharString&) const{abort();}
void SetFactionAsAlliedToFaction(const CCharString&, const CCharString&) const{abort();}
void SetFactionAsNeutralToFaction(const CCharString&, const CCharString&) const{abort();}
void SetFactionAsEnemyToFaction(const CCharString&, const CCharString&) const{abort();}
bool AreEntitiesEnemies(const CScriptThing&, const CScriptThing&) const{abort();}
EOpinionAttitudeType GetNextInOpinionAttitudeGraph(EOpinionAttitudeType) const{abort();}
void GetOpinionAttitudeAsString(EOpinionAttitudeType, CCharString&) const{abort();}
EOpinionAttitudeType EntityGetOpinionAttitudeToPlayer(const CScriptThing&) const{abort();}
void EntityGetOpinionAttitudeToPlayerAsString(const CScriptThing&, CCharString&) const{abort();}
float EntityGetOpinionOfPlayer(const CScriptThing&, EOpinion) const{abort();}
void EntitySetOpinionReactionMask(const CScriptThing&, long) const{abort();}
void EntitySetOpinionReactionMask(const CScriptThing&, const CCharString&) const{abort();}
void EntitySetOpinionDeedMask(const CScriptThing&, long) const{abort();}
void EntitySetOpinionDeedMask(const CScriptThing&, const CCharString&) const{abort();}
void EntitySetOpinionDeedTypeEnabled(const CScriptThing&, EOpinionDeedType, bool) const{abort();}
void EntitySetOpinionAttitudeEnabled(const CScriptThing&, EOpinionAttitudeType, bool) const{abort();}
void EntitySetOpinionReactionEnabled(const CScriptThing&, EOpinionReactionType, bool) const{abort();}
void EntitySetPersonalityOverride(const CScriptThing&, const CCharString&) const{abort();}
void EntitySetPersonalityOverride(const CScriptThing&, long) const{abort();}
void EntityClearPersonalityOverride(const CScriptThing&) const{abort();}
void EntitySetAsOpinionSource(const CScriptThing&, const CCharString&) const{abort();}
void EntitySetAsOpinionSource(const CScriptThing&, long) const{abort();}
void EntityUnsetAsOpinionSource(const CScriptThing&, bool) const{abort();}
void OpinionSourceSetAsExclusive(const CScriptThing&, bool) const{abort();}
void OpinionSourceSetAsAttentionGrabbing(const CScriptThing&, bool) const{abort();}
void EntityPostOpinionDeedToAll(const CScriptThing&, EOpinionDeedType) const{abort();}
void EntityPostOpinionDeedToRecipient(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const{abort();}
void EntityPostOpinionDeedToRecipientVillage(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const{abort();}
long EntityPostOpinionDeedKeepSearchingForWitnesses(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const{abort();}
void RemoveOpinionDeedStillSearchingForWitnesses(const CScriptThing&, long) const{abort();}
bool IsDeedWitnessed(long) const{abort();}
bool CanThingBe_Seen_ByOtherThing(const CScriptThing&, const CScriptThing&) const{abort();}
bool CanThingBe_NearlySeen_ByOtherThing(const CScriptThing&, const CScriptThing&) const{abort();}
bool CanThingBe_Smelled_ByOtherThing(const CScriptThing&, const CScriptThing&) const{abort();}
bool CanThingBe_Heard_ByOtherThing(const CScriptThing&, const CScriptThing&) const{abort();}
bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing&, const CScriptThing&) const{abort();}
void EntitySetAsAwareOfThing(const CScriptThing&, const CScriptThing&) const{abort();}
void EntitySetSoundRadius(const CScriptThing&, float) const{abort();}
void EntitySetSmellRadius(const CScriptThing&, float) const{abort();}
void EntitySetSightRadius(const CScriptThing&, float) const{abort();}
void EntitySetExtendedSightRadius(const CScriptThing&, float) const{abort();}
void EntitySetGiveUpChaseRadius(const CScriptThing&, float) const{abort();}
float EntityGetHearingRadius(const CScriptThing&) const{abort();}
bool ManuallyTriggerTrap(const CScriptThing&) const{abort();}
bool ManuallyResetTrap(const CScriptThing&) const{abort();}
void SetTimeOfDay(float) const{abort();}
long GetTimeOfDay() const{abort();}
void SetTimeAsStopped(bool, long&) const{abort();}
void FastForwardTimeTo(float, float) const{abort();}
bool IsTimeOfDayBetween(long, long) const{abort();}
long GetDayOfWeek() const{abort();}
long GetDayCount() const{abort();}
long GetWorldFrame() const{abort();}
long GetConstantFPS() const{abort();}
CCharString GetActiveQuestName() const{abort();}
void TransitionToTheme(const CCharString&, float) const{abort();}
void ResetToDefaultTheme(float) const{abort();}
void TransitionToThemeAllInternals(const CCharString&, float) const{abort();}
void ResetToDefaultThemeAllInternals(float) const{abort();}
void TransitionToThemeExternals(const CCharString&, float) const{abort();}
void ResetToDefaultThemeExternals(float) const{abort();}
void SetEnvironmentThemeWeightAllChannels(const CCharString&, float) const{abort();}
void SetEnvironmentThemeWeightAllInternals(const CCharString&, float) const{abort();}
void SetEnvironmentThemeWeightExternals(const CCharString&, float) const{abort();}
void SetSoundThemesAsEnabledForRegion(const CCharString&, bool) const{abort();}
void SetAllSoundsAsMuted(bool) const{abort();}
void* RadialBlurFadeTo(float, C3DVector, float, C3DVector, float, float, float) const{abort();}
void* RadialBlurFadeTo(float, float, float, float, float, float, float) const{abort();}
void RadialBlurFadeOut(float, void*) const{abort();}
bool IsRadialBlurFadeActive() const{abort();}
void CancelRadialBlurFade() const{abort();}
void RadialBlurSetCenterWorldPos(void*, const C3DVector&) const{abort();}
void* DisplacementMonochromeEffectColourFadeTo(float, const CRGBFloatColour&) const{abort();}
void DisplacementMonochromeEffectColourFadeOut(float, void*) const{abort();}
void* ScreenFilterFadeTo(float, float, float, float, float, const CRGBFloatColour&, const std::vector<CScreenFilterSThingByPass,std::allocator<CScreenFilterSThingByPass> >&) const{abort();}
void ScreenFilterFadeOut(float, void*) const{abort();}
void SetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&, void*) const{abort();}
void UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&) const{abort();}
bool IsGiftRomantic(const CCharString&) const{abort();}
bool IsGiftFriendly(const CCharString&) const{abort();}
bool IsGiftOffensive(const CCharString&) const{abort();}
bool IsThingABed(const CScriptThing&) const{abort();}
bool IsThingAChest(const CScriptThing&) const{abort();}
bool IsThingADoor(const CScriptThing&) const{abort();}
bool IsThingSmashable(const CScriptThing&) const{abort();}
bool IsThingSearchable(const CScriptThing&) const{abort();}
void ApplyScriptBrush(const CCharString&) const{abort();}
void EnableDecals(bool) const{abort();}
unsigned long PlayCriteriaSoundOnThing(const CScriptThing&, const CCharString&) const{abort();}
unsigned long PlaySoundOnThing(const CScriptThing&, const CCharString&) const{abort();}
unsigned long PlaySoundAtPos(const C3DVector&, const CCharString&) const{abort();}
unsigned long Play2DSound(const CCharString&) const{abort();}
bool IsSoundPlaying(unsigned long) const{abort();}
void EnableSounds(bool) const{abort();}
void StopSound(unsigned long) const{abort();}
void OverrideMusic(EMusicSetType, bool, bool) const{abort();}
void StopOverrideMusic(bool) const{abort();}
void CacheMusicSet(EMusicSetType) const{abort();}
void EnableDangerMusic(bool) const{abort();}
bool IsDangerMusicEnabled() const{abort();}
void StartCountdownTimer(float) const{abort();}
float GetCountdownTimer() const{abort();}
void AutoSaveCheckPoint() const{abort();}
void AutoSaveQuestStart() const{abort();}
void AutoSave() const{abort();}
void SetSavingAsEnabled(bool) const{abort();}
bool IsSavingEnabled() const{abort();}
void SetSaveGameMarkerPos(const C3DVector&) const{abort();}
void ResetToFrontEnd() const{abort();}
void SetGuildSealRecallLocation(const C3DVector&, float) const{abort();}
C3DVector GetGuildSealRecallPos() const{abort();}
float GetGuildSealRecallAngleXY() const{abort();}
void SetReadableObjectText(const CScriptThing&, const CWideString&) const{abort();}
void SetReadableObjectTextTag(const CScriptThing&, const CCharString&) const{abort();}
CWideString GetFormattedString(const CCharString&, const std::vector<CWideString,std::allocator<CWideString> >&) const{abort();}
CWideString GetTextString(const CCharString&) const{abort();}
void AddRumourCategory(const CCharString&) const{abort();}
void AddNewRumourToCategory(const CCharString&, const CCharString&) const{abort();}
void RemoveRumourCategory(const CCharString&) const{abort();}
void SetCategoryActivity(const CCharString&, bool) const{abort();}
void AddGossipVillage(const CCharString&, const CCharString&) const{abort();}
void AddGossipFactionToCategory(const CCharString&, const CCharString&) const{abort();}
void SetIsGossipForPlayer(const CCharString&, bool) const{abort();}
void SetIsGossipForPlayer(CCharString, bool) {abort();}
void UpdateOnlineScore_Archery(float) const{abort();}
void UpdateOnlineScore_ChickenKick(float) const{abort();}
void UpdateOnlineScore_ChapelOrTemple(float) const{abort();}
void UpdateOnlineScore_FishingCompo(float) const{abort();}
void UpdateScore_FishingCompetition(float) const{abort();}
float GetBestTimePairs() const{abort();}
float GetBestTimeSorting() const{abort();}
long GetBestScoreBlackjack() const{abort();}
long GetBestScoreCoinGolfOakVale() const{abort();}
long GetBestScoreCoinGolfSnowSpire() const{abort();}
long GetBestScoreShoveHaPenny() const{abort();}
float GetBestTimeGuessTheAddition() const{abort();}
bool IsHeroInTavernGame() const{abort();}
long GetNumHousesOwned() const{abort();}
void StartSneaking() const{abort();}
long GetStealDuration(const CScriptThing&) const{abort();}
void SetUseableByHero(const CScriptThing&, bool) const{abort();}
void SetOwnedByHero(const CScriptThing&, bool) const{abort();}
void SetTavernTableAvailableForUse(const CScriptThing&, bool) const{abort();}
void SetIsThingTurncoatable(const CScriptThing&, bool) const{abort();}
void SetIsThingForcePushable(const CScriptThing&, bool) const{abort();}
void SetIsThingLightningable(const CScriptThing&, bool) const{abort();}
void SetIsThingEpicSpellable(const CScriptThing&, bool) const{abort();}
bool IsThingTurncoated(const CScriptThing&) const{abort();}
void AddCreatureScriptedMode(const CScriptThing&, const CCharString&) const{abort();}
void RemoveCreatureScriptedMode(const CScriptThing&) const{abort();}
void ForceShipsVisible() const{abort();}
bool GetSleepingPositionAndOrientationFromBed(const CScriptThing&, const CScriptThing&, C3DVector&, C3DVector&) {abort();}
void SetBedAvailability(const CScriptThing&, bool) {abort();}
void RepopulateVillage(const CScriptThing&) const{abort();}
void SmashAllWindowsWithinRadiusOfPoint(const C3DVector&, float) {abort();}
CScriptThing SetResidency(const CScriptThing&, bool) {abort();}
void SetThankingPhrase(CScriptThing, unsigned long) {abort();}
unsigned long GetThankingPhrase(CScriptThing) {abort();}
void ResetThankingPhrase(CScriptThing) {abort();}
void SetIgnoringPhrase(CScriptThing, unsigned long) {abort();}
unsigned long GetIgnoringPhrase(CScriptThing) {abort();}
void ResetIgnoringPhrase(CScriptThing) {abort();}
void SetWanderCentrePoint(CScriptThing, C3DVector) {abort();}
C3DVector GetWanderCentrePoint(CScriptThing) {abort();}
void ResetWanderCentrePoint(CScriptThing) {abort();}
void SetWanderMinDistance(CScriptThing, float) {abort();}
float GetWanderMinDistance(CScriptThing) {abort();}
void ResetWanderMinDistance(CScriptThing) {abort();}
void SetWanderMaxDistance(CScriptThing, float) {abort();}
float GetWanderMaxDistance(CScriptThing) {abort();}
void ResetWanderMaxDistance(CScriptThing) {abort();}
void SetGossipCounter(CScriptThing, long) {abort();}
long GetGossipCounter(CScriptThing) {abort();}
void ResetGossipCounter(CScriptThing) {abort();}
void SetMaxGossipPhrase(CScriptThing, long) {abort();}
long GetMaxGossipPhrase(CScriptThing) {abort();}
void ResetMaxGossipPhrase(CScriptThing) {abort();}
void SetWarningPhrase(CScriptThing, unsigned long) {abort();}
unsigned long GetWarningPhrase(CScriptThing) {abort();}
void ResetWarningPhrase(CScriptThing) {abort();}
void SetBeerRequestPhrase(CScriptThing, unsigned long) {abort();}
unsigned long GetBeerRequestPhrase(CScriptThing) {abort();}
void ResetBeerRequestPhrase(CScriptThing) {abort();}
void SetScriptingStateGroup(CScriptThing, EScriptingStateGroups) {abort();}
EScriptingStateGroups GetScriptingStateGroup(CScriptThing) {abort();}
void ResetScriptingStateGroup(CScriptThing) {abort();}
void SetMaxHeroReactionDistance(CScriptThing, float) {abort();}
float GetMaxHeroReactionDistance(CScriptThing) {abort();}
void ResetMaxHeroReactionDistance(CScriptThing) {abort();}
void SetActionFrequency(CScriptThing, long) {abort();}
long GetActionFrequency(CScriptThing) {abort();}
void ResetActionFrequency(CScriptThing) {abort();}
void SetActionFrequencyVariation(CScriptThing, float) {abort();}
float GetActionFrequencyVariation(CScriptThing) {abort();}
void ResetActionFrequencyVariation(CScriptThing) {abort();}
void SetAction(CScriptThing, CCharString) {abort();}
CCharString GetAction(CScriptThing) {abort();}
void ResetAction(CScriptThing) {abort();}
void SetFaceHeroForAction(CScriptThing, bool) {abort();}
bool GetFaceHeroForAction(CScriptThing) {abort();}
void ResetFaceHeroForAction(CScriptThing) {abort();}
void SetTargetName(CScriptThing, CCharString) {abort();}
CCharString GetTargetName(CScriptThing) {abort();}
void ResetTargetName(CScriptThing) {abort();}
void SetFollowDistance(CScriptThing, float) {abort();}
float GetFollowDistance(CScriptThing) {abort();}
void ResetFollowDistance(CScriptThing) {abort();}
void SetAttackHeroOnSight(CScriptThing, bool) {abort();}
bool GetAttackHeroOnSight(CScriptThing) {abort();}
void ResetAttackHeroOnSight(CScriptThing) {abort();}
void SetTimeToSpendHarassingHero(CScriptThing, long) {abort();}
long GetTimeToSpendHarassingHero(CScriptThing) {abort();}
void ResetTimeToSpendHarassingHero(CScriptThing) {abort();}
void SetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing, float) {abort();}
float GetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) {abort();}
void ResetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) {abort();}
void SetCombatNearbyBreakOffRange(CScriptThing, float) {abort();}
float GetCombatNearbyBreakOffRange(CScriptThing) {abort();}
void ResetCombatNearbyBreakOffRange(CScriptThing) {abort();}
void SetStealStealableItems(CScriptThing, bool) {abort();}
bool GetStealStealableItems(CScriptThing) {abort();}
void ResetStealStealableItems(CScriptThing) {abort();}
void SetRecoverStealableItems(CScriptThing, bool) {abort();}
bool GetRecoverStealableItems(CScriptThing) {abort();}
void ResetRecoverStealableItems(CScriptThing) {abort();}
void SetTakeStealableItemToRandomDestination(CScriptThing, bool) {abort();}
bool GetTakeStealableItemToRandomDestination(CScriptThing) {abort();}
void ResetTakeStealableItemToRandomDestination(CScriptThing) {abort();}
void SetKillSelfAndStealableItemAfterReachingDestination(CScriptThing, bool) {abort();}
bool GetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) {abort();}
void ResetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) {abort();}
void SetAllowedToFollow(CScriptThing, bool) {abort();}
bool GetAllowedToFollow(CScriptThing) {abort();}
void ResetAllowedToFollow(CScriptThing) {abort();}
void SetTableName(CScriptThing, CCharString) {abort();}
CCharString GetTableName(CScriptThing) {abort();}
void ResetTableName(CScriptThing) {abort();}
void SetSeatName(CScriptThing, CCharString) {abort();}
CCharString GetSeatName(CScriptThing) {abort();}
void ResetSeatName(CScriptThing) {abort();}
void SetDisableHeadLooking(CScriptThing, bool) {abort();}
bool GetDisableHeadLooking(CScriptThing) {abort();}
void ResetDisableHeadLooking(CScriptThing) {abort();}
void SetIsPushableByHero(CScriptThing, bool) {abort();}
bool GetIsPushableByHero(CScriptThing) {abort();}
void ResetIsPushableByHero(CScriptThing) {abort();}
void SetLookForFiniteTime(CScriptThing, bool) {abort();}
bool GetLookForFiniteTime(CScriptThing) {abort();}
void ResetLookForFiniteTime(CScriptThing) {abort();}
void SetAvoidRegionExits(CScriptThing, bool) {abort();}
bool GetAvoidRegionExits(CScriptThing) {abort();}
void ResetAvoidRegionExits(CScriptThing) {abort();}
void SetTargetingDistanceOffset(CScriptThing, float) {abort();}
float GetTargetingDistanceOffset(CScriptThing) {abort();}
void ResetTargetingDistanceOffset(CScriptThing) {abort();}
void SetPlayerUsingMeleeDummies(bool) {abort();}
bool GetPlayerUsingMeleeDummies() {abort();}
void SetPlayerUsingRangedDummies(bool) {abort();}
bool GetPlayerUsingRangedDummies() {abort();}
void SetPlayerUsingWillDummies(bool) {abort();}
bool GetPlayerUsingWillDummies() {abort();}
void SetCheapHeadLooking(bool) {abort();}
bool GetCheapHeadLooking() {abort();}
void SetQuitTavernGame(bool) {abort();}
bool GetQuitTavernGame() {abort();}
void SetPrizeTavernTable(bool) {abort();}
bool GetPrizeTavernTable() {abort();}
void SetBettingActive(bool) {abort();}
bool GetBettingActive() {abort();}
void SetBettingAccept(bool) {abort();}
bool GetBettingAccept() {abort();}
void SetBettingAmount(long) {abort();}
long GetBettingAmount() {abort();}
void SetCountBetMoneyDown(bool) {abort();}
bool GetCountBetMoneyDown() {abort();}
void SetSpotTheAdditionBeaten(bool) {abort();}
bool GetSpotTheAdditionBeaten() {abort();}
void SetGlobalTargetingDistanceOffset(float) {abort();}
float GetGlobalTargetingDistanceOffset() {abort();}
void SetTradingPriceMult(float) {abort();}
float GetTradingPriceMult() {abort();}
void SetBoastingEnabled(bool) {abort();}
bool GetBoastingEnabled() {abort();}
void SetActiveGossipCategories(CCharString, bool) {abort();}
bool* GetActiveGossipCategories(CCharString) {abort();}
const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetActiveGossipCategories() {abort();}
int GetActiveGossipCategoriesSize() {abort();}
void ClearActiveGossipCategories() {abort();}
bool* GetIsGossipForPlayer(CCharString) {abort();}
const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetIsGossipForPlayer() {abort();}
int GetIsGossipForPlayerSize() {abort();}
void ClearIsGossipForPlayer() {abort();}
void SetGossip(CCharString, CCharString, long) {abort();}
CCharString GetGossip(CCharString, long) {abort();}
const std::vector<CCharString,std::allocator<CCharString> >* GetGossip(CCharString) {abort();}
int GetGossipSize(CCharString) {abort();}
void ClearGossip(CCharString) {abort();}
void RemoveGossip(CCharString) {abort();}
void AddGossip(CCharString, CCharString) {abort();}
void AddGossip(CCharString) {abort();}
void SetGossipVillages(CCharString, CCharString, long) {abort();}
CCharString GetGossipVillages(CCharString, long) {abort();}
const std::vector<CCharString,std::allocator<CCharString> >* GetGossipVillages(CCharString) {abort();}
int GetGossipVillagesSize(CCharString) {abort();}
void ClearGossipVillages(CCharString) {abort();}
void RemoveGossipVillages(CCharString) {abort();}
void AddGossipVillages(CCharString, CCharString) {abort();}
void AddGossipVillages(CCharString) {abort();}
void SetGossipFactions(CCharString, CCharString, long) {abort();}
CCharString GetGossipFactions(CCharString, long) {abort();}
const std::vector<CCharString,std::allocator<CCharString> >* GetGossipFactions(CCharString) {abort();}
int GetGossipFactionsSize(CCharString) {abort();}
void ClearGossipFactions(CCharString) {abort();}
void RemoveGossipFactions(CCharString) {abort();}
void AddGossipFactions(CCharString, CCharString) {abort();}
void AddGossipFactions(CCharString) {abort();}
};
static FixtureInterface Interface;
CGameScriptInterfaceBase*pDSTGame=&Interface;

NScript::CQ_EndGameScript::CQ_EndGameScript(CScriptDataBase*d,CGameScriptInterfaceBase&g):CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(d,g){}
NScript::CQ_EndGameScript::CHookCoastTeleporter::CHookCoastTeleporter(CQ_EndGameScript&p,CScriptDataBase*,const CScriptThing&me):CEntityScriptBase(*p.Game,me),ParentClass(p),PMasterData(0){}
bool CEntityScriptBase::IsActiveThreadTerminating()const{assert(state.checks<state.stop);return ++state.checks==state.stop;}
void FABLE_FASTCALL AddLogbookStoryEntry(long n){assert(n==240&&state.teleports==1);++state.stories;EventAt(16);}
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&n,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&cast,std::map<CCharString,bool>*a,std::map<CCharString,CCharString>*b,bool c,bool d){assert(strcmp(n.GetAsCharArray(),"CS_GATEWAY_OPEN")==0&&!a&&!b&&!c&&d&&state.pauseStage==1&&state.movies==1&&cast.size()==1);std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::iterator i=cast.find(CCharString("HERO"));assert(i!=cast.end()&&i->second.PImp.Data==ActiveImplementation&&static_cast<FixtureScriptedThing*>(i->second.PImp.Data)->Kind==7&&i->second.PImp.Info->RefCount==2);++state.cutscenes;EventAt(12);}
static void CheckRealMapConstructor(){typedef std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>Map;for(int k=0;k<4096;++k){Map map;assert(map.size()==0&&map.begin()==map.end());std::_Rb_tree_node_base*node=map.end()._M_node;assert(node&&!node->_M_color&&!node->_M_parent&&node->_M_left==node&&node->_M_right==node);}}

void CBaseObject::Validate() const { abort(); }
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
void CScriptDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CScriptDef::GetSizeofClass() { abort(); }
void CScriptDef::Transfer(CPersistContext&) { abort(); }
void CFeatDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CFeatDef::GetSizeofClass() { abort(); }
void CFeatDef::Transfer(CPersistContext&) { abort(); }
void CFireHeartPatternDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CFireHeartPatternDef::GetSizeofClass() { abort(); }
void CFireHeartPatternDef::Transfer(CPersistContext&) { abort(); }
void CHeroSoulsRoundDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CHeroSoulsRoundDef::GetSizeofClass() { abort(); }
void CHeroSoulsRoundDef::Transfer(CPersistContext&) { abort(); }
void CArenaRoundDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CArenaRoundDef::GetSizeofClass() { abort(); }
void CArenaRoundDef::Transfer(CPersistContext&) { abort(); }
void CConversation::Copy(const CDefClassBase*) { abort(); }
unsigned long CConversation::GetSizeofClass() { abort(); }
void CConversation::Transfer(CPersistContext&) { abort(); }
const CCharString& CScriptThing::GetName() const { abort(); }
CCharString CScriptThing::GetDefName() const { abort(); }
CCharString CScriptThing::GetDataString() const { abort(); }
void CScriptThing::SetDataString(const CCharString&) { abort(); }
C3DVector CScriptThing::GetFocalPos() const { abort(); }
const C3DVector& CScriptThing::GetPos() const { abort(); }
C3DVector CScriptThing::GetHomePos() const { abort(); }
CCharString CScriptThing::GetCurrentMapName() const { abort(); }
CCharString CScriptThing::GetHomeMapName() const { abort(); }
float CScriptThing::GetAngleXY() const { abort(); }
CThing* CScriptThing::GetPThing() const { abort(); }
FableNScriptUInt64 CScriptThing::GetPThingUniqueID() const { abort(); }
EAIStateGroupType CScriptThing::GetCurrentStateGroupType() const { abort(); }
EScriptAIPriority CScriptThing::GetCurrentScriptPriority() const { abort(); }
bool CScriptThing::IsSneaking() const { abort(); }
bool CScriptThing::IsAwareOfHero() const { abort(); }
bool CScriptThing::MsgIsTriggeredBy(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsKilledBy(const CCharString&) const { abort(); }
CScriptThing CScriptThing::MsgWhoKilledMe() const { abort(); }
bool CScriptThing::MsgIsUsedBy(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitBy(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByWithFlourish(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByWithDecapitate(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByWithWeapon(const CCharString&, const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByWithProjectileWeapon(const CCharString&, float&) const { abort(); }
CScriptThing CScriptThing::MsgWhoHitMe() const { abort(); }
bool CScriptThing::MsgIsTalkedToBy(const CCharString&) const { abort(); }
CScriptThing CScriptThing::MsgWhoTalkedToMe() const { abort(); }
bool CScriptThing::MsgExpressionPerformedTo(CCharString&) const { abort(); }
CScriptThing CScriptThing::MsgWhoExpressedToMe() const { abort(); }
long CScriptThing::MsgHowLongWasExpressionPerformed() const { abort(); }
bool CScriptThing::MsgIsKnockedOutBy(const CCharString&) const { abort(); }
CScriptThing CScriptThing::MsgWhoKnockedMeOut() const { abort(); }
bool CScriptThing::MsgReceivedMoney(long&) const { abort(); }
bool CScriptThing::MsgIsPresentedWithItem(CCharString&) const { abort(); }
bool CScriptThing::MsgReceivedInventoryItem() const { abort(); }
bool CScriptThing::MsgPerformedSpecialAbility(EHeroAbility) const { abort(); }
bool CScriptThing::MsgPerformedAnySpecialAbility() const { abort(); }
bool CScriptThing::MsgPerformedAnyAggressiveSpecialAbility() const { abort(); }
bool CScriptThing::MsgPerformedAnyNonAggressiveSpecialAbility() const { abort(); }
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(EHeroAbility, const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByAnyNonAggressiveSpecialAbilityFrom(const CCharString&) const { abort(); }
bool CScriptThing::MsgOpenedChest() const { abort(); }
bool CScriptThing::MsgHitEnemyWithMeleeWeapon() const { abort(); }
bool CScriptThing::MsgHitFriendWithMeleeWeapon() const { abort(); }
bool CScriptThing::MsgHitEnemyWithRangedWeapon() const { abort(); }
bool CScriptThing::MsgHitFriendWithRangedWeapon() const { abort(); }
bool CScriptThing::MsgHitEnemyWithBareHands() const { abort(); }
bool CScriptThing::MsgHitFriendWithBareHands() const { abort(); }
bool CScriptThing::MsgAttackedWithMeleeWeaponWithoutHittingAnything() const { abort(); }
bool CScriptThing::MsgAttackedWithBareHandsWithoutHittingAnything() const { abort(); }
bool CScriptThing::MsgTalkedToAnyone() const { abort(); }
bool CScriptThing::MsgGetThingsKilled(std::vector<unsigned long,std::allocator<unsigned long> >&) const { abort(); }
bool CScriptThing::MsgPerformedFlourish() const { abort(); }
bool CScriptThing::MsgPerformedSuccessfulFlourish() const { abort(); }
bool CScriptThing::MsgOnMoralityChanged(long&) const { abort(); }
bool CScriptThing::MsgOnCutSceneAnimEvent(CCharString&) const { abort(); }
bool CScriptThing::MsgIsKicked() const { abort(); }
bool CScriptThing::IsUnconscious() const { abort(); }
bool CScriptThing::IsUsable() const { abort(); }
bool CScriptThing::IsBeingCarriedBy(const CCharString&) const { abort(); }
bool CScriptThing::IsOpenDoor() const { abort(); }
bool CScriptThing::IsSummonedCreature() const { abort(); }
void CScriptThing::SetAsUsable(bool) { abort(); }
void CScriptThing::SetFriendsWithEverythingFlag(bool) { abort(); }
bool CScriptThing::GetActivationTriggerStatus() const { abort(); }
void CScriptThing::SetActivationTriggerStatus(bool) { abort(); }
void CScriptThing::SetToKillOnLevelUnload(bool) { abort(); }
void CScriptThing::UpdateThingAttachment() { abort(); }
void CScriptThing::IncrementScriptCounter() { abort(); }
void CScriptThing::DecrementScriptCounter() { abort(); }
long CScriptThing::GetScriptCounter() { abort(); }
bool CScriptThing::IsAlive() const { abort(); }
bool CScriptThing::IsDead() const { abort(); }
bool CScriptThing::IsNull() const { abort(); }
bool CScriptThing::IsEqualTo(const CScriptThing&) const { abort(); }
void CScriptGameResourceObjectScriptedThingBase::SummonerLightningOrbAttackTarget(const CScriptThing&) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::FireProjectileWeaponAtTarget(const CScriptThing&) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::MoveToPosition(const C3DVector&, float, EScriptEntityMoveType, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::MoveToThing(const CScriptThing&, float, EScriptEntityMoveType, bool, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::FollowPreCalculatedRoute(const CScriptThing&, EScriptEntityMoveType, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::FollowThing(const CScriptThing&, float, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::StopFollowingThing(const CScriptThing&) { abort(); }
bool CScriptGameResourceObjectScriptedThingBase::IsFollowActionRunning(const CScriptThing&) const { abort(); }
void CScriptGameResourceObjectScriptedThingBase::ClearCommands() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::WaitWhilePerformingTasks() { abort(); }
CScriptThing CScriptGameResourceObjectScriptedThingBase::GetScriptThing() const { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing&, unsigned long, ETextGroupSelectionMethod, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing&, const char*, ETextGroupSelectionMethod, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::AskHeroQuestion(const CScriptThing&, const char*, const CCharString&, const CCharString&, const CCharString&, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Converse(const CScriptGameResourceObjectScriptedThingBase&, const char*) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::PerformExpression(const CScriptThing&, const CCharString&) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::PlayAnimation(const CCharString&, bool, bool, bool, bool, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::PlayCombatAnimation(const CCharString&, bool, bool, bool, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::PlayLoopingAnimation(const CCharString&, long, bool, bool, bool, bool, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::ClearAllActions() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::ClearAllActionsIncludingLoopingAnimations() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::MoveToAndPickUpGenericBox(CScriptThing&, EScriptEntityMoveType, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::DropGenericBox() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::UnsheatheWeapons() { abort(); }
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask() const { abort(); }
bool CScriptGameResourceObjectScriptedThingBase::IsFollowingThing() const { abort(); }
void CScriptGameResourceObjectScriptedThingBase::WaitForEntityToFinishPerformingTasks(const CScriptGameResourceObjectScriptedThingBase&) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Wait(float) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Validate() const { abort(); }
void CScriptBase::RegisterMain() { abort(); }
void CScriptBase::Main() { abort(); }
void CScriptBase::Init() { abort(); }
void CScriptBase::OnPersist(CPersistContext&) { abort(); }
void CEntityScriptBase::Init() { abort(); }
void CEntityScriptBase::OnPersist(CPersistContext&) { abort(); }
void CEntityScriptBase::OnPredicateFail() { abort(); }
void CEntityScriptBase::OnInterrupted() { abort(); }
void NScript::CQ_EndGameScript::RegisterMain() { abort(); }
void NScript::CQ_EndGameScript::Main() { abort(); }
void NScript::CQ_EndGameScript::Init() { abort(); }
void NScript::CQ_EndGameScript::OnPersist(CPersistContext&) { abort(); }
CScriptBase* NScript::CQ_EndGameScript::CHookCoastTeleporter::GetParentScript()const { abort(); }
void NScript::CQ_EndGameScript::CHookCoastTeleporter::Init() { abort(); }
static float FloatBits(unsigned u){float f;memcpy(&f,&u,4);return f;}
int main(){
    assert(sizeof(float)==4&&sizeof(unsigned)==4);CheckRealMapConstructor();Hero=MakeThing(1);Marker=MakeThing(2);Teleport=MakeThing(3);pGlobalScriptDef.Reset(&Definition);
    float inputs[]={-4.0f,0.0f,1.0f,2.0f,4.0f,7.0f,8.0f,12.0f,16.0f,FloatBits(0x7fc12345u),FloatBits(0x7f800000u),FloatBits(0xff800000u)};
    float amounts[]={2.0f,4.0f},uppers[]={8.0f,12.0f},intervals[]={-2.0f,0.0f,1.75f,3.5f};
    int finishes[]={0,1,3,6},failures[]={0,1,3};
    unsigned cases=0;
    for(int initial=0;initial<12;++initial)for(int amount=0;amount<2;++amount)for(int upper=0;upper<2;++upper)for(int interval=0;interval<4;++interval)for(int finish=0;finish<4;++finish)for(int fail=0;fail<3;++fail){
        Event previous[2048];int previousCount=0;
        for(int stop=1;stop<=48;++stop){
            memset(&state,0,sizeof(state));state.stop=stop;state.initial=inputs[initial];state.amount=amounts[amount];state.upper=uppers[upper];state.interval=intervals[interval];state.finishFrame=finishes[finish];state.failStarts=failures[fail];state.initialEnough=state.initial>=state.upper;
            Definition.EndGame_UpperBarLimit=state.upper;Definition.EndGame_BarDecrementAmount=state.amount;Definition.EndGame_BarDecrementInterval=state.interval;
            NScript::CQ_EndGameScript parent(0,Interface);parent.UndeadBar=state.initial;parent.GuiCounter=71;parent.ShowUndeadBar=false;state.bar=&parent.UndeadBar;
            NScript::CQ_EndGameScript::CHookCoastTeleporter script(parent,0,Marker);script.Main();
            assert(state.checks==stop&&state.registered==state.deregistered&&!LiveHandles()&&!state.movies&&state.allocated==state.freed);
            assert(state.messages==0||state.messages==2);assert(state.messages==state.pauses);assert(parent.ShowUndeadBar==(state.messages==2));
            assert(state.registered==((state.initial<state.amount&&stop==1)?0:1));
            assert(state.removed==state.markers&&state.removed<=1&&state.cutscenes<=1&&state.fades==state.quests&&state.quests==state.teleports&&state.teleports==state.stories);
            assert(state.pauseStage==0||state.pauseStage==2);assert(state.cutscenes==state.stories);
            if(state.initialEnough)assert(!state.messages&&!state.updates&&!state.starts&&!state.cutscenes&&state.removed==(stop>1));
            if(stop==48&&!state.initialEnough&&(state.finished||!(state.initial<state.upper)))assert(state.stories==1);
            assert(previousCount<=state.eventCount);for(int n=0;n<previousCount;++n)assert(previous[n].kind==state.events[n].kind&&previous[n].value==state.events[n].value);
            previousCount=state.eventCount;memcpy(previous,state.events,previousCount*sizeof(Event));++cases;
        }
    }
    assert(cases==110592u);pGlobalScriptDef.Reset(0);puts("00d2988b_TEST PASS");return 0;
}