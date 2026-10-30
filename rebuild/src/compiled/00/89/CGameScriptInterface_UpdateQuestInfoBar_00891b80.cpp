namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void UpdateBar(
        long questId,
        float value,
        float minimum,
        float maximum);
};
}

struct CGSIUpdateQuestInfoBar_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern CGSIUpdateQuestInfoBar_GuiOwner*
    CGSIUpdateQuestInfoBar_Gui;

void CGameScriptInterface::UpdateQuestInfoBar(
    long questId,
    float value,
    float minimum,
    float maximum) const
{
    CGSIUpdateQuestInfoBar_Gui->m_QuestInfo
        ->UpdateBar(questId, value, minimum, maximum);
}
