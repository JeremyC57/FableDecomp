struct CGameScriptInterfaceOwnerOverlay
{
    unsigned char m_Pad00[0x4];
    void* m_pOwner; // 0x4
};

struct CGameScriptInterfaceGuildMasterMessagesOwnerOverlay
{
    unsigned char m_Pad00[0xE0];
    unsigned char m_bGuildMasterMessages; // 0xE0
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetGuildMasterMessages(bool enabled) const
{
    const CGameScriptInterfaceOwnerOverlay* ownerHolder = reinterpret_cast<const CGameScriptInterfaceOwnerOverlay*>(this);
    CGameScriptInterfaceGuildMasterMessagesOwnerOverlay* owner = reinterpret_cast<CGameScriptInterfaceGuildMasterMessagesOwnerOverlay*>(ownerHolder->m_pOwner);
    owner->m_bGuildMasterMessages = enabled;
}