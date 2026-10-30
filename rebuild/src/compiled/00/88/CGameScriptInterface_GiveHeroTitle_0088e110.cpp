// CGameScriptInterface::GiveHeroTitle(const CCharString&) const -- gives the hero a title object: GiveHeroObject(name, -1, true)
// Leaf: GiveHeroTitle@CGameScriptInterface
// Retail: Fable.exe @ 0x0088E110, 20 bytes, `ret 4`; calls its own vtable slot 121 (+0x1e4), GiveHeroObject(const CCharString&, long, bool)
// (ghidra_out/cgsi_vtable_slots.tsv). The vtable is modelled in slot order with placeholder virtuals.
class CCharString;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::GiveHeroTitle(const CCharString& titleKey) const
{
    GiveHeroObject(titleKey, -1, true);
}