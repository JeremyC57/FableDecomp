// Leaf: ?AddNewRumourToCategory@CGameScriptInterface@@UBEXABVCCharString@@0@Z

class CCharString {
public:
    CCharString();
    CCharString(const CCharString& other);
    ~CCharString();
    char* m_pData;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void __stdcall CGameScriptInterface_AddRumourToCategoryEx(CCharString rumourCategory, CCharString rumourText);

void CGameScriptInterface::AddNewRumourToCategory(const class CCharString& rumourCategory, const class CCharString& rumourText) const
{
    CGameScriptInterface_AddRumourToCategoryEx(rumourCategory, rumourText);
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_AddNewRumourToCategory -> CGameScriptInterface::AddNewRumourToCategory, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
