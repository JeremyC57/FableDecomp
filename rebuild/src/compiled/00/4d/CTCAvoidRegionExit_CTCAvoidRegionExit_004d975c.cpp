// CTCAvoidRegionExit::CTCAvoidRegionExit(CThing* thing) -- CTCBase(thing), own vtable, the EntranceMarker and ExitMarker intelligent pointers constructed, NextFrameCheck = 0
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Leaf: ??0CTCAvoidRegionExit@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCAvoidRegionExit@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D975C, length 61 bytes; a size-optimised TU
// (`push dword ptr [esp+10h]` pass-through, `and dword ptr [esi+1Ch],0`, the CIntelligentPointer vtable
// held in ebx for both member stores): `#pragma optimize("s", on)`.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10 (x2). Vtables: 0x0123EFAC = CTCAvoidRegionExit, 0x01238C6C = CIntelligentPointer (rtti-vtables.tsv).
// Layout = Ego_r.pdb CTCAvoidRegionExit (32 bytes): EntranceMarker +0xc, ExitMarker +0x14
// (CIntelligentPointer<CThing>), NextFrameCheck +0x1c. Shape: the vtable store precedes the member
// constructors (inline CIntelligentPointer() over the out-of-line base, vtable stored through the
// member's address register); the scalar initialiser is stored last.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

class CBaseObject;

class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CTCAvoidRegionExit : public CTCBase { public:                 // vtable 0x0123EFAC
    CIntelligentPointer<CThing> EntranceMarker;                     // +0x0c
    CIntelligentPointer<CThing> ExitMarker;                         // +0x14
    unsigned long       NextFrameCheck;                     // +0x1c
    CTCAvoidRegionExit(CThing& thing);
};

CTCAvoidRegionExit::CTCAvoidRegionExit(CThing& thing)
    : CTCBase(thing)
    , NextFrameCheck(0)
{
}