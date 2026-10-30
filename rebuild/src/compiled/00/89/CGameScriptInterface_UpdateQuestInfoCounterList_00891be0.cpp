namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void UpdateCounterList(
        long questId,
        long value,
        long maximum);
};
}

struct CGSIUpdateQuestInfoCounterList_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern CGSIUpdateQuestInfoCounterList_GuiOwner*
    CGSIUpdateQuestInfoCounterList_Gui;

void CGameScriptInterface::UpdateQuestInfoCounterList(
    long questId,
    long value,
    long maximum) const
{
    CGSIUpdateQuestInfoCounterList_Gui->m_QuestInfo
        ->UpdateCounterList(questId, value, maximum);
}
