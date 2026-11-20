// CTCFlashOnHit::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CTCFlashOnHit 0x007E3420, then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCTCFlashOnHit@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D62D1, 28 bytes, thiscall, `ret 4`; slot 0 of the CTCFlashOnHit vtable 0x0123E034 (RTTI .?AVCTCFlashOnHit@@).
// Names (agentNF182, 2026-11-20; was a free __fastcall stand-in calling ?CTCFlashOnHit_Destruct@@YIXPAX@Z): the destructor it calls is
//   ??1CTCFlashOnHit@@UAE@XZ 0x007E3420 (Ego_r.pdb + FableWin.pdb; its landed row defines that leaf), operator delete 0x00BFE9BC.
//   Hierarchy (ghidra_out/rtti_bases.tsv): CTCFlashOnHit : CTCBase.
// Shape: VC7.1 emits a class's vtable, and this ??_G with it, in a TU that constructs the class (retail: the size-optimised TU of the
//   constructor 0x004D6292 and Alloc 0x004D94AB). The header's inline constructor (engine/CTCFlashOnHit.h opt-in
//   FABLE_RETAIL_CTCFLASHONHIT_DTOR) is used by a TU-internal scaffolding function (the 0x00BB1740 / 0x00BCA060 pattern), so the vtable,
//   this ??_G and the constructor are pick-any COMDATs here; the constructor copy grades RELOCATION_MATCH against its own row 0x004D6292.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCFLASHONHIT_VSLOTS
#define FABLE_RETAIL_CTCFLASHONHIT_DTOR
#include "engine/CTCFlashOnHit.h"
namespace {
// Fidelity: scaffolding, not retail: the smallest use of the header's inline constructor, so this TU emits CTCFlashOnHit's vtable and
// with it the scalar deleting destructor (both pick-any COMDATs). Internal to this TU.
void ReferenceFlashOnHitVtable(CThing& thing)
{
    CTCFlashOnHit flash(thing);
}
}
