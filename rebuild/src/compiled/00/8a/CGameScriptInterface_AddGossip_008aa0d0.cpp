// Byte-exact reconstruction of CGameScriptInterface::AddGossip @ 0x008aa0d0
//
// Retail is a __fastcall member (this=ecx) taking a CCharString by value. It copies
// the argument into a temporary and forwards it BY VALUE to an inner routine
// (0x8ae660) which takes ownership of that copy; the by-value parameter `name` is then
// destroyed. Modeled here as a __fastcall free function with an edx dummy so the
// CCharString argument stays on the stack (=> ret 4), matching the thiscall layout.

class CCharString { public:
    char* p;
    CCharString(const CCharString& o);
    ~CCharString();
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// Inner gossip routine @ 0x8ae660: takes the string BY VALUE (owns/destroys the copy).
void __fastcall Inner(CCharString byval);

void __fastcall AddGossip(CGameScriptInterface* self, void* edx, CCharString category)
{
    Inner(category);
}