// CTCCrateStack::GetNumCrates() const -- 2 when the second crate slot holds a crate, else 1 or 0 for the first slot
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B6660, length 38 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice).
// Layout = Ego_r.pdb CTCCrateStack (0x20 bytes): FirstCrate +0xc, SecondCrate +0x14 (CIntelligentPointer<CThing>),
// CrateDefIndex +0x1c.
// Shape: the second slot is tested first with its own `mov eax,2` return; the first slot's pointer becomes a
// `long` 0/1 through `neg ; sbb ; neg` (`!= 0`).
class CBaseObject;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCCrateStack { public:
    void*                   __vftable;                      // +0x00
    unsigned char           _pad04[8];                      // +0x04 CTCBase
    CBaseIntelligentPointer FirstCrate;                     // +0x0c CIntelligentPointer<CThing>
    CBaseIntelligentPointer SecondCrate;                    // +0x14 CIntelligentPointer<CThing>
    long                    CrateDefIndex;                  // +0x1c
    long GetNumCrates() const;
};

long CTCCrateStack::GetNumCrates() const
{
    if (SecondCrate.GetPItem())
        return 2;
    return FirstCrate.GetPItem() != 0;
}