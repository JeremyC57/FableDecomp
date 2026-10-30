class CCharString;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long AddCounterList(
        const CCharString& labelText,
        long value,
        float fadeTime);
};
}

struct CGSIAddQuestInfoCounterList_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern CGSIAddQuestInfoCounterList_GuiOwner*
    CGSIAddQuestInfoCounterList_Gui;

long CGameScriptInterface::AddQuestInfoCounterList(
    const CCharString& labelText,
    long value,
    float fadeTime) const
{
    return CGSIAddQuestInfoCounterList_Gui->m_QuestInfo
        ->AddCounterList(labelText, value, fadeTime);
}
