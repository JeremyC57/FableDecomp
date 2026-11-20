// NScript::CQ_FireHeartScript::CFH_Prisoner::GetEndSpeech(CCharString&) const -- the prophet's end-speech text tag: TEXT_QST_B01_PROPHET<round+1>, then
//   _FREED in the first round, else _FREED_ASEXPECTED / _FREED_DEFAULT / _FREED_SURPRISE by the mission history
// Leaf: ?GetEndSpeech@CFH_Prisoner@CQ_FireHeartScript@NScript@@ABEXAAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D3ABE0. Callees: GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690,
//   CCharString::operator=, operator+=(const char*) 0x0099F100, ~CCharString, CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30.
// Naming (NF129, 2026-11-20, q4627): 0x0099F100 is CCharString::operator+=(const char*) (Ego_r.pdb 0x00AFA9A0 + FableWin.pdb 0x02F619E0
//   ??YCCharString@@QAEABV0@PBD@Z, declared by fable_string.h).
// Naming (NF162, 2026-11-20, q11711): was a row-local global `struct CFH_Prisoner` (leaf ?GetEndSpeech@CFH_Prisoner@@QBEXAAUCPrisonerSpeechString@@@Z,
//   a CCharString-derived CPrisonerSpeechString adapter as the parameter, and a CPrisonerQuestState parent view with casts of `this` to a
//   local CEntityScriptBase); now on the shared engine/CQ_FireHeartScript.h opt-in FABLE_RETAIL_CQ_FIREHEARTSCRIPT_PRISONER (nested
//   NScript::CQ_FireHeartScript::CFH_Prisoner, private const member taking CCharString&; the parent's PuzzleRound +0x88 and MissionHistory
//   +0x184) and engine/CEntityScriptBase.h. Identity (signature from two sources): Ego_r.pdb 0x00D43780 + FableWin.pdb 0x03702450
//   ?GetEndSpeech@CFH_Prisoner@CQ_FireHeartScript@NScript@@ABEXAAVCCharString@@@Z; its only landed referencer CFH_Prisoner::Main
//   0x00D3A4A0 binds this spelling.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CQ_FIREHEARTSCRIPT_PRISONER
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_FireHeartScript.h"
#include "engine/fable_script_text.h"                  // GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690

void NScript::CQ_FireHeartScript::CFH_Prisoner::GetEndSpeech(CCharString& out) const {
 out="TEXT_QST_B01_PROPHET"+GFIntToCharString(ParentClass->PuzzleRound+1);
 if(ParentClass->PuzzleRound==0){if(IsActiveThreadTerminating())return;out += "_FREED";return;}
 if(IsActiveThreadTerminating())return;
 if(ParentClass->MissionHistory==1){if(IsActiveThreadTerminating())return;out += "_FREED_ASEXPECTED";return;}
 if(ParentClass->MissionHistory==2){if(IsActiveThreadTerminating())return;out += "_FREED_DEFAULT";return;}
 if(ParentClass->MissionHistory==3){if(IsActiveThreadTerminating())return;out += "_FREED_SURPRISE";return;}
 IsActiveThreadTerminating();
}
