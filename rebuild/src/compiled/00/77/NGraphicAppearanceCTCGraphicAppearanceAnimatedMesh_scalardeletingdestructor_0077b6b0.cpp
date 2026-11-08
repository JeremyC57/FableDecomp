// NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::`scalar deleting destructor'(unsigned int flags) -- unregister from the owner's graphic appearance unless the owner is dead, then the base destructors and the optional delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077B6B0, length 80 bytes. Callees: NGraphicAppearance::CTCGraphicAppearance::
// UnregisterComponent(CAGraphicAppearanceComponent&) @ 0x004C0000, CTCBase::~CTCBase @ 0x00686830 (manifest "~CChunkedFileChunk", propagated), operator delete
// @ 0x00BFE9BC.
// Label correction: the manifest files this under CTCGraphicAppearanceStaticMesh; it is slot 0 of vtable 0x0126ACDC,
// which rtti-vtables.tsv names NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh (and the body stores 0x0126ACDC /
// 0x0126ACC0, AnimatedMesh's primary and component-base vtables).
// Layout from Ego_r.pdb (20 bytes): CTCBase +0x00..+0x0b, CAGraphicAppearanceComponent (vptr only) +0x0c,
// PrimHandle (CPrimitive*) +0x10. Owner side (retail CThing, PDB offsets - 0x10): PTCGraphicAppearance +0x64,
// the Dead bit = bit 0 of the flag byte +0x91.
// Shape: the derived destructor is `__forceinline virtual` so the `??_G` inlines its body; the component base has a
// NON-virtual inline destructor (its vtable 0x0125A170 has no destructor slot) whose vtable store lands after the
// unregister call; CTCBase's virtual destructor is out of line (the `mov ecx,esi ; call`). A `new` factory pulls the
// vtable (and so the `??_G`) into the TU.
// The engine header CTCBase.h is a flattened POD (`void* __vftable`), unusable as a polymorphic base: local model.
// Leaf: ??_GCTCGraphicAppearanceAnimatedMesh
#include "fable_compiler.h"
class CThing;

namespace NGraphicAppearance {

class CAGraphicAppearanceComponent;
class CPrimitive;

class CTCGraphicAppearance { public:
    void UnregisterComponent(CAGraphicAppearanceComponent& component);   // 0x004C0000
};

}

#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"

class CTCBase { public:                                        // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual ~CTCBase();                                 // 0x00686830
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CTCBase(CThing* owner);                             // 0x00686800
};

namespace NGraphicAppearance {

class CAGraphicAppearanceComponent { public:                   // vtable 0x0125A170 (no destructor slot)
    virtual void v00();
    CAGraphicAppearanceComponent() { }
    ~CAGraphicAppearanceComponent() { }
};

class CTCGraphicAppearanceAnimatedMesh : public CTCBase, public CAGraphicAppearanceComponent { public:
    CPrimitive* PrimHandle;                             // +0x10
    virtual void v00();
    CTCGraphicAppearanceAnimatedMesh(CThing* owner) : CTCBase(owner), PrimHandle(0) { }
    FABLE_FORCEINLINE virtual ~CTCGraphicAppearanceAnimatedMesh()
    {
        if (!ParentThing->IsDead())
            ParentThing->GetTCGraphicAppearance()->UnregisterComponent(*this);
    }
};

}

NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh* CreateAnimatedMesh(CThing* owner)
{
    return new NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh(owner);
}