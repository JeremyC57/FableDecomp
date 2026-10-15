#pragma optimize("s", on)
// CTCVillageMember::CTCVillageMember(CThing*) -- CTCBase(owner), own vtable, a null def pointer, a null PVillage pointer and a zero VillageUID
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DAEE9, length 46 bytes. Size TU (`push dword ptr [esp+8]`
// pass-through, `and dword ptr [..],0` zero stores): `optimize("s",on)`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated),
// CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) @ 0x004A96F0 (called with 0). Vtable 0x0124007C.
// Shape: init-list members in declaration order after the vtable store: PDef (inline CDefPointer constructor),
// PVillage(0) through the out-of-line converting constructor, VillageUID(0) as two `and` stores.
// Layout: Ego_r.pdb CTCVillageMember (no containers, so retail = PDB): PDef +0x0C, PVillage +0x10, VillageUID +0x18,
// sizeof 0x20 (the landed factory 0x004DC620). No engine header.
struct CThing;

struct CTCBase {
    virtual void v00();
    CThing*       Thing;                    // +0x04
    unsigned char _pad_0x08[0x4];
    CTCBase(CThing* thing);                 // 0x00686800
};

struct CVillageMemberDef;

struct CDefPointerVillageMemberDef {
    const CVillageMemberDef* Object;
    CDefPointerVillageMemberDef() : Object(0) { }
};

struct CIntelligentPointer {
    void* __vftable;                        // +0x00
    void* PData;                            // +0x04
    CIntelligentPointer(CThing* thing);     // 0x004A96F0
};

struct CTCVillageMember : CTCBase {
    CDefPointerVillageMemberDef PDef;       // +0x0C
    CIntelligentPointer         PVillage;   // +0x10
    unsigned __int64            VillageUID; // +0x18
    virtual void v00();
    CTCVillageMember(CThing* thing);
};

CTCVillageMember::CTCVillageMember(CThing* thing)
    : CTCBase(thing), PDef(), PVillage(0), VillageUID(0)
{
}
