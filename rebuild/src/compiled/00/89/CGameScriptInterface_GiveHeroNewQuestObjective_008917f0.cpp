class CCharString;
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).

class CQuestManager
{
public:
    void AddNewObjective(
        const CCharString& questScriptName,
        unsigned long objectiveId);
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

#include "engine/CTBaseSingletonNoDefaultAlloc.h"
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

void CGameScriptInterface::GiveHeroNewQuestObjective(
    const CCharString& questScriptName,
    unsigned long objectiveId) const
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().AddNewObjective(
        questScriptName,
        objectiveId);
}
