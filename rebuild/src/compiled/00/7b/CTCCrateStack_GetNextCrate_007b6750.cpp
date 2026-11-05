// CTCCrateStack::GetNextCrate() const -- the top crate: the second slot's thing when set, else the first slot's, else null
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B6750, length 60 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (each slot tested, then re-read by a tail jump).
// Layout = Ego_r.pdb CTCCrateStack (0x20 bytes): FirstCrate +0xc, SecondCrate +0x14, CrateDefIndex +0x1c.
// Shape: a zero-initialised result local (ebx, cleared before the first call) ASSIGNED the call result in
// each arm and returned at the end: both assignments become tail jumps (`jmp GetPItem`) and only the
// empty case returns the zero register (GOTCHAS wave 24k).
class CBaseObject;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCCrateStack { public:
    void*                   __vftable;                      // +0x00
    unsigned char           _pad04[8];                      // +0x04 CTCBase
    CBaseIntelligentPointer FirstCrate;                     // +0x0c CIntelligentPointer<CThing>
    CBaseIntelligentPointer SecondCrate;                    // +0x14 CIntelligentPointer<CThing>
    long                    CrateDefIndex;                  // +0x1c
    CBaseObject* GetNextCrate() const;
};

CBaseObject* CTCCrateStack::GetNextCrate() const
{
    CBaseObject* crate = 0;
    if (SecondCrate.GetPItem())
        crate = SecondCrate.GetPItem();
    else if (FirstCrate.GetPItem())
        crate = FirstCrate.GetPItem();
    return crate;
}