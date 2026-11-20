// CTCFlashOnHit::CTCFlashOnHit(CThing& thing) -- thing-component constructor: the CTCBase base, then the members zeroed
// Leaf: ??0CTCFlashOnHit@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D6292, 38 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): stores vtable 0x0123E034 (RTTI .?AVCTCFlashOnHit@@); the zeroed members match Ego_r.pdb CTCFlashOnHit; its caller is CTCFlashOnHit::Alloc 0x004D94AB.
// Callees: CTCBase::CTCBase(CThing&) 0x00686800.
// Shape (family M095, representative 0x004D43B9): `: CTCBase(thing), FadeInTime(0), FadeOutTime(0), LifeTime(0), Timer(0) {}`;
//   `optimize("s")`: the thing reference pushed straight from its slot.
// Re-port (agentNF182, 2026-11-20, with the ??_G row 0x004D62D1): onto the shared engine/CTCFlashOnHit.h (opt-in FABLE_RETAIL_CTCFLASHONHIT_DTOR
//   holds the constructor inline, retail's Alloc 0x004D94AB still calls it out of line); was a local CTCBase / CTCFlashOnHit model that defined
//   the constructor strongly and emitted a ??_G with an implicit destructor. A TU-internal scaffolding function uses the inline constructor
//   (the 0x00BB1740 pattern), so the constructor, the vtable and the ??_G (calling ~CTCFlashOnHit 0x007E3420) are pick-any COMDATs here, the
//   same copies 0x004D62D1 emits (one definition).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCFLASHONHIT_VSLOTS
#define FABLE_RETAIL_CTCFLASHONHIT_DTOR
#include "engine/CTCFlashOnHit.h"
namespace {
// Fidelity: scaffolding, not retail: the smallest use of the header's inline constructor, so this TU emits it (pick-any COMDAT) with the
// vtable and the scalar deleting destructor. Internal to this TU.
void ReferenceFlashOnHitConstructor(CThing& thing)
{
    CTCFlashOnHit flash(thing);
}
}
