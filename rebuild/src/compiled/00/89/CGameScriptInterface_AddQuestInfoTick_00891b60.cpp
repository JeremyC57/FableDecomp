class CCharString;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long AddTick(
        const CCharString& labelText,
        bool completed,
        float fadeTime);
};
}

struct CGSIAddQuestInfoTickText_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern CGSIAddQuestInfoTickText_GuiOwner*
    CGSIAddQuestInfoTickText_Gui;

long CGameScriptInterface::AddQuestInfoTick(
    const CCharString& labelText,
    bool completed,
    float fadeTime) const
{
    return CGSIAddQuestInfoTickText_Gui->m_QuestInfo
        ->AddTick(labelText, completed, fadeTime);
}
