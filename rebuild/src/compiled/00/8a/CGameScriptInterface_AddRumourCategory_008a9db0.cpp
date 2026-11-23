// Leaf: ?AddRumourCategory@CGameScriptInterface@@UBEXABVCCharString@@@Z
#define FABLE_RETAIL_NSCRIPT   // alias route: engine/CGameScriptInterface.h declares AddRumourCategory under this class-model switch
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public:
    unsigned int value;
    CCharString(const CCharString& other);
};

extern void __stdcall CGameScriptInterface_FindOrCreateRumourCategory(CCharString category);


void CGameScriptInterface::AddRumourCategory(const CCharString& category) const
{
    CGameScriptInterface_FindOrCreateRumourCategory(category);
}