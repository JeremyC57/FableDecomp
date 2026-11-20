// NScript::CQ_FireHeartScript::CFH_Prisoner::GetEndCutscene(CCharString&) const -- form cage cutscene and append history variant.
// Leaf: ?GetEndCutscene@CFH_Prisoner@CQ_FireHeartScript@NScript@@ABEXAAVCCharString@@@Z
// Native00D3AB00,213 bytes. String helpers99F830,99F690,99EFB0,99EAE0,99F100; cancellationF35B30.
// Donor CFH_Prisoner MyCageNumber+20,ParentClass+14; native matches. Parent history at184 needs donor container-size mapping audit.
// Naming (NF129, 2026-11-20, q4627): 0x0099F100 is CCharString::operator+=(const char*) (Ego_r.pdb 0x00AFA9A0 + FableWin.pdb 0x02F619E0
//   ??YCCharString@@QAEABV0@PBD@Z, declared by fable_string.h); the field-free CharStringMethods adapter's Append stand-in is replaced by the
//   header operator (fable_string.h retail branch, FABLE_RETAIL_CCHARSTRING).
// Naming (NF162, 2026-11-20, q11710): was a row-local global `struct CFH_Prisoner` (leaf ?GetEndCutscene@CFH_Prisoner@@QBEXAAVCCharString@@@Z)
//   over a local CEntityScriptBase and a FireHeartRetailView parent; now on the shared engine/CQ_FireHeartScript.h opt-in
//   FABLE_RETAIL_CQ_FIREHEARTSCRIPT_PRISONER (nested NScript::CQ_FireHeartScript::CFH_Prisoner, private const member; ParentClass +0x14,
//   MyCageNumber +0x20; the parent's MissionHistory +0x184) and engine/CEntityScriptBase.h. Identity: Ego_r.pdb 0x00D43640 + FableWin.pdb
//   0x037021B0 ?GetEndCutscene@CFH_Prisoner@CQ_FireHeartScript@NScript@@ABEXAAVCCharString@@@Z; its only landed referencer
//   CFH_Prisoner::Main 0x00D3A4A0 binds this spelling.
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

void NScript::CQ_FireHeartScript::CFH_Prisoner::GetEndCutscene(CCharString& out) const {
 out="CS_FIREHEART_KILL"+GFIntToCharString(MyCageNumber+1);
 if(MyCageNumber>0 && !IsActiveThreadTerminating()) {
  if(ParentClass->MissionHistory==1) { if(!IsActiveThreadTerminating()) out += "S"; }
  else if(ParentClass->MissionHistory==2) { if(!IsActiveThreadTerminating()) out += "D"; }
  else if(ParentClass->MissionHistory==3) { if(!IsActiveThreadTerminating()) out += "X"; }
  else IsActiveThreadTerminating();
 }
}
