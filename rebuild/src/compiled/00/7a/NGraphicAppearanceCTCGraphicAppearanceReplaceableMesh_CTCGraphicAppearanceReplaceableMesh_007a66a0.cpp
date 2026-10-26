// NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh::CTCGraphicAppearanceReplaceableMesh(CThing* owner) -- thing-component and appearance-component bases, not replaced, mesh index/primitive/def cleared, then registered with the owner's graphic appearance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A66A0, length 80 bytes. Callees: CTCBase::CTCBase(CThing*) @ 0x00686800
// (manifest label "CEngineSubPrimitive2DClockSprite", propagated), CTCGraphicAppearance::RegisterComponent @ 0x004BD390
// (manifest "DoubleLinkedListInsertNode").
// Vtables: CAGraphicAppearanceComponent 0x0125A170, CTCGraphicAppearanceReplaceableMesh 0x0126CB7C (primary) and
// 0x0126CB60 (in the component base). Only caller: the factory @ 0x004D4D71 (operator new(0x20)).
// Layout from Ego_r.pdb (32 bytes): CTCBase +0x00..+0x0b, CAGraphicAppearanceComponent (vptr only) +0x0c,
// Replaced (bool) +0x10, MeshIndex (ulong) +0x14, PrimHandle (CPrimitive*) +0x18,
// PDef (CDefPointer<CReplaceableMeshDef const>) +0x1c. Owner side (retail CThing): PTCGraphicAppearance +0x64.
// Shape: the three plain initialisers are scheduled BEFORE the derived vtable stores, the def pointer (a member
// object with an inline constructor) after them; the zeroes share one register (`xor ecx,ecx`).
// The engine header CTCBase.h is a flattened POD (`void* __vftable`), unusable as a polymorphic base: local model.
// Leaf: ??0CTCGraphicAppearanceReplaceableMesh
class CThing;

namespace NGraphicAppearance {

class CAGraphicAppearanceComponent;
class CPrimitive;

}

struct CTCGraphicAppearance {
    void RegisterComponent(NGraphicAppearance::CAGraphicAppearanceComponent* component);   // 0x004BD390
};

class CThing { public:
    unsigned char         _pad_0x00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64 (retail)
};

class CTCBase { public:                                        // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CTCBase(CThing* owner);                             // 0x00686800
};

namespace NGraphicAppearance {

class CAGraphicAppearanceComponent { public:                   // vtable 0x0125A170
    virtual void v00();
    CAGraphicAppearanceComponent() { }
};

}

class CReplaceableMeshDef;

struct CDefPointer_CReplaceableMeshDef {                // one word: the def
    const CReplaceableMeshDef* m_pDef;
    CDefPointer_CReplaceableMeshDef() : m_pDef(0) { }
};

namespace NGraphicAppearance {

class CTCGraphicAppearanceReplaceableMesh : public CTCBase, public CAGraphicAppearanceComponent { public:
    bool                            Replaced;           // +0x10
    unsigned long                   MeshIndex;          // +0x14
    CPrimitive*                     PrimHandle;         // +0x18
    CDefPointer_CReplaceableMeshDef PDef;               // +0x1c
    virtual void v00();
    CTCGraphicAppearanceReplaceableMesh(CThing* owner);
};

CTCGraphicAppearanceReplaceableMesh::CTCGraphicAppearanceReplaceableMesh(CThing* owner)
    : CTCBase(owner), CAGraphicAppearanceComponent(), Replaced(false), MeshIndex(0), PrimHandle(0), PDef()
{
    ParentThing->PTCGraphicAppearance->RegisterComponent(this);
}

}