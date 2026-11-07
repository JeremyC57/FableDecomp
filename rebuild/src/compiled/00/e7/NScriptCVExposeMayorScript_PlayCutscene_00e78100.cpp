// NScript::CV_ExposeMayorScript::PlayCutscene(CCharString name, bool fade, CCharString arg) -- play one of the Expose-the-Mayor cut-scenes: take the hero under scripted control, pass `arg` as the cut-scene's $ARG1 unless it equals "NULL" (case-insensitive), cast the hero as HERO and the quest's suspicious villager, sheriff, Lady Grey and Rhodri handles as PRIS / SHER / LADY / RHODRI, then run the named cut-scene macro with the movie camera fixed, inside a paused movie sequence when `fade` is set
// Leaf: ?PlayCutscene@CV_ExposeMayorScript@NScript@@QAEXVCCharString@@_N0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E78100, 1323 bytes, thiscall `ret 0Ch` (both by-value strings are destroyed by the callee).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?PlayCutscene@CV_ExposeMayorScript@NScript@@QAEXVCCharString@@_N0@Z (string, bool,
//   string = `ret 0Ch`); the landed PhaseThree 0x00E7A9F0, PhaseFive 0x00E7A740 and CEM_Sheriff::Main 0x00E78630 call it with
//   (name, bool, "NULL"); the cast entries read this+0x58 / +0x78 / +0x68 / +0x88 = seh_Suspicious / seh_Sheriff / seh_LadyGrey /
//   seh_Rhodri (engine/CV_ExposeMayorScript.h, Ego_r.pdb - 0x14).
// Callees: CBaseObject ctor / operator= / dtor 0x0099A380 / 0x0099A3B0 / 0x0099A430, CCharString 0x0099EBF0 / 0x0099EAE0 /
//   operator= 0x0099EFB0, _stricmp 0x00BFEBA8 (the inline CCharString::EqualsCaseInsensitive on the implicit "NULL" temporary), malloc 0x00BFEA0E / free 0x00BFEA14 (map
//   headers), operator delete 0x00BFE9BC, map<CCharString, CCharString>::operator[] 0x009AC700 / _M_erase 0x009AA830,
//   map<CCharString, CScriptGameResourceObjectScriptedThingBase>::operator[] 0x00CD3D2E / _M_erase 0x00CD344A, the movie PImp
//   dtor 0x006E7AB0 (declared-only specialization inside the expanded implicit ~CScriptGameResourceObjectMovieBase),
//   RunCutsceneMacro 0x00CBFB7D (__fastcall); Game (+0x40) slots 8 StartScriptingEntity, 70 GetHero, 370 StartMovieSequence,
//   371 FixMovieSequenceCamera, 379 PauseAllNonScriptedEntities (the inline CWideScreenMagicPauseEntities, Game kept in edi).
// Data: "NULL" 0x012393E4, "$ARG1" 0x012DD73C, "HERO" 0x01255174, "PRIS" 0x012D24F4, "SHER" 0x012E9B98, "LADY" 0x012E9B90,
//   "RHODRI" 0x012E9B88, "" 0x0129AAF4 (NullChar) / 0x0122D70E.
// Shape: the landed CV_SickChildScript::PlayCutscene 0x00ECE460 body (csargs, hero, "$ARG1", cast) on the landed
//   CQ_FireHeartScript::PlayCutscene 0x00D39330 shared-header model, with the CQ_AwakeningTheOracleScript 0x00CFF9A0
//   `if (fade) { movie; pause; run } else run` split.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST,SCRIPT_AI_PRIORITY_LOW,SCRIPT_AI_PRIORITY_MEDIUM,SCRIPT_AI_PRIORITY_HIGH,SCRIPT_AI_PRIORITY_HIGHEST };
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0, called inside the expanded movie dtor
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_map.h"
#include "engine/CV_ExposeMayorScript.h"
#include "engine/fable_script_cutscene.h"
__forceinline CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp=other.PImp; return *this; }

void NScript::CV_ExposeMayorScript::PlayCutscene(CCharString name, bool fade, CCharString arg)
{
    std::map<CCharString, CCharString> csargs;
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, SCRIPT_AI_PRIORITY_HIGHEST);
    if (!arg.EqualsCaseInsensitive("NULL"))
        csargs["$ARG1"] = arg;
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
    cast["HERO"] = hero;
    cast["PRIS"] = seh_Suspicious;
    cast["SHER"] = seh_Sheriff;
    cast["LADY"] = seh_LadyGrey;
    cast["RHODRI"] = seh_Rhodri;
    if (fade) {
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities pause(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro(name, cast, 0, &csargs, false, true);
        Game->FixMovieSequenceCamera(false);
    } else {
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro(name, cast, 0, &csargs, false, true);
        Game->FixMovieSequenceCamera(false);
    }
}