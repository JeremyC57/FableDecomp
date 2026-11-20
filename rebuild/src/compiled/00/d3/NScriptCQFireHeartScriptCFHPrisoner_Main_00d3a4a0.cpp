// NScript::CQ_FireHeartScript::CFH_Prisoner::Main() -- a Fire Heart prisoner (prophet): under an alive filter on Me take scripted control into the quest's seh_Prophet[MyCageNumber] and count itself scripted; wait (commenting when asked) until the puzzle round reaches its cage, become the guest and make the round-start comment; then until chosen: when released say the end speech into csargs, set the camera and play CS_FIREHEART_RELEASE (freed count, one sun less, history false, morality), when killed play its end cutscene (killed count, one sun more, history true, morality), else comment when asked; finally advance the round, spawn the parent's CheckCompletion thread, remove itself and quit the Fire Heart minigame
// Leaf: ?Main@CFH_Prisoner@CQ_FireHeartScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D3A4A0, 1140 bytes, thiscall, plain `ret`, no EH frame.
// Identity: slot 1 (Main) of the CFH_Prisoner entity vtable 0x012CCF10 stored by the landed Alloc 0x00D36540; Ego_r.pdb / Ego_d.pdb /
//   FableWin.pdb ?Main@CFH_Prisoner@CQ_FireHeartScript@NScript@@EAEXXZ.
// Source shape: Ego_d.pdb line table (lines 961..1022) and locals (_Fail_Val; CutsceneName, iAnim in the killed branch), one source
//   line per Ego_d line: 963 predicate + frame + terminate test (134 bytes), 965 the scripting start (release if valid, the
//   StartScriptingEntity wait, the terminate test; 248), 966 ProphetsScripted++ (33), 969 / 982 / 984 / 996 / 1010 loop or branch
//   heads with their terminate tests, 986..994 and 998..1009 the branch bodies, 1016..1021 the tail.
// Callees: CIsThingAlive (vtable 0x012C32F0; ctor and dtor expanded: CScriptThing copy 0x004ABE90, ~CScriptThing 0x004AA840),
//   CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CScriptGameResourceObjectScriptedThingBase
//   IsValid 0x00CD23B9 / Release 0x00CD2770, CFH_Prisoner MakeComment 0x00D3ACE0 / MakeRoundStartComment 0x00D3A9B0 / GetEndSpeech
//   0x00D3ABE0 / GetEndCutscene 0x00D3AB00, CQ_FireHeartScript PlayCutscene 0x00D39330 / UpdateHistory 0x00D3A920 / CheckCompletion
//   0x00D3AD60 (member pointer), map<CCharString,CCharString>::operator[] 0x009AC700, CCharString 0x0099EBF0 / copy 0x0099EC30 /
//   ~ 0x0099EAE0 / operator= 0x0099EFB0, GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690,
//   operator+(const CCharString&, const CCharString&) 0x0099F570, rand 0x00BFEB16 (result dead), operator new 0x00BFEA1A,
//   CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CQ_FireHeartScript> vtable 0x012CCBA4), CScriptBase::AddSpawnedFunction 0x00CB7E50;
//   CGameScriptInterfaceBase slots 7 NewScriptFrame, 8 StartScriptingEntity, 108 RemoveThing, 156 GiveHeroMorality, 255
//   HeroQuitFireheartMinigame.
// Header: engine/CQ_FireHeartScript.h new opt-in FABLE_RETAIL_CQ_FIREHEARTSCRIPT_PRISONER (UpdateHistory, CheckCompletion and the
//   CFH_Prisoner class: Ego_r.pdb members ParentClass +0x14, PMasterData +0x18, Chosen +0x1c, MyCageNumber +0x20; 0x24 bytes).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CQ_FIREHEARTSCRIPT_PRISONER
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#include <stdlib.h>
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_map.h"
#include "engine/CQ_FireHeartScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/fable_script_text.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
// Inline-budget: __declspec(noinline) on IsValid's real body (PImp.Data != 0; retail COMDAT 0x00CD23B9, not byte-equal here: 11v9) (the landed
//   0x00EB3BA0 text, GOTCHAS chunk Q32): c2 sees that IsValid stores nothing and computes &seh_Prophet[MyCageNumber] once for IsValid
//   and Release (`mov ecx,[esi+20h] ; shl ecx,4 ; lea edi,[edx+ecx]`, 0x00D3A4FA..0x00D3A506), as retail. With the declaration only,
//   the address is recomputed for Release (1145v1140); a named reference local (the 2026-11-08 drafts; Ego_d.pdb has none) targets
//   edi directly (`mov edi,[esi+20h] ; shl edi,4 ; add edi,edx`, 1140v1140, 87 grades).
inline __declspec(noinline) bool CScriptGameResourceObjectScriptedThingBase::IsValid() const
{
    return PImp.Data != 0;
}

void NScript::CQ_FireHeartScript::CFH_Prisoner::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
    if(ParentClass->seh_Prophet[MyCageNumber].IsValid())ParentClass->seh_Prophet[MyCageNumber].Release();while(!Game->StartScriptingEntity(Me,ParentClass->seh_Prophet[MyCageNumber],SCRIPT_AI_PRIORITY_HIGHEST)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();
    ParentClass->ProphetsScripted++;
    while(ParentClass->PuzzleRound<MyCageNumber){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
        if(ParentClass->ShouldProphetSpeak[MyCageNumber]){if(IsActiveThreadTerminating())return _Fail_Val.Return();
            MakeComment();
        }
    }
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    ParentClass->Guest=MyCageNumber;
    MakeRoundStartComment();
    while(!Chosen){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
        if(ParentClass->ProphetsReleased[MyCageNumber]){if(IsActiveThreadTerminating())return _Fail_Val.Return();
            GetEndSpeech(ParentClass->csargs["$LINE"]);
            ParentClass->csargs["$CAMERA"]="CAM_FH_PRIS"+GFIntToCharString(MyCageNumber);
            ParentClass->PlayCutscene("CS_FIREHEART_RELEASE");
            ParentClass->FreedProphets++;
            ParentClass->StartNumberOfSuns--;
            ParentClass->UpdateHistory(false);
            Chosen=true;
            ParentClass->KilledAllPrisoners=false;
            Game->GiveHeroMorality(ParentClass->MoralityChangeOnFreed);
        }
        else if(ParentClass->ProphetsKilled[MyCageNumber]){if(IsActiveThreadTerminating())return _Fail_Val.Return();
            // Inferred: the divisor -- the value is dead (retail keeps only the rand call, 0x00D3A75E); Ego_d line 998 is 20 bytes, the
            //   /Od size of `rand()%2^k` (an odd divisor is 16).
            long iAnim=rand()%2;
            CCharString CutsceneName("");
            GetEndCutscene(CutsceneName);
            ParentClass->PlayCutscene(CutsceneName);
            ParentClass->KilledProphets++;
            ParentClass->StartNumberOfSuns++;
            ParentClass->UpdateHistory(true);
            Chosen=true;
            ParentClass->FreedAllPrisoners=false;
            Game->GiveHeroMorality(ParentClass->MoralityChangeOnKilled);
        }
        else if(ParentClass->ShouldProphetSpeak[MyCageNumber]){if(IsActiveThreadTerminating())return _Fail_Val.Return();
            MakeComment();
        }
    }
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    ParentClass->PuzzleRound++;
    ParentClass->AddSpawnedFunction(new CSpawnedFunc<CQ_FireHeartScript>(CCharString("ParentClass.")+CCharString("CheckCompletion"),0,ParentClass,&CQ_FireHeartScript::CheckCompletion),CCharString(""));
    Game->RemoveThing(Me,false,true);
    Game->HeroQuitFireheartMinigame();
}