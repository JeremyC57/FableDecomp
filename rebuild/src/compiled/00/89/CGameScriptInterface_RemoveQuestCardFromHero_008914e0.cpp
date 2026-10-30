class CCharString;

class CQuestManager
{
public:
    void RemoveActiveQuestCard(
        const CCharString& questCardScriptName);
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern CQuestManager* CGSIRemoveQuestCardFromHero_Manager;

void CGameScriptInterface::RemoveQuestCardFromHero(
    const CCharString& questCardScriptName) const
{
    CGSIRemoveQuestCardFromHero_Manager->RemoveActiveQuestCard(
        questCardScriptName);
}
