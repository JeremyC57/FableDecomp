namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void UpdateTick(long questId, bool enabled);
};
}

struct CGSIUpdateQuestInfoTick_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern CGSIUpdateQuestInfoTick_GuiOwner*
    CGSIUpdateQuestInfoTick_Gui;

void CGameScriptInterface::UpdateQuestInfoTick(
    long questId,
    bool enabled) const
{
    CGSIUpdateQuestInfoTick_Gui->m_QuestInfo
        ->UpdateTick(questId, enabled);
}
