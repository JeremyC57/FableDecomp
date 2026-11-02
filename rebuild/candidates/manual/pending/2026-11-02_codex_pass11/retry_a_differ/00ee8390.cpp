// NScript::CCS_OakValeRevisitedScript::OakVale() -- run the revisited OakVale cutscene and remove fire and fence entities.
// Leaf: ?OakVale@CCS_OakValeRevisitedScript@NScript@@QAEXXZ
// Grade: DIFFER(1224v1240) -- inline budget: retail inlines the one-argument Fires.resize(n) wrapper with ctor 0x006E7B40 / resize(n,x) 0x00D34AC0 / dtor 0x004AA840 calls while inlining ~CScriptThing at the teleport temporary; here resize(n) stays a call
// Context: grades against the Codex row context $S/codex11_dl/x/rows/00ee8390/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = Codex selected; 00ee8390 probes in probe/00ee8390.log
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SCRIPT_QUEST
#define FABLE_RETAIL_OAKVALE
#define FABLE_RETAIL_OAKVALE_ALLOCATOR_LIFETIME
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_STLPORT_HTTP
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CCharString.h"
#include "engine/TRetVoid.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
#pragma auto_inline(off)
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()
{
    if(Info) {
        --Info->RefCount;
        if(Info->RefCount==0) {Info->DeleteFunc(Info->Data);delete Info;}
    }
    Data=0;Info=0;
}
#include "engine/CScriptGameResourceObjectMovieBase.h"
#pragma auto_inline(on)
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CCS_OakValeRevisitedScript.h"
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
void __fastcall RunCutsceneMacro(const CCharString&,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&,std::map<CCharString,bool>*,std::map<CCharString,CCharString>*,bool,bool);
void NScript::CCS_OakValeRevisitedScript::OakVale()
{
    TRetVoid _Fail_Val;
    AddSpawnedFunction(new CSpawnedFunc<CCS_OakValeRevisitedScript>("OakValeFire",0,this,&CCS_OakValeRevisitedScript::OakValeFire),CCharString(""));
    Game.EntityTeleportToThing(Game.GetHero(),Game.GetThingWithScriptName("OV_HERO_START_MARKER"),false);
    {
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("",speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        Game.StartScriptingEntity(Game.GetHero(),seh_Hero,(EScriptAIPriority)4);
        mymap["Hero"]=seh_Hero;
        RunCutsceneMacro("CS_OAKVALE_REVISITED",mymap,&OakValeFlag,0,false,true);
    }
    Fires.resize(FirePoint.size());
    for(int c=0;c<Fires.size();++c) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.RemoveThing(Fires[c],false,true);
    }
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.DeactivateQuestLater("CS_OakValeRevisited",0);
    std::vector<CScriptThing> fence;
    Game.GetAllThingsWithScriptName("Fence",fence);
    for(int c=0;c<fence.size();++c) {
        if(fence[c].IsAlive()) Game.RemoveThing(fence[c],false,true);
    }
}
