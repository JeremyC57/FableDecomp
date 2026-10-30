struct CGSISetCutsceneSkippable_Manager
{
    unsigned char m_Pad00[0xF8];
    bool m_IsSkippable;
};

extern CGSISetCutsceneSkippable_Manager*
    CGSISetCutsceneSkippable_ManagerInstance;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetCutsceneSkippable(
    bool isSkippable) const
{
    CGSISetCutsceneSkippable_ManagerInstance->m_IsSkippable =
        isSkippable;
}
