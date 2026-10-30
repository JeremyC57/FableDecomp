class CTimer;
class CCharString;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long AddCounter_0064f93f(
        const CTimer& timer,
        const CCharString& labelText,
        float fadeTime);
};
}

struct CGSIAddQuestInfoTimer_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern CGSIAddQuestInfoTimer_GuiOwner*
    CGSIAddQuestInfoTimer_Gui;

long CGameScriptInterface::AddQuestInfoTimer(
    const CTimer& timer,
    const CCharString& labelText,
    float fadeTime) const
{
    return CGSIAddQuestInfoTimer_Gui->m_QuestInfo
        ->AddCounter_0064f93f(timer, labelText, fadeTime);
}
