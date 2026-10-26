// CTCCarrying::CTCCarrying(CThing* owner) -- thing-component base, the carry-slot vector map emptied, filters/key cleared, the primary-slot list built
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E748A, length 59 bytes. Callees: CTCBase::CTCBase(CThing*) @ 0x00686800
// (manifest label "CEngineSubPrimitive2DClockSprite", propagated), the std::list<CIntelligentPointer<CThing> > constructor
// @ 0x004B8920 (manifest "std::list_Construct").
// Label correction: the manifest calls this a CTCGraphicAppearanceReplaceableMesh constructor; the vtable it stores,
// 0x012433F4, is CTCCarrying's (rtti-vtables.tsv) and the PDB CTCCarrying fits every store once the retail
// vector is taken as 12 bytes (PDB 16). Only caller: the factory @ 0x004E7DFC.
// Layout (Ego_r.pdb CTCCarrying, retail offsets): the CTCBase part +0x00..+0x0b, CarrySlots (CVectorMap<long,
// CIntelligentPointer<CThing> >: vector first/last/end +0x0c..+0x17, empty Compare +0x18, Dirty +0x19),
// BypassFilters +0x1c, Key +0x20, ThingsInPrimarySlots (std::list, one sentinel pointer) +0x24.
// Shape: frame-pointer size TU (`optimize("y",off)` + `("s",on)`); the derived vtable store precedes the zero run
// because the zeroed words belong to the CVectorMap member's inline constructor; the list's empty allocator
// temporary is homed in the high byte of the dead `owner` slot (`lea eax,[ebp+0Bh]`).
// The engine header CTCBase.h is a flattened POD (`void* __vftable`), unusable as a polymorphic base: local model.
// Leaf: ??0CTCCarrying
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;

class CTCBase { public:                                        // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CTCBase(CThing* owner);                             // 0x00686800
};

struct SCarrySlot;                                      // pair<long, CIntelligentPointer<CThing> >

struct CRetailVector_SCarrySlot {                       // retail std::vector: first, last, end (12 bytes)
    SCarrySlot* m_pFirst;
    SCarrySlot* m_pLast;
    SCarrySlot* m_pEnd;
    CRetailVector_SCarrySlot() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CKeyPairCompareLess { };

struct CVectorMap_CarrySlots {                          // CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess<..> >
    CRetailVector_SCarrySlot Vector;                    // +0x00
    CKeyPairCompareLess      Compare;                   // +0x0c
    bool                     Dirty;                     // +0x0d
    CVectorMap_CarrySlots() : Dirty(false) { }
};

struct CRetailAllocator { CRetailAllocator() { } };

struct CRetailList_CIntelligentPointer {                // retail std::list<CIntelligentPointer<CThing> >, one sentinel pointer
    void* head;
    CRetailList_CIntelligentPointer(const CRetailAllocator& al = CRetailAllocator());   // 0x004B8920
};

class CTCCarrying : public CTCBase { public:
    CVectorMap_CarrySlots           CarrySlots;         // +0x0c
    bool                            BypassFilters;      // +0x1c
    void*                           Key;                // +0x20
    CRetailList_CIntelligentPointer ThingsInPrimarySlots;   // +0x24
    virtual void v00();
    CTCCarrying(CThing* owner);
};

CTCCarrying::CTCCarrying(CThing* owner)
    : CTCBase(owner), CarrySlots(), BypassFilters(false), Key(0), ThingsInPrimarySlots()
{
}