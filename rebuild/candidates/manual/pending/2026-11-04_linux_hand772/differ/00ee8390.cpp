// NScript::CCS_OakValeRevisitedScript::OakVale() -- Oakvale-revisited cut-scene body: spawn the OakValeFire thread, teleport the hero to OV_HERO_START_MARKER, play CS_OAKVALE_REVISITED (hero cast, OakValeFlag flags) inside a paused movie sequence, remove the fires, deactivate CS_OakValeRevisited and remove every live "Fence"
// Grade: DIFFER(1224v1240) -- dl=12: retail expands vector<CScriptThing>::resize(n) (n, CScriptThing() 0x006E7B40, resize(n,const T&) 0x00D34AC0, ~CScriptThing 0x004AA840 called) while the GetThingWithScriptName temporary's ~CScriptThing is expanded; VC7.1 keeps resize(n) a call (INLINE_DEPTH_REVISIT "out-of-line callee inside an expanded inline" family)
// Leaf: ?OakVale@CCS_OakValeRevisitedScript@NScript@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE8390, 1240 bytes, thiscall, plain `ret`, no EH frame. Literals "OakValeFire" 0x012F5428,
//   "" 0x0122D70E, "OV_HERO_START_MARKER" 0x012F5410, "Hero" 0x012C4570, "CS_OAKVALE_REVISITED" 0x012F53F8,
//   "CS_OakValeRevisited" 0x012C5928, "Fence" 0x012F53F0.
// Identity (PROPOSAL-plus): Ego_r.pdb / FableWin.pdb name CCS_OakValeRevisitedScript's methods ctor, RegisterMain, OakValeFire,
//   OakVale, Main, dtor; retail ctor 0x00EE8100, RegisterMain 0x00EE8210, Main 0x00EE82D0 (its only caller, tail call), OakValeFire
//   0x00EE8870 (landed; this body spawns it as "OakValeFire" with the member pointer 0x00EE8870), so this is OakVale by elimination;
//   the seed-corpus Lua port OakValeRevisited.lua OakVale() has the same statements. Catalog DoMission_OakValeRevisited is wrong.
// Layout (retail = Ego_r - 0x14, 12-byte STLport containers): Game +0x40, FirePoint +0x48, Fires +0x54, OakValeFlag +0x60.
// Tried (8 grades): one-argument resize (a, dl=12: resize(n) called); explicit `resize(n, CScriptThing())` (b: temp dtor expanded);
//   __default_constructed form (c); a FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE header switch declaring resize(n,x) only (d: resize(n)
//   still called), + __forceinline resize(n) (d2: dtor expanded); ~CScriptThing declaration-only (e: resize matches, the
//   GetThingWithScriptName temporary's dtor becomes a call, dl=18); early explicit instantiation (f); implicit dtor (g).
//   Next idea: the mechanism behind retail's nested out-of-line dtor (same as the 0x00E16E00 CWB_Guard1 predicate park).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST,SCRIPT_AI_PRIORITY_LOW,SCRIPT_AI_PRIORITY_MEDIUM,SCRIPT_AI_PRIORITY_HIGH,SCRIPT_AI_PRIORITY_HIGHEST };
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#pragma auto_inline(off)
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()
{
 if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}
 Data=0;Info=0;
}
#pragma auto_inline(on)
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/fable_script_cutscene.h"
__forceinline CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp=other.PImp; return *this; }
inline CScriptThing::~CScriptThing() { }
namespace NScript {
class CQ_SunnyvaleMasterData;
class CCS_OakValeRevisitedScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void OakValeFire();                                                          // 0x00EE8870
    void OakVale();
    std::vector<CScriptThing> FirePoint;                                         // +0x48
    std::vector<CScriptThing> Fires;                                             // +0x54
    std::map<CCharString,bool> OakValeFlag;                                      // +0x60
    CScriptGameResourceObjectScriptedThingBase v1,v2,v3,v4,v5,v6,v7;            // +0x6c..
};
}
void NScript::CCS_OakValeRevisitedScript::OakVale()
{
    AddSpawnedFunction(new CSpawnedFunc<CCS_OakValeRevisitedScript>("OakValeFire", 0, this, &NScript::CCS_OakValeRevisitedScript::OakValeFire), CCharString(""));
    Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("OV_HERO_START_MARKER"), false);
    {
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities pause(*Game);
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> cast;
        Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
        cast["Hero"] = seh_hero;
        RunCutsceneMacro("CS_OAKVALE_REVISITED", cast, &OakValeFlag, 0, false, true);
    }
    Fires.resize(FirePoint.size());
    for (unsigned long i = 0; i < Fires.size(); ++i) {
        if (IsActiveThreadTerminating()) return;
        Game->RemoveThing(Fires[i], false, true);
    }
    if (IsActiveThreadTerminating()) return;
    Game->DeactivateQuestLater("CS_OakValeRevisited", 0);
    std::vector<CScriptThing> fences;
    Game->GetAllThingsWithScriptName("Fence", fences);
    for (unsigned long j = 0; j < fences.size(); ++j) {
        if (fences[j].IsAlive())
            Game->RemoveThing(fences[j], false, true);
    }
}
