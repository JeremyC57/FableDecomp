// CTCVillageMember::IsAttachedToVillage() const -- attached when the PVillage pointer resolves, or (before it is resolved) when a VillageUID is recorded
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EE240, length 38 bytes (PDB `?IsAttachedToVillage@CTCVillageMember@@QBE_NXZ`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Shape: the early `return true` is `mov al,1`; the 64-bit `VillageUID != 0` test (`mov eax,lo ; or eax,hi ; je`)
// materialises its bool through a full register (`mov eax,1` / `xor eax,eax`), each arm with its own epilogue.
// Layout: Ego_r.pdb CTCVillageMember (retail = PDB): PVillage +0x10, VillageUID +0x18. No engine header.
class CThing;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCVillageMember { public:
    void*                   __vftable;      // +0x00
    unsigned char           _pad_0x04[0xC];
    CBaseIntelligentPointer PVillage;       // +0x10
    unsigned __int64        VillageUID;     // +0x18
    bool IsAttachedToVillage() const;
};

bool CTCVillageMember::IsAttachedToVillage() const
{
    if (((CThing*)PVillage.GetPItem()))
        return true;
    return VillageUID != 0;
}