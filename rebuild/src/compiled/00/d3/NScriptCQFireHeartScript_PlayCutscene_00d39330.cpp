// NScript::CQ_FireHeartScript::PlayCutscene(CCharString name) -- play a Fire Heart cutscene: take the hero under scripted control, cast him as HERO with the guest and five prophets, Briar Rose, the Scythe and the Guildmaster, run the named cutscene with the quest's cutscene flags and csargs inside a paused movie sequence with a fixed camera, then clear csargs
// Leaf: ?PlayCutscene@CQ_FireHeartScript@NScript@@QAEXVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D39330, 1578 bytes (ret 4: the callee destroys the by-value name); RELOCATION_MATCH, every relocation bound-same, 11 literals equal.
// Identity: 9 callers in CQ_FireHeartScript (Main 0x00D35090 and its threads); Ego_r.pdb + FableWin.pdb
//   ?PlayCutscene@CQ_FireHeartScript@NScript@@QAEXVCCharString@@@Z. The second label CScriptBase::PlayCutscene at this address is wrong.
// Callees: CBaseObject ctor/operator=/dtor 0x0099A380 / 0x0099A3B0 / 0x0099A430, CCharString 0x0099EBF0 / 0x0099EAE0, malloc 0x00BFEA0E,
//   free 0x00BFEA14, operator delete 0x00BFE9BC, cast map operator[] 0x00CD3D2E / _M_erase 0x00CD344A, map<CCharString,CCharString>::clear
//   0x009AACE0 (declared-only specialization), movie ctor 0x006E7B60, movie PImp dtor 0x006E7AB0, RunCutsceneMacro 0x00CBFB7D (__fastcall).
// Layout: engine/CQ_FireHeartScript.h: CutsceneFlags +0x4C, csargs +0x58, seh_Prophet / seh_Rose / seh_Scythe / seh_GM handles.
// Shape: the landed CQ_WaspBossScript intro/outro model (malloc'd cast-map header, CWideScreenMagicPauseEntities pause pair).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST,SCRIPT_AI_PRIORITY_LOW,SCRIPT_AI_PRIORITY_MEDIUM,SCRIPT_AI_PRIORITY_HIGH,SCRIPT_AI_PRIORITY_HIGHEST };
#include "stlport/fable_containers.h"
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
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_map.h"
#include "engine/CQ_FireHeartScript.h"
#include "engine/fable_script_cutscene.h"
__forceinline CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp=other.PImp; return *this; }
template<> void std::map<CCharString, CCharString>::clear();   // 0x009AACE0

void NScript::CQ_FireHeartScript::PlayCutscene(CCharString name)
{
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, SCRIPT_AI_PRIORITY_HIGHEST);
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
    cast["HERO"] = hero;
    cast["GUEST"] = seh_Prophet[Guest];
    cast["PROPHET1"] = seh_Prophet[0];
    cast["PROPHET2"] = seh_Prophet[1];
    cast["PROPHET3"] = seh_Prophet[2];
    cast["PROPHET4"] = seh_Prophet[3];
    cast["PROPHET5"] = seh_Prophet[4];
    cast["ROSE"] = seh_BriarRose;
    cast["SCYTHE"] = seh_Scythe;
    cast["GM"] = seh_GuildMaster;
    CScriptGameResourceObjectMovieBase movie;
    Game->StartMovieSequence("", movie);
    CWideScreenMagicPauseEntities pause(*Game);
    Game->FixMovieSequenceCamera(true);
    RunCutsceneMacro(name, cast, &CutsceneFlags, &csargs, false, true);
    Game->FixMovieSequenceCamera(false);
    csargs.clear();
}