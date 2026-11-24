// CGameScriptInterface::ActivateQuest(const CCharString& name) const -- activate the named quest through the quest manager singleton,
//   as a game-script quest with its resources loaded
// Leaf: ?ActivateQuest@CGameScriptInterface@@UBEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00892E80 (vtable slot 276 of CGameScriptInterface; `this` unused, `ret 4`).
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).
// NF75 (round 7, land1117fu q10942): the quest manager call binds Ego_r.pdb ?ActivateQuest@CQuestManager@@QAE_NABVCCharString@@_N1@Z
//   (0x004B4A10, bool result; engine/CQuestManager.h FABLE_RETAIL_CQUESTMANAGER_ACTIVATEQUEST) on the shared class; was a CQuestMgr stand-in
//   reached through a reinterpret_cast (?Activate@CQuestMgr@@QAEXHHH@Z). The parameter is the quest name by reference (an int stand-in
//   before; same bytes). The function keeps its landed free __stdcall stand-in form for the virtual const member
//   ?ActivateQuest@CGameScriptInterface@@UBEXABVCCharString@@@Z (Ego_r 0x008D0010; NOTES).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEQUEST
#include "fable_string.h"
#include "engine/CQuestManager.h"
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

void CGameScriptInterface::ActivateQuest(const class CCharString& quest) const
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().ActivateQuest(quest, true, true);
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_ActivateQuest -> CGameScriptInterface::ActivateQuest, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
