enum EGameAction
{
    EGameAction_None = 0
};

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long AddTick(
        EGameAction action,
        bool completed,
        float fadeTime);
};
}

struct CGSIAddQuestInfoTickAction_GuiOwner
{
    unsigned char m_Unused[0x1C8];
    NPlayerGui::CDrawQuestInfo* m_QuestInfo;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern CGSIAddQuestInfoTickAction_GuiOwner*
    CGSIAddQuestInfoTickAction_Gui;

long CGameScriptInterface::AddQuestInfoTick(
    EGameAction action,
    bool completed,
    float fadeTime) const
{
    return CGSIAddQuestInfoTickAction_Gui->m_QuestInfo
        ->AddTick(action, completed, fadeTime);
}
