// CTCCrateStack::CTCCrateStack(CThing* thing) -- CTCBase(thing), own vtable, the FirstCrate and SecondCrate intelligent pointers constructed, CrateDefIndex = 0
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Leaf: ??0CTCCrateStack@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCCrateStack@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B6610, length 66 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10 (x2). Vtables: 0x0126DA14 = CTCCrateStack, 0x01238C6C = CIntelligentPointer (rtti-vtables.tsv).
// Identity: the manifest label "CTCAvoidRegionExit::CTCAvoidRegionExit" is propagated; the vtable names the class.
// Layout = Ego_r.pdb CTCCrateStack (32 bytes): FirstCrate +0xc, SecondCrate +0x14 (CIntelligentPointer<CThing>),
// CrateDefIndex +0x1c. Shape: the vtable store precedes the member constructors (each an inline
// CIntelligentPointer() over the out-of-line base, its vtable stored through the member's address
// register), and the scalar initialiser is stored last.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

class CBaseObject;

class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CTCCrateStack : public CTCBase { public:                      // vtable 0x0126DA14
    CIntelligentPointer<CThing> FirstCrate;                         // +0x0c
    CIntelligentPointer<CThing> SecondCrate;                        // +0x14
    long                CrateDefIndex;                      // +0x1c
    CTCCrateStack(CThing& thing);
};

CTCCrateStack::CTCCrateStack(CThing& thing)
    : CTCBase(thing)
    , CrateDefIndex(0)
{
}