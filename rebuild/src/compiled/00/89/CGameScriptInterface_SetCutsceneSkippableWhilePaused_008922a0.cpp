struct CGSISetCutsceneSkippablePaused_Manager
{
    unsigned char m_Pad00[0xF9];
    bool m_IsSkippableWhilePaused;
};

extern CGSISetCutsceneSkippablePaused_Manager*
    CGSISetCutsceneSkippablePaused_ManagerInstance;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetCutsceneSkippableWhilePaused(
    bool isSkippableWhilePaused) const
{
    CGSISetCutsceneSkippablePaused_ManagerInstance
        ->m_IsSkippableWhilePaused = isSkippableWhilePaused;
}
