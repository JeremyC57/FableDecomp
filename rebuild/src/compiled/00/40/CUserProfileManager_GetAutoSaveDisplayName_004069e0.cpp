typedef unsigned short wchar16;

class CWideString { public:
    const wchar16* p;
    CWideString(const wchar16* s);          // 1-arg ctor 0x594cd0 (else branch)
    ~CWideString();                          // dtor 0x598100
};

// The (lit,-1) temporary is a CCharString symbol name (relocation identity: ctor 0x0099EBF0, dtor 0x0099EAE0),
// handed to CDataBank::GetTextBySymbol 0x009C95E0.
class CCharString { public:
    const char* p;
    CCharString(const char* s, long n);      // 0x0099EBF0
    ~CCharString();                          // 0x0099EAE0
};

namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const; // 0x009C95E0, returns by value
};
}
using NGameText::CDataBank;

// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its text bank PTextBank +0x14 through GetTextBank(); was the stand-in SlotA* g_13b86a0.
// NF47 (round 7): 0x013B871C is ?m_pNewFrontendGameComponent@CNewFrontendGameComponent@@0PAV1@A (private static, read through the
//   inline CNewFrontendGameComponent::Get(), engine/CNewFrontendGameComponent.h), m_pTextBank +0x60 through GetTextBank(); was the stand-in SlotB* g_13b871c.
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#include "engine/CMainGameComponent.h"
#include "engine/CNewFrontendGameComponent.h"

// Literal (NF420, 2026-11-25): made-up data symbol(s) replaced by retail's pooled literal (/GF-merged ??_C@) at each use site:
//   k_122df40 (table ?k_122df40@@3QBDB -> 0x0122DF40) -> "TEXT_GUI_PROFILE_DEFAULT"
//   k_122de80 (table ?k_122de80@@3QBGB -> 0x0122DE80) -> L"Default"

class CUserProfileManager { public:
    CWideString GetAutoSaveDisplayName();
};

CWideString CUserProfileManager::GetAutoSaveDisplayName()
{
    CMainGameComponent* a = CMainGameComponent::Get();
    if (a) {
        CCharString t("TEXT_GUI_PROFILE_DEFAULT", -1);
        return a->GetTextBank().GetTextBySymbol(t);
    }
    CNewFrontendGameComponent* b = CNewFrontendGameComponent::Get();
    if (b) {
        CCharString t("TEXT_GUI_PROFILE_DEFAULT", -1);
        return b->GetTextBank().GetTextBySymbol(t);
    }
    return CWideString(L"Default");
}