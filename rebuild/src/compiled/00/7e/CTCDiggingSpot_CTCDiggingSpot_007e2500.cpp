// CTCDiggingSpot::CTCDiggingSpot(CThing&) -- thing component of a digging spot: CTCBase(parent), own vtable, the "can be dug" flag (+0xc) set and the "dug" flag (+0xd) cleared
// Leaf: ??0CTCDiggingSpot@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCDiggingSpot@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E2500, length 33 bytes (+15 int3 padding).
// Callee: CTCBase::CTCBase(CThing&) @ 0x00686800 (landed in 0x007509A0). Vtable 0x0127017C = CTCDiggingSpot.
// Label correction: the manifest "CTCShotDefinitionNymphFun::CTCShotDefinitionNymphFun" is propagated; the vtable is
// CTCDiggingSpot's.
// Shape: speed TU (`mov eax,[esp+4] ; push eax`); the two byte initialisers follow the vtable store in declaration order.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

class CTCDiggingSpot : public CTCBase { public:
    bool m_canBeDug;                         // +0x0c
    bool m_dug;                              // +0x0d
    CTCDiggingSpot(CThing& parent);
};

CTCDiggingSpot::CTCDiggingSpot(CThing& parent)
    : CTCBase(parent)
    , m_canBeDug(true)
    , m_dug(false)
{
}