// CTCCarryable::CTCCarryable(CThing* thing) -- CTCBase(thing), own vtable, not fragile, no carrier, zero attach offset and offset amount
// Leaf: ??0CTCCarryable@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCCarryable@@QAE@AAVCThing@@@Z).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3D0E, length 52 bytes. Size-optimised TU: `#pragma optimize("s", on)`.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (landed).
// Identity: vtable 0x0123B35C = CTCCarryable (rtti-vtables.tsv). Layout: Ego_r.pdb CTCCarryable (0x24 bytes):
// ForceFragility +0xc, PCarryingTC +0x10, OffsetCoordRelativeToAttachToDummy (C3DVector) +0x14, OffsetAmount +0x20.
// engine/CTCBase.h is layout-only, so the polymorphic base is modelled as CTCBase_Calls.
// Shape: size TU (pass-through push, `and dword ptr [esi+10h],0` for the null carrier); the four floats are
// zeroed with fldz/fstp (the vector through its three-float constructor); scalar stores are init-list
// members scheduled around the vtable store.
#pragma optimize("s", on)

#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
class CTCCarrying;

#include "engine/C3DVector.h"

class CTCCarryable : public CTCBase { public:               // vtable 0x0123B35C
    bool         ForceFragility;                    // +0x0c
    CTCCarrying* PCarryingTC;                       // +0x10
    C3DVector    OffsetCoordRelativeToAttachToDummy;// +0x14
    float        OffsetAmount;                      // +0x20
    CTCCarryable(CThing& thing);
};

CTCCarryable::CTCCarryable(CThing& thing)
    : CTCBase(thing)
    , ForceFragility(false)
    , PCarryingTC(0)
    , OffsetCoordRelativeToAttachToDummy(0.0f, 0.0f, 0.0f)
    , OffsetAmount(0.0f)
{
}