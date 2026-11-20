// NScript::CV_RockTrollFirstEncounterScript::CRTFE_RockTroll::Main() -- the Witchwood rock troll: spawn the parent's WatchForRockTrollKilled thread on Me; take scripted control; give it the two RockTrollReward items once; play CS_ROCKTROLL_EXHUME (HERO / TROLL) once under a paused movie and spawn WatchForRockTrollHit; play SPECIAL_BOAST then SPECIAL_IDLE and wait; release control, face and target the hero, then idle
// Inline-budget: `#pragma inline_depth(0)` ... `#pragma inline_depth()` (inside `#ifdef _MSC_VER`) round an explicit plain-`inline` CSpawnedFunc1<CV_RockTrollFirstEncounterScript,CScriptThing> ctor specialization whose init list and body are token-identical to engine/CSpawnedFunc1.h's (the landed idiom of 0x00ED5F50 / 0x00E33EA0 / 0x00E2FF00) -- retail expands both spawn ctors but CALLS the nested Par1 CScriptThing copy 0x004ABE90 inside them (0x00EC4A97, 0x00EC4E58) while the WatchForRockTrollKilled by-value Me argument copy expands (0x00EC4A25..0x00EC4A44) and the WatchForRockTrollHit one is called (0x00EC4DFA). Without it (the header's forceinline ctor) DIFFER(1588v1540): c2 trace, nested copy f 1.0 c 25 score 25, expanded in the ctor's own pipe and again in Main; argument copy f 0.5 c 28 score 32, so no threshold separates them -- /d2inlT50/55/60/65/70 DIFFER(1540/1516/1516/1516/1588 v1540), /d2inlS15/30 DIFFER(1588/1620). Depth 0 is the smallest N (depth 1 also matches); the specialization's FABLE_FORCEINLINE (as the landed siblings) is not needed. Plus FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR (header opt-in: retail expands the pause ctor; plain inline calls it at f 0.0117: DIFFER(1532v1540)); FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY (without it every copy is called: DIFFER(1524v1540)); FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING (the plain header expands the table call: DIFFER(1556v1540)).
// Leaf: ?Main@CRTFE_RockTroll@CV_RockTrollFirstEncounterScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC4A00, 1540 bytes, thiscall, plain `ret`, no EH frame, default flags.
// Identity (two sources): slot 1 (Main) of the CRTFE_RockTroll entity vtable 0x012F16C4 stored by the landed Alloc 0x00EC4490; Ego_r.pdb
//   ?Main@CRTFE_RockTroll@CV_RockTrollFirstEncounterScript@NScript@@EAEXXZ (private virtual) and the Ego_d.pdb proc of the same name.
// Source shape: Ego_d.pdb line table (lines 186..232, ref egod_lines/) and locals: function scope `_Fail_Val` (TRetVoid), `seh_Troll`
//   (CScriptGameResourceObjectScriptedThingBase); one block {csEntMap (std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>),
//   seh_Hero, speech_movie (CScriptGameResourceObjectMovieBase), PauseMyEntitiesPlease (CWideScreenMagicPauseEntities)}. Generated quest-script
//   text as the landed Mains (0x00ED5F50, 0x00E07640): the spawn line (189, 510 bytes), the seh macro on one line (192), RunCutsceneMacro and the
//   second spawn on one line (215, 543 bytes), each terminate check on its statement's line.
// Callees: operator new 0x00BFEA1A; CCharString(const char*, long) 0x0099EBF0 / operator+ 0x0099F570 / ~ 0x0099EAE0; CSpawnedFuncBase ctor
//   0x00CDD450; CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840; CScriptBase::AddSpawnedFunction 0x00CB7E50; CScriptGameResourceObjectBase
//   ctor 0x0099A380 / dtor 0x0099A430; CScriptGameResourceObjectScriptedThingBase IsValid 0x00CD23B9 / Release 0x00CD2770 / operator= 0x008ABD10 (this TU's COMDAT of the header's inline body, byte-equal) /
//   ~ 0x007E74D0 (expanded at the first loop's exit, 0x00EC4CB5) / PlayAnimation 0x007E73D0 / IsPerformingScriptTask 0x007E7450;
//   CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; map ctor 0x00CDBF70 / operator[] 0x00CD3D2E / dtor 0x00CDBFB0; movie ctor 0x006E7B60 /
//   ~ 0x006E7B80 (this TU's COMDATs, byte-equal); RunCutsceneMacro 0x00CBFB7D; Game (+4) slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 370 StartMovieSequence,
//   379 (pause entities, the expanded CWideScreenMagicPauseEntities ctor / dtor), 451 GiveThingBestEnemyTarget, 498 EntityForceToLookAtThing,
//   585 AddItemToContainer; pGlobalScriptDef 0x0143E90C (RockTrollReward1 +0x730, RockTrollReward2 +0x734);
//   NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748; CDefString::GetString 0x00415D70 behind the expanded conversion
//   (FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING, the NScript-TU binding of 2026-11-19; the plain header expands the table call, DIFFER(1556v1540)).
// Layout: engine/CV_RockTrollFirstEncounterScript.h (NEW in this batch; Ego_r.pdb - 0x14): PlayedExhumeCutScene +0x54, AddedItemsToRockTroll +0x55;
//   CRTFE_RockTroll ParentClass +0x14. CSpawnedFunc1 Func +0x34, PClass +0x38, Par1 +0x3C (engine/CSpawnedFunc1.h).
// Header contract: FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY (retail expands the copy in this TU: the argument copy here and the instantiation's
//   SuspendableProcess 0x00EC52F0; without it every copy is called, DIFFER(1524v1540)); FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
//   (retail expands the pause ctor at this f 0.0117 site; plain inline calls it, DIFFER(1532v1540)).
// No `// Inferred:` lines.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING
#define FABLE_RETAIL_STLP_MAP
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CV_RockTrollFirstEncounterScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_animation_defaults.h"
#include "engine/fable_script_def_global.h"
#include "engine/TRetVoid.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();                         // 0x00CDBF70
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();                        // 0x00CDBFB0

#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
template<> inline CSpawnedFunc1<NScript::CV_RockTrollFirstEncounterScript,CScriptThing>::CSpawnedFunc1(const CCharString& name, unsigned long id, NScript::CV_RockTrollFirstEncounterScript* pclass, void (NScript::CV_RockTrollFirstEncounterScript::*func)(CScriptThing), CScriptThing par1)
    : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif

void NScript::CV_RockTrollFirstEncounterScript::CRTFE_RockTroll::Main()
{
    TRetVoid _Fail_Val;
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_RockTrollFirstEncounterScript,CScriptThing>(CCharString("ParentClass.")+CCharString("WatchForRockTrollKilled"),0,&ParentClass,&CV_RockTrollFirstEncounterScript::WatchForRockTrollKilled,Me),CCharString(""));
    CScriptGameResourceObjectScriptedThingBase seh_Troll;
    if(seh_Troll.IsValid())seh_Troll.Release();while(!Game->StartScriptingEntity(Me,seh_Troll,SCRIPT_AI_PRIORITY_HIGHEST)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();
    if(!ParentClass.AddedItemsToRockTroll){if(IsActiveThreadTerminating())return _Fail_Val.Return();
        Game->AddItemToContainer(Me,pGlobalScriptDef->RockTrollReward1);
        Game->AddItemToContainer(Me,pGlobalScriptDef->RockTrollReward2);
        ParentClass.AddedItemsToRockTroll=true;
    }
    if(!ParentClass.PlayedExhumeCutScene){if(IsActiveThreadTerminating())return _Fail_Val.Return();
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        if(seh_Hero.IsValid())seh_Hero.Release();while(!Game->StartScriptingEntity(Game->GetHero(),seh_Hero,SCRIPT_AI_PRIORITY_HIGHEST)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> csEntMap;
        csEntMap["HERO"]=seh_Hero;
        csEntMap["TROLL"]=seh_Troll;
        CScriptGameResourceObjectMovieBase speech_movie;Game->StartMovieSequence("",speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        RunCutsceneMacro("CS_ROCKTROLL_EXHUME",csEntMap,0,0,false,true);ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_RockTrollFirstEncounterScript,CScriptThing>(CCharString("ParentClass.")+CCharString("WatchForRockTrollHit"),0,&ParentClass,&CV_RockTrollFirstEncounterScript::WatchForRockTrollHit,Me),CCharString(""));
        ParentClass.PlayedExhumeCutScene=true;
    }
    seh_Troll.PlayAnimation("SPECIAL_BOAST",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
    seh_Troll.PlayAnimation("SPECIAL_IDLE",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
    while(seh_Troll.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();
    if(seh_Troll.IsValid())seh_Troll.Release();
    Game->EntityForceToLookAtThing(Me,Game->GetHero());
    Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
    while(true){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}
}
