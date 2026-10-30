class CCharString;

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

extern CQuestManager* CGSIGiveHeroNewQuestObjective_Manager;

void CGameScriptInterface::GiveHeroNewQuestObjective(
    const CCharString& questScriptName,
    unsigned long objectiveId) const
{
    CGSIGiveHeroNewQuestObjective_Manager->AddNewObjective(
        questScriptName,
        objectiveId);
}
