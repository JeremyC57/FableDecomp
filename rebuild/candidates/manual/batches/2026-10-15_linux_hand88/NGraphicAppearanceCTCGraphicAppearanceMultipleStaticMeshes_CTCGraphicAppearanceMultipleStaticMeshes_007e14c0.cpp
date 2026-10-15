// NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::CTCGraphicAppearanceMultipleStaticMeshes(CThing* owner) -- thing-component and appearance-component bases, empty primitive vector and def pointer, then registered with the owner's graphic appearance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E14C0, length 80 bytes. Callees: CTCBase::CTCBase(CThing*) @ 0x00686800
// (manifest label "CEngineSubPrimitive2DClockSprite", propagated), CTCGraphicAppearance::RegisterComponent @ 0x004BD390
// (manifest "DoubleLinkedListInsertNode"; it is the Components-list insert of the appearance).
// Vtables: CAGraphicAppearanceComponent 0x0125A170 (the inlined secondary-base constructor's store survives),
// CTCGraphicAppearanceMultipleStaticMeshes 0x0126FFCC (primary) and 0x0126FFB4 (in the component base).
// Only caller: the factory @ 0x004D5E44 (operator new(0x20)).
// Layout from Ego_r.pdb (36 bytes; retail 0x20 with the 12-byte retail vector): CTCBase +0x00..+0x0b,
// CAGraphicAppearanceComponent (vptr only) +0x0c, PrimHandles (vector<CPrimitive*>) +0x10..+0x1b,
// PDef (CDefPointer<CMultiStaticMeshDef const>) +0x1c. Owner side (retail CThing, PDB offsets - 0x10):
// PTCGraphicAppearance +0x64.
// Shape: speed TU (the argument is loaded then pushed); the zero stores sit after both derived vtable stores
// because they belong to member objects with inline constructors (the vector and the def pointer); the
// register call passes the component base pointer kept from the vtable store (`lea eax,[esi+0Ch]`).
// The engine header CTCBase.h is a flattened POD (`void* __vftable`), unusable as a polymorphic base: local model.
// Leaf: ??0CTCGraphicAppearanceMultipleStaticMeshes
struct CThing;

namespace NGraphicAppearance {

struct CAGraphicAppearanceComponent;
struct CPrimitive;

}

struct CTCGraphicAppearance {
    void RegisterComponent(NGraphicAppearance::CAGraphicAppearanceComponent* component);   // 0x004BD390
};

struct CThing {
    unsigned char         _pad_0x00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64 (retail)
};

struct CTCBase {                                        // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CTCBase(CThing* owner);                             // 0x00686800
};

namespace NGraphicAppearance {

struct CAGraphicAppearanceComponent {                   // vtable 0x0125A170
    virtual void v00();
    CAGraphicAppearanceComponent() { }
};

}

struct CRetailVector_CPrimitivePtr {                    // retail std::vector: first, last, end (12 bytes)
    NGraphicAppearance::CPrimitive** m_pFirst;
    NGraphicAppearance::CPrimitive** m_pLast;
    NGraphicAppearance::CPrimitive** m_pEnd;
    CRetailVector_CPrimitivePtr() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CMultiStaticMeshDef;

struct CDefPointer_CMultiStaticMeshDef {                // one word: the def
    const CMultiStaticMeshDef* m_pDef;
    CDefPointer_CMultiStaticMeshDef() : m_pDef(0) { }
};

namespace NGraphicAppearance {

struct CTCGraphicAppearanceMultipleStaticMeshes : CTCBase, CAGraphicAppearanceComponent {
    CRetailVector_CPrimitivePtr     PrimHandles;        // +0x10
    CDefPointer_CMultiStaticMeshDef PDef;               // +0x1c
    virtual void v00();
    CTCGraphicAppearanceMultipleStaticMeshes(CThing* owner);
};

CTCGraphicAppearanceMultipleStaticMeshes::CTCGraphicAppearanceMultipleStaticMeshes(CThing* owner)
    : CTCBase(owner), CAGraphicAppearanceComponent(), PrimHandles(), PDef()
{
    ParentThing->PTCGraphicAppearance->RegisterComponent(this);
}

}
