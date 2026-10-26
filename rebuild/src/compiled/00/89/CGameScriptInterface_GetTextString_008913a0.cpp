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
extern char* g_globalObj;

CWideString CGameScriptInterface_Methods::GetTextString(CCharString const& str)
{
    NGameText::CDataBank* bank = *(NGameText::CDataBank**)(g_globalObj + 0x14);
    return bank->GetTextBySymbol(str);
}