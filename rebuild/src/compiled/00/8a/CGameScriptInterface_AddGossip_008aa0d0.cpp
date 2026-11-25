// Byte-exact reconstruction of CGameScriptInterface::AddGossip @ 0x008aa0d0
// Leaf: ?AddGossip@CGameScriptInterface@@UAEXVCCharString@@@Z
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

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x008AE660 ?AddGossip@CTCAIScratchpad@@SIXVCCharString@@@Z (was ?Inner@@YIXVCCharString@@@Z)
class CTCAIScratchpad { public: static void __fastcall AddGossip(CCharString); };
void CGameScriptInterface::AddGossip(class CCharString category)
{
    CTCAIScratchpad::AddGossip(category);
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_AddGossip -> CGameScriptInterface::AddGossip, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
