// CAIStateGroup_StealStealableItem::CAIStateGroup_StealStealableItem() -- base, own vtable, three intelligent pointers, no cleanup pending.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090151A, length 71 bytes.
// Callees: CAIStateGroupBase::CAIStateGroupBase() @ 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10 three times (PStealableItem +0x14 and PPreviouslyCarriedItem +0x24: vtable 0x01238C6C =
// CIntelligentPointer<CThing>; PEscapePoint +0x1c: vtable 0x01237F1C = CIntelligentPointer<const CThing>).
// Own vtable 0x0128CC60 is CAIStateGroup_StealStealableItem's (rtti-vtables), not RecoverStealableItem's as the
// manifest label says; the engine header for StealStealableItem has NeedsCleanup at +0x38, which is the byte the
// body clears. Pragma: `#pragma optimize("s", on)` -- the shared <CThing> vtable constant is hoisted into ebx
// across the two member constructors (size optimisation).
#pragma optimize("s", on)
class CThing;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointerThing : CBaseIntelligentPointer {        // vtable 0x01238C6C
    CIntelligentPointerThing() : CBaseIntelligentPointer() { }
};

struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    CIntelligentPointerConstThing() : CBaseIntelligentPointer() { }
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x4..+0x13
    CAIStateGroupBase();                        // 0x008FD255
};

struct CAIStateGroup_StealStealableItem : CAIStateGroupBase {   // vtable 0x0128CC60
    CIntelligentPointerThing PStealableItem;    // +0x14
    CIntelligentPointerConstThing PEscapePoint; // +0x1c
    CIntelligentPointerThing PPreviouslyCarriedItem;   // +0x24
    long ActionPointIndex;                      // +0x2c
    bool GotActionPoint;                        // +0x30
    bool ReachedDestination;                    // +0x31
    long PickUpActionID;                        // +0x34
    bool NeedsCleanup;                          // +0x38
    CAIStateGroup_StealStealableItem();
};

CAIStateGroup_StealStealableItem::CAIStateGroup_StealStealableItem()
    : CAIStateGroupBase()
    , PStealableItem()
    , PEscapePoint()
    , PPreviouslyCarriedItem()
{
    NeedsCleanup = false;
}