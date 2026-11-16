// CTCTrespassable::CTCTrespassable(CThing* parent) -- CTCBase(parent), Trespassable = IsBeingTrespassedIn = false, own vtable
// Leaf: ??0CTCTrespassable@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCTrespassable@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BAD20, length 33 bytes.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800.
// Vtable 0x0126DD9C = CTCTrespassable (rtti-vtables.tsv): the chunk label "CTCEditor" is propagated.
// PDB CTCTrespassable (0x10 bytes): Trespassable +0x0c, IsBeingTrespassedIn +0x0d.
// Shape: speed TU; `xor al,al` and both byte initialisers are scheduled before the derived vtable store.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

class CTCTrespassable : public CTCBase { public:
    bool Trespassable;                                      // +0x0c
    bool IsBeingTrespassedIn;                               // +0x0d
    CTCTrespassable(CThing& parent);
};

CTCTrespassable::CTCTrespassable(CThing& parent)
    : CTCBase(parent)
    , Trespassable(false)
    , IsBeingTrespassedIn(false)
{
}