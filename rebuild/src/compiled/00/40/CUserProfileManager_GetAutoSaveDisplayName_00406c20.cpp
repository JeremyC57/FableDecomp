class CWideString
// Leaf: ?GetAutoSaveDisplayName@CUserProfileManager@@QBE?AVCWideString@@XZ
{ public:
    void* p;
    CWideString(const unsigned short* s);          // ctor helper 0x594a90 (this, str)        [factory ret]
    CWideString(const CWideString& o);             // user copy ctor -> non-POD, memory return
    ~CWideString();                                // dtor helper 0x597ec0 (this)
};

// The (lit,-1) temporary is a CCharString symbol name (relocation identity: ctor 0x0099EBF0, dtor 0x0099EAE0),
// handed to CDataBank::GetTextBySymbol 0x009C95E0.
class CCharString
{ public:
    void* p;
    CCharString(const char* s, long n);            // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};

namespace NGameText {
class CDataBank
{ public:
    // struct-returning member: helper 0x5c29c0, __fastcall(this, hidden_ret, CWideString* fmt)
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
using NGameText::CDataBank;

// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its text bank PTextBank +0x14 through GetTextBank(); was the stand-in NodeA* g_A.
// NF47 (round 7): 0x013B871C is ?m_pNewFrontendGameComponent@CNewFrontendGameComponent@@0PAV1@A (private static, read through the
//   inline CNewFrontendGameComponent::Get(), engine/CNewFrontendGameComponent.h), m_pTextBank +0x60 through GetTextBank(); was the stand-in NodeB* g_B.
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#include "engine/CMainGameComponent.h"
#include "engine/CNewFrontendGameComponent.h"


class CUserProfileManager
{ public:
    CWideString GetAutoSaveDisplayName() const;    // __fastcall member, returns CWideString by value
};

CWideString CUserProfileManager::GetAutoSaveDisplayName() const
{
    CMainGameComponent* a = CMainGameComponent::Get();
    if (a) {
        CCharString fmt("TEXT_GUI_SAVE_AUTO_SAVE_NAME", -1);
        return a->GetTextBank().GetTextBySymbol(fmt);
    }
    CNewFrontendGameComponent* b = CNewFrontendGameComponent::Get();
    if (b) {
        CCharString fmt("TEXT_GUI_SAVE_AUTO_SAVE_NAME", -1);
        return b->GetTextBank().GetTextBySymbol(fmt);
    }
    return CWideString((const unsigned short*)L"AutoSave");
}