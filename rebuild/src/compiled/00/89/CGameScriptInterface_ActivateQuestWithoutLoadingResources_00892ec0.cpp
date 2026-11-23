// Naming wave 18: the global stand-in g_questMgr (0x013B89FC) is the private static CCountedPointer<CQuestManager>
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Leaf: ?ActivateQuestWithoutLoadingResources@CGameScriptInterface@@UBEXABVCCharString@@@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT with the PDB parameter / return types (was ?ActivateQuestWithoutLoadingResources@CGameScriptInterface_Methods@@QAEXPAVCCharString@@@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
#include "engine/CCountedPointer.h"
class CCharString;

template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager { public:
  // NF75 (round 7, land1117fu q10942): Ego_r.pdb ?ActivateQuest@CQuestManager@@QAE_NABVCCharString@@_N1@Z (0x004B4A10; bool result, name by
  //   reference; engine/CQuestManager.h ACTIVATEQUEST) in the row's pre-existing local declaration (was the made-up Activate(CCharString*, int, int)).
  bool ActivateQuest(const CCharString& quest_name, bool game_script, bool load_resources);   // 0x004B4A10
};
void CGameScriptInterface::ActivateQuestWithoutLoadingResources(const CCharString& cs) const {
  CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().ActivateQuest(cs, true, false);
}
