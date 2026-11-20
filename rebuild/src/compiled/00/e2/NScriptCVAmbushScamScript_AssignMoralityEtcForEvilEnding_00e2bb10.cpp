// NScript::CV_AmbushScamScript::AssignMoralityEtcForEvilEnding() -- give the hero the ambush-scam evil morality, renown and experience from the script def
// Leaf: ?AssignMoralityEtcForEvilEnding@CV_AmbushScamScript@NScript@@QAEXXZ
// Retail: Fable.exe @ 0x00E2BB10, 77 bytes (+ int3 padding). Called once from the quest's ending code.
// Callees (virtual, CGameScriptInterface through Game at +0x40): GiveHeroMorality(float) slot 156 (+0x270),
// GiveHeroRenownPoints slot 161 (+0x284), GiveHeroExperience slot 128 (+0x200).
// Data: the script def @ 0x0143E90C. Its retail AmbushScam block sits 4 bytes below Ego_r.pdb's CScriptDef offsets:
// Experience +0x2a8, Renown +0x2ac, MoralityEvil +0x2b0, MoralityGood +0x2b4 -- this body reads +0x2b0 (MoralityEvil).
// The evil-ending twin of 0x00E2BAC0 (which reads MoralityGood +0x2b4). Identity: Ego_r.pdb (donor 0x00E778E0) + FableWin.pdb
//   (0x03923730) ?AssignMoralityEtcForEvilEnding@CV_AmbushScamScript@NScript@@QAEXXZ; the landed Main 0x00E2B090 binds 0x00E2BB10 so.
// Shape: each argument is read through the def global and pushed as a dword (a float lvalue argument is copied with
// integer moves); the interface pointer is re-read from +0x40 for each call.
// Naming (NF162, 2026-11-20, q4751 / q4752): was a row-local global `struct CV_AmbushScamScript` (leaf
//   ?AssignMoralityEtcForEvilEnding@CV_AmbushScamScript@@QAEXXZ, shared by 0x00E2BAC0 and 0x00E2BB10); now on the shared
//   engine/CV_AmbushScamScript.h (NScript, CLevelScriptBase<CQ_SunnyvaleMasterData>: Game +0x40 is the CGameScriptInterfaceBase*).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#include "stlport/fable_containers.h"
#include "engine/CV_AmbushScamScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>)

void NScript::CV_AmbushScamScript::AssignMoralityEtcForEvilEnding()
{
    Game->GiveHeroMorality(pGlobalScriptDef->AmbushScamMoralityEvil);
    Game->GiveHeroRenownPoints(pGlobalScriptDef->AmbushScamRenown);
    Game->GiveHeroExperience(pGlobalScriptDef->AmbushScamExperience);
}
