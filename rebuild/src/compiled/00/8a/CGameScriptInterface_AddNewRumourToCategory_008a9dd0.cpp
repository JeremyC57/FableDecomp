
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

void __fastcall CGameScriptInterface_AddNewRumourToCategory(CGameScriptInterface* self, void* /*edx*/, const CCharString& rumourCategory, const CCharString& rumourText)
{
    CGameScriptInterface_AddRumourToCategoryEx(rumourCategory, rumourText);
}