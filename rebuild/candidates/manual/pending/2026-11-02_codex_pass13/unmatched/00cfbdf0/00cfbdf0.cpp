// NScript::CQ_AwakeningTheOracleScript::CNecropolisGhost::Main() -- guide grave ghosts through available digging spots.
// Leaf: ?Main@CNecropolisGhost@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ
// Grade: DIFFER(2306v2317) -- predicate destructor expansion and timer register allocation/cleanup differ; frame0x94 agrees.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CISTHINGALIVE
#define FABLE_RETAIL_CTIMER
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#include "engine/CQ_AwakeningTheOracleScript.h"
#include "engine/CTimer.h"
#include "engine/CIsThingAlive.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/TRetVoid.h"
enum ESex { SEX_NULL=0, SEX_MALE=1, SEX_FEMALE=2, NO_OF_SEXES=3 };
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&,const CScriptThing&,float);
CCharString FABLE_FASTCALL GFIntToCharString(long);
CCharString FABLE_FASTCALL operator+(const char*,const CCharString&);
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime()==time; }
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
template<> CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>&);
inline void CScriptThing::SetDataString(const CCharString& text) { if (!PImp.IsNull()) PImp->SetDataString(text); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}

void NScript::CQ_AwakeningTheOracleScript::CNecropolisGhost::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing DiggingPoint;
    ++ParentClass.GraveNumbersGivenOut;
    MyGraveNumber=ParentClass.GraveNumbersGivenOut;
    if (ParentClass.DiggingPoints.size()>ParentClass.PositionTaken) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        DiggingPoint=ParentClass.DiggingPoints[ParentClass.PositionTaken];
        ++ParentClass.PositionTaken;
    }
    {
        CScriptThing GhostStandsHere=Game.GetNearestWithScriptName(DiggingPoint,"GhostPos");
        GhostStandsHere.SetDataString("SpotTaken");
        Game.EntityTeleportToThing(Me,GhostStandsHere,false);
    }
    for (;!false;Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CTimer DiggingFlash;
        CTimer ConvDelay;
        while (Game.IsDiggingSpotEnabled(DiggingPoint)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (DiggingFlash==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.CreateEffect("Ghost_Appear_01",Me.GetPos(),"",0.0f,false,false);
                DiggingFlash=8;
            }
            if (ConvDelay==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),15.0f) && MyGraveNumber<13) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.CreateEffect("Ghost_Appear_01",Me.GetPos(),"",0.0f,false,false);
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                if (Game.EntityGetSex(Me)==SEX_MALE) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B03_GHOST_MALE_NEAR_GRAVE"+GFIntToCharString(MyGraveNumber),false,Me,Game.GetHero());
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B03_GHOST_NEAR_GRAVE"+GFIntToCharString(MyGraveNumber),false,Me,Game.GetHero());
                }
                ConvDelay=16;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.CreateEffect("Ghost_Appear_01",Me.GetPos(),"",0.0f,false,false);
        if (ParentClass.DiggingPoints.size()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            for (long i=0;i<ParentClass.DiggingPoints.size();++i) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                DiggingPoint=ParentClass.DiggingPoints[i];
                if (Game.GetNearestWithScriptName(DiggingPoint,"GhostPos").GetDataString()!="SpotTaken") {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    break;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing GhostStandsHere=Game.GetNearestWithScriptName(DiggingPoint,"GhostPos");
            if (GhostStandsHere.GetDataString()!="SpotTaken") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntityTeleportToThing(Me,GhostStandsHere,false);
                GhostStandsHere.SetDataString("SpotTaken");
                ++ParentClass.GraveNumbersGivenOut;
                MyGraveNumber=ParentClass.GraveNumbersGivenOut;
                Game.EntitySetFacingAngleTowardsThing(Me,DiggingPoint,false);
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.RemoveThing(Me,false,true);
            }
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.RemoveThing(Me,false,true);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
