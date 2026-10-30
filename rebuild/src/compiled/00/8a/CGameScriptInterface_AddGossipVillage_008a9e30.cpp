class CCharString
{ public:
    char* p;
    CCharString(const CCharString& other);
    ~CCharString();
};

void __stdcall CGameScriptInterface_AddGossipVillageEx(CCharString gossipVillage, CCharString gossipText);

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::AddGossipVillage(const CCharString& gossipVillage, const CCharString& gossipText) const
{
    CGameScriptInterface_AddGossipVillageEx(gossipVillage, gossipText);
}