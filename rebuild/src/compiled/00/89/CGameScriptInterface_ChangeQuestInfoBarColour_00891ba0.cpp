class CRGBColour;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    void ChangeBarColour(
        long questId,
        const CRGBColour& primaryColour,
        const CRGBColour& secondaryColour);
};
}

struct CGSIChangeQuestInfoBarColour_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern CGSIChangeQuestInfoBarColour_GuiOwner*
    CGSIChangeQuestInfoBarColour_Gui;

void CGameScriptInterface::ChangeQuestInfoBarColour(
    long questId,
    const CRGBColour& primaryColour,
    const CRGBColour& secondaryColour) const
{
    CGSIChangeQuestInfoBarColour_Gui->m_QuestInfo
        ->ChangeBarColour(
            questId,
            primaryColour,
            secondaryColour);
}
