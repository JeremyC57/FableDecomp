#pragma optimize("s", on)
// CTCVillageMember::CTCVillageMember(CThing&) -- CTCBase(owner), own vtable, a null def pointer, a null PVillage pointer and a zero VillageUID
// Leaf: ??0CTCVillageMember@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCVillageMember@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DAEE9, length 46 bytes. Size TU (`push dword ptr [esp+8]`
// pass-through, `and dword ptr [..],0` zero stores): `optimize("s",on)`.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated),
// CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) @ 0x004A96F0 (called with 0). Vtable 0x0124007C.
// Shape: init-list members in declaration order after the vtable store: PDef (inline CDefPointer constructor),
// PVillage(0) through the out-of-line converting constructor, VillageUID(0) as two `and` stores.
// Layout: Ego_r.pdb CTCVillageMember (no containers, so retail = PDB): PDef +0x0C, PVillage +0x10, VillageUID +0x18,
// sizeof 0x20 (the landed factory 0x004DC620). No engine header.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

class CVillageMemberDef;

struct CDefPointerVillageMemberDef {
    const CVillageMemberDef* Object;
    CDefPointerVillageMemberDef() : Object(0) { }
};

struct CIntelligentPointer {
    void* __vftable;                        // +0x00
    void* PData;                            // +0x04
    CIntelligentPointer(CThing* thing);     // 0x004A96F0
};

class CTCVillageMember : public CTCBase { public:
    CDefPointerVillageMemberDef PDef;       // +0x0C
    CIntelligentPointer         PVillage;   // +0x10
    unsigned __int64            VillageUID; // +0x18
    CTCVillageMember(CThing& thing);
};

CTCVillageMember::CTCVillageMember(CThing& thing)
    : CTCBase(thing), PDef(), PVillage(0), VillageUID(0)
{
}