// NScript::CQ_RansomVictimScript::PlayCutscene(CCharString name, CCharString arg) -- play one of the Ransom Victim quest's cut-scenes: take the hero under scripted control, pass `arg` as the cut-scene's $ARG1 unless it equals "NULL" (case-insensitive), cast the hero as HERO and the quest's guard, father, victim, brother and ransommer handles as GUARD / CHIEF / VICTIM / BROTHER / BANDIT, then run the named cut-scene macro with the movie camera fixed
// Leaf: ?PlayCutscene@CQ_RansomVictimScript@NScript@@QAEXVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE6730, 1252 bytes, thiscall `ret 8` (both by-value strings are destroyed by the callee).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?PlayCutscene@CQ_RansomVictimScript@NScript@@QAEXVCCharString@@0@Z (two by-value
//   strings = `ret 8`; engine/CQ_RansomVictimScript.h declares it); the body sits between the landed DisappearFollowers-side rows
//   and CQ_RansomVictimScript::Main's threads (0x00DE4710 .. 0x00DEB280) and its cast reads this+0x6c / +0x7c / +0x8c / +0x9c /
//   +0xac = seh_Guard / seh_Father / seh_Victim / seh_Brother / seh_Ransommer (Ego_r.pdb - 0x14). The catalog label
//   CV_ExposeMayorScript::PlayCutscene is a propagated name: that function is 0x00E78100 (three parameters, `ret 0Ch`).
// Callees: CBaseObject ctor / operator= / dtor 0x0099A380 / 0x0099A3B0 / 0x0099A430, CCharString 0x0099EBF0 / 0x0099EAE0 /
//   operator= 0x0099EFB0, _stricmp 0x00BFEBA8 (the inline CCharString::EqualsCaseInsensitive on the implicit "NULL" temporary), malloc 0x00BFEA0E / free 0x00BFEA14 (map
//   headers), operator delete 0x00BFE9BC, map<CCharString, CCharString>::operator[] 0x009AC700 / _M_erase 0x009AA830,
//   map<CCharString, CScriptGameResourceObjectScriptedThingBase>::operator[] 0x00CD3D2E / _M_erase 0x00CD344A,
//   RunCutsceneMacro 0x00CBFB7D (__fastcall); Game (+0x40) slots 8 StartScriptingEntity, 70 GetHero, 371 FixMovieSequenceCamera.
// Data: "NULL" 0x012393E4, "$ARG1" 0x012DD73C, "HERO" 0x01255174, "GUARD" 0x012CB384, "CHIEF" 0x012CB38C, "VICTIM" 0x012DD734,
//   "BROTHER" 0x012DD72C, "BANDIT" 0x012D5AB0, "" 0x0129AAF4 (NullChar).
// Shape: the landed CV_SickChildScript::PlayCutscene 0x00ECE460 body (csargs, hero, "$ARG1", cast) on the landed
//   CQ_FireHeartScript::PlayCutscene 0x00D39330 shared-header model.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST,SCRIPT_AI_PRIORITY_LOW,SCRIPT_AI_PRIORITY_MEDIUM,SCRIPT_AI_PRIORITY_HIGH,SCRIPT_AI_PRIORITY_HIGHEST };
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_map.h"
#include "engine/CQ_RansomVictimScript.h"
#include "engine/fable_script_cutscene.h"
__forceinline CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp=other.PImp; return *this; }

void NScript::CQ_RansomVictimScript::PlayCutscene(CCharString name, CCharString arg)
{
    std::map<CCharString, CCharString> csargs;
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, SCRIPT_AI_PRIORITY_HIGHEST);
    if (!arg.EqualsCaseInsensitive("NULL"))
        csargs["$ARG1"] = arg;
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
    cast["HERO"] = hero;
    cast["GUARD"] = seh_Guard;
    cast["CHIEF"] = seh_Father;
    cast["VICTIM"] = seh_Victim;
    cast["BROTHER"] = seh_Brother;
    cast["BANDIT"] = seh_Ransommer;
    Game->FixMovieSequenceCamera(true);
    RunCutsceneMacro(name, cast, 0, &csargs, false, true);
    Game->FixMovieSequenceCamera(false);
}