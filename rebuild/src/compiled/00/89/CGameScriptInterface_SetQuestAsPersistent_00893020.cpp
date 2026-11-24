// Leaf: ?SetQuestAsPersistent@CGameScriptInterface@@UBEXABVCCharString@@_N@Z
// CGameScriptInterface::SetQuestAsPersistent
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in DAT_013b89fc (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CCharString;

#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager
{ public:
    void SetQuestAsPersistent(
        const CCharString& questName,
        bool persistent);
};


void CGameScriptInterface::SetQuestAsPersistent(const class CCharString& questName, bool persistent) const
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().SetQuestAsPersistent(questName, persistent);
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetQuestAsPersistent -> CGameScriptInterface::SetQuestAsPersistent, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
