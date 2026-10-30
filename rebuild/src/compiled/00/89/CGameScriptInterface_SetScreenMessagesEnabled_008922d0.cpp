struct CGSISetScreenMessagesEnabled_Manager
{
    unsigned char m_Pad00[0x2BC];
    bool m_AreScreenMessagesEnabled;
};

extern CGSISetScreenMessagesEnabled_Manager*
    CGSISetScreenMessagesEnabled_ManagerInstance;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetScreenMessagesEnabled(
    bool enabled) const
{
    CGSISetScreenMessagesEnabled_ManagerInstance
        ->m_AreScreenMessagesEnabled = enabled;
}
