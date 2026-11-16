// CTCInfoDisplay::CTCInfoDisplay(CThing&) -- CTCBase(owner), vtable, both direction flags false, two empty text tags, zero radius and time
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E1AC0, length 57 bytes (catalogued as the propagated label
// "CTCOwnedEntity::CTCOwnedEntity"; the vtable it stores, 0x01270044, is CTCInfoDisplay's in rtti-vtables.tsv, and its
// only caller @ 0x004D5E86 is the factory).
// Leaf: ??0CTCInfoDisplay@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model (CTCBase_007e1ac0, which bound a made-up constructor name to
//   0x00686800) is gone; the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, CCharString::CCharString() @ 0x0099E4B0 (twice).
// PDB CTCInfoDisplay (0x20 bytes): Forward +0x0c, Backward +0x0d, TextTag +0x10, TextTagBack +0x14, Radius +0x18,
// DisplayTime +0x1c.
// Shape: one zero register (ebx) for the two bytes before the string constructors and the two words after them.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

class CCharString { public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
};

struct CTCInfoDisplay : CTCBase {
    bool        Forward;                            // +0x0c
    bool        Backward;                           // +0x0d
    CCharString TextTag;                            // +0x10
    CCharString TextTagBack;                        // +0x14
    float       Radius;                             // +0x18
    float       DisplayTime;                        // +0x1c
    CTCInfoDisplay(CThing& owner);
};

CTCInfoDisplay::CTCInfoDisplay(CThing& owner)
    : CTCBase(owner), Forward(false), Backward(false), Radius(0.0f), DisplayTime(0.0f)
{
}