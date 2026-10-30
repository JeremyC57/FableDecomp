class CCharStringData;

class CCharString
{
public:
    CCharStringData* m_pData;

    CCharString(const CCharString& other);
    ~CCharString();
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void __stdcall AddGossipFactionToCategory_Impl(
    CCharString category,
    CCharString faction);

void CGameScriptInterface::AddGossipFactionToCategory(
    const CCharString& category,
    const CCharString& faction) const
{
    AddGossipFactionToCategory_Impl(category, faction);
}