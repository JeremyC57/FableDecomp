// CTCWallMount::CTCWallMount(CThing* owner) -- wall-mount component constructor: CTCBase(owner), bought for 0, no trophy pointer, trophy id 0
// Leaf: ??0CTCWallMount@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCWallMount@@QAE@AAVCThing@@@Z).
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DE4A0, length 64 bytes (ret 4). Vtable 0x0126FB7C = CTCWallMount
// (rtti-vtables.tsv); created by the factory @ 0x004D5BE3 (operator new(0x20)). The catalog label
// "CTCTavernBar::CTCTavernBar" is propagated over this constructor and rejected.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10.
// Vtable 0x01238C6C = CIntelligentPointer (the trophy pointer's own vtable).
// Ego_r.pdb CTCWallMount (0x20): BoughtForAmount +0x0c, Trophy (CIntelligentPointer<CThing>) +0x10, TrophyID (ulong64) +0x18.
// Shape: an initialiser list in declaration order: the immediate zero for BoughtForAmount after the vtable store, the
// trophy pointer's base constructor and vtable, then the two halves of the 64-bit id as immediates.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
class CBaseObject;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CTCWallMount : public CTCBase { public:              // vtable 0x0126FB7C
    long                        BoughtForAmount;   // +0x0c
    CIntelligentPointer<CThing> Trophy;            // +0x10
    unsigned __int64            TrophyID;          // +0x18
    CTCWallMount(CThing& owner);
};

CTCWallMount::CTCWallMount(CThing& owner)
    : CTCBase(owner)
    , BoughtForAmount(0)
    , Trophy()
    , TrophyID(0)
{
}