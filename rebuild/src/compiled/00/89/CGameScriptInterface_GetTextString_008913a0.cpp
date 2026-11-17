// CGameScriptInterface_Methods::GetTextString @ 0x008913a0
// Returns CWideString by value; forwards to a member helper obtained from a global.

#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public: char* p; };
class CWideString { public: unsigned short* p; };

// The game text bank at global[0x14]: NGameText::CDataBank::GetTextBySymbol 0x009C95E0 (FableWin, const, returns const CWideString).
namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;
};
}

struct CGameScriptInterface_Methods : CGameScriptInterface {
    CWideString GetTextString(CCharString const& str);
};

// global object; +0x14 = Sub*
// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its text bank PTextBank +0x14 through GetTextBank(); was the stand-in char* g_globalObj (a cast +0x14 word read).
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#include "engine/CMainGameComponent.h"

CWideString CGameScriptInterface_Methods::GetTextString(CCharString const& str)
{
    const NGameText::CDataBank* bank = &CMainGameComponent::Get()->GetTextBank();
    return bank->GetTextBySymbol(str);
}