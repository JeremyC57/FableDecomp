// CWomanToAttract::DoLeavingCutscene() -- the laughing woman leaves: take the hero under scripted control, cast him as HERO and the woman's own handle as WOMAN, pass the ingredient object as the $ITEM argument and run CS_SICK_LAUGHING_WOMAN_LEAVES with the movie camera fixed
// Leaf: ?DoLeavingCutscene@CWomanToAttract@CV_SickChildScript@NScript@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED0B10, 747 bytes, plain `ret`.
// Callees: CBaseObject ctor/dtor/operator= 0x0099A380 / 0x0099A430 / 0x0099A3B0, CCharString 0x0099EBF0 / 0x0099EAE0 /
//   operator= 0x0099EFB0, CDefString::GetString() 0x00415D70, CRT malloc 0x00BFEA0E / free 0x00BFEA14 (map headers),
//   operator delete 0x00BFE9BC, cast map operator[] 0x00CD3D2E / _M_erase 0x00CD344A, argument map operator[] 0x009AC700 /
//   _M_erase 0x009AA830, cut-scene runner 0x00CBFB7D (__fastcall); CGameScriptInterface slots 8 StartScriptingEntity,
//   70 GetHero, 371 FixMovieSequenceCamera. Literals "HERO" 0x01255174, "WOMAN" 0x012F2B04, "$ITEM" 0x012F2AFC,
//   "CS_SICK_LAUGHING_WOMAN_LEAVES" 0x012F2ADC; pGlobalScriptDef 0x0143E90C (IngredientObject at retail +0x738, as landed).
// Layout: Ego_r.pdb CWomanToAttract (52 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData
//   +0x18, GivenObject +0x1c, TimesMadeLaugh +0x20, seh_me +0x24.
// Shape: the landed CQ_WaspBossScript::WaspIntro 0x00E12F20 model; both local maps (cast, then arguments) are destroyed
// NF87 (2026-11-17): moved onto the shared engine/CV_SickChildScript.h (nested NScript::CV_SickChildScript::CWomanToAttract, private,
//   as Ego_r.pdb + FableWin.pdb spell it: ?DoLeavingCutscene@CWomanToAttract@CV_SickChildScript@NScript@@AAEXXZ) with STLport
//   std::map cast / argument maps (stlport/fable_map.h), so the argument map's operator[] binds 0x009AC700 under its real spelling,
//   spelled as the landed shared-header cutscene rows (0x00D6D670, 0x00D33730, NF87 0x00E6DA80).
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "stlport/fable_map.h"
#include "engine/CV_SickChildScript.h"
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&, std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&, std::map<CCharString,bool>*, std::map<CCharString,CCharString>*, bool, bool);   // 0x00CBFB7D

void NScript::CV_SickChildScript::CWomanToAttract::DoLeavingCutscene()
{
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
    cast["HERO"] = hero;
    cast["WOMAN"] = seh_me;
    std::map<CCharString, CCharString> args;
    args["$ITEM"] = pGlobalScriptDef->IngredientObject;
    Game->FixMovieSequenceCamera(true);
    RunCutsceneMacro("CS_SICK_LAUGHING_WOMAN_LEAVES", cast, 0, &args, false, true);
    Game->FixMovieSequenceCamera(false);
}
