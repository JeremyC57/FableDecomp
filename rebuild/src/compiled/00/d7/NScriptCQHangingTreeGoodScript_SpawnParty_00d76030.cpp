// NScript::CQ_HangingTreeGoodScript::SpawnParty(long,CScriptThing &) -- spawn the guard party at the supplied script thing.
// Leaf: ?SpawnParty@CQ_HangingTreeGoodScript@NScript@@QAEXJAAVCScriptThing@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST,SCRIPT_AI_PRIORITY_LOW,SCRIPT_AI_PRIORITY_MEDIUM,SCRIPT_AI_PRIORITY_HIGH,SCRIPT_AI_PRIORITY_HIGHEST };
#include "engine/C3DVector.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
#pragma auto_inline(off)
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()
{
    if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}
    Data=0;Info=0;
}
#pragma auto_inline(on)
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFunc.h"
#include "stlport/fable_map.h"
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp=other.PImp; return *this; }
inline bool CScriptThing::IsAlive()const{if(PImp.IsNull())return false;return PImp->IsAlive();}
void __fastcall RunCutsceneMacro(const CCharString&,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&,std::map<CCharString,bool>*,std::map<CCharString,CCharString>*,bool,bool);
#include "engine/CQ_HangingTreeGoodScript.h"
void NScript::CQ_HangingTreeGoodScript::SpawnParty(long RemainingGuards,CScriptThing& SpawnPoint)
{
    UpdateWayPoint();
    CScriptThing Executioner=Game->GetThingWithScriptName("HTG_Executioner");
    for(long i=0;i<RemainingGuards;++i) {
        if(IsActiveThreadTerminating()) return;
        CScriptThing NewGuard=Game->CreateCreatureNearby("CREATURE_BS_GUARD",SpawnPoint.GetPos(),7.0f,"HTG_Guard",false);
        Game->AddQuestInfoBarHealth(NewGuard,CRGBColour(255,255,255,255),"HUD_QUEST_ICON_GUARD",1.0f);
        Game->EntityAttachToScript(NewGuard,Game->GetActiveQuestName());
        if(Executioner.IsAlive()) Game->EntitySetThingAsAllyOfThing(NewGuard,Executioner);
        SpawnPoint=NewGuard;
    }
    if(IsActiveThreadTerminating()) return;
    Game->DisplayQuestInfo(true);
    ReassignLeader();
    Prisoner=Game->CreateCreatureNearby("CREATURE_PRISONER",SpawnPoint.GetPos(),1.0f,"HTG_Prisoner",false);
    Game->EntityAttachToScript(Prisoner,Game->GetActiveQuestName());
    PartySuccessfullySetUp=true;
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeGoodScript>("WatchForHeroNearExit",0,this,&CQ_HangingTreeGoodScript::WatchForHeroNearExit),CCharString(""));
}