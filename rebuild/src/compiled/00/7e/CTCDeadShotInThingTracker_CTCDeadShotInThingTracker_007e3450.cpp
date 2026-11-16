// CTCDeadShotInThingTracker::CTCDeadShotInThingTracker(CThing* parent) -- CTCBase(parent), own vtable, null PThingStuckIn, HasThingStuckIn = false
// Leaf: ??0CTCDeadShotInThingTracker@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCDeadShotInThingTracker@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E3450, length 56 bytes.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800; CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10;
// CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Vtable 0x0127040C = CTCDeadShotInThingTracker, 0x01237F1C = CIntelligentPointer (rtti-vtables.tsv).
// PDB CTCDeadShotInThingTracker (0x18 bytes): PThingStuckIn (CIntelligentPointer<const CThing>) +0x0c,
// HasThingStuckIn +0x14.
// Shape: speed TU (argument loaded and pushed); the derived vtable store sits between `lea edi` and the
// member's base constructor; the CIntelligentPointer(item = 0) constructor is inlined (base ctor, its
// vtable, SetPItem(0)); the bool is stored after the member constructor (declaration order).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {                            // sizeof 8: vptr + PData
    virtual void v00();
    void* PData;                                            // +0x04 CBaseObjectPointer*
    CBaseIntelligentPointer();                              // 0x00A01B10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(CBaseObject* item = 0) : CBaseIntelligentPointer() { SetPItem(item); }
};

class CTCDeadShotInThingTracker : public CTCBase { public:
    CIntelligentPointer PThingStuckIn;                      // +0x0c CIntelligentPointer<const CThing>
    bool                HasThingStuckIn;                    // +0x14
    CTCDeadShotInThingTracker(CThing& parent);
};

CTCDeadShotInThingTracker::CTCDeadShotInThingTracker(CThing& parent)
    : CTCBase(parent)
    , PThingStuckIn()
    , HasThingStuckIn(false)
{
}