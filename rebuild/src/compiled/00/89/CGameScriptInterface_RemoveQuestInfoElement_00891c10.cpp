namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void RemoveElement(long elementId);
};
}

struct CGSIRemoveQuestInfoElement_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern CGSIRemoveQuestInfoElement_GuiOwner*
    CGSIRemoveQuestInfoElement_Gui;

void CGameScriptInterface::RemoveQuestInfoElement(
    long elementId) const
{
    CGSIRemoveQuestInfoElement_Gui->m_QuestInfo
        ->RemoveElement(elementId);
}
