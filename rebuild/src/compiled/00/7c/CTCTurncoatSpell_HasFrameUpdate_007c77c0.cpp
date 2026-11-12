// CTCTurncoatSpell::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCTurncoatSpell@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C77C0, thiscall, `ret`.
// Identity: slot 8 of CTCTurncoatSpell's vtable 0x0126E5C4 (defined here: the slot holds this body in 1 vtable(s), all of CTCTurncoatSpell or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCTurncoatSpell@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCTurncoatSpell.h (opt-ins as in the landed row (bare include)).
#define FABLE_RETAIL_CTCTURNCOATSPELL_VSLOTS
#include "engine/CTCTurncoatSpell.h"
bool CTCTurncoatSpell::HasFrameUpdate() const
{
    return true;
}
