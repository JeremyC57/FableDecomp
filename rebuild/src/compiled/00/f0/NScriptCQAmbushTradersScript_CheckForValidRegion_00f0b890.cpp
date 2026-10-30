// NScript::CQ_AmbushTradersScript::CheckForValidRegion() -- true when the hero is in one of the three regions the ambush can run in (BowerstoneJail, DesertedFarm or GibbetWoods is loaded)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0B890, length 197 bytes, plain `ret`, bool in al.
// Identity: catalog CQ_AmbushTradersScript::CheckForValidRegion, next to the landed PhaseOne 0x00F0B780 of the class.
// Callees: CGameScriptInterface slot 12 IsRegionLoaded(const CCharString&) (+0x30) through GSI (+0x40);
// CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0. Strings: "BowerstoneJail" @ 0x012C7788,
// "DesertedFarm" @ 0x012C7778, "GibbetWoods" @ 0x012C776C.
// Shape: one `||` chain over three implicit-conversion temporaries: VC builds them in three frame words and tracks
// which ones exist with a constant-folded destruction mask in ebx (`mov ebx,1/3/7`, cleared bit by bit as each
// destructor runs, last-built first); the chain's result is parked in a frame byte across the destructors.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

class CQ_AmbushTradersScript : public CScriptBase { public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    bool CheckForValidRegion();
};

bool CQ_AmbushTradersScript::CheckForValidRegion()
{
    return GSI->IsRegionLoaded("BowerstoneJail") || GSI->IsRegionLoaded("DesertedFarm")
        || GSI->IsRegionLoaded("GibbetWoods");
}
}