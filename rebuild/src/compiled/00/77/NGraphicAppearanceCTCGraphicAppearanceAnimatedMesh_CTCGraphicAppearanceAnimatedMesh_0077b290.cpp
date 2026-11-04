// NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::~CTCGraphicAppearanceAnimatedMesh() -- the appearance component's destructor: its two vtables, unless the parent thing is dead unregister the component from the parent's graphic appearance TC, then the CAGraphicAppearanceComponent part and ~CTCBase
// Leaf: ??1CTCGraphicAppearanceAnimatedMesh@NGraphicAppearance@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077B290, 55 bytes, thiscall, tail `jmp ~CTCBase' 0x00686830.
// Identity (proven, two sources): it stores vtable 0x0126ACDC first and 0x0126ACC0 at +0xC, both RTTI
//   .?AVCTCGraphicAppearanceAnimatedMesh@NGraphicAppearance@@ (rtti-vtables.tsv; rtti_bases: CTCBase +0, NGraphicAppearance::CAGraphicAppearanceComponent
//   +0xC); Ego_r.pdb / FableWin.pdb ??1CTCGraphicAppearanceAnimatedMesh@NGraphicAppearance@@UAE@XZ. No caller: the scalar deleting destructor
//   (slot 0 of 0x0126ACDC, landed) expands the same body inline, as for the landed ReplaceableMesh pair 0x007A66F0 / 0x007A6DF0.
//   The catalog label "~CTCGraphicAppearanceAnimatedMesh" is propagated across the four byte-identical bodies 0x0077B290 /
//   0x0077B9F0 / 0x0077BE80 / 0x0077C440; the vtable stores name the class.
// Callees: ?UnregisterComponent@CTCGraphicAppearance@NGraphicAppearance@@QAEXAAVCAGraphicAppearanceComponent@2@@Z 0x004C0000 (the
//   CAGraphicAppearanceComponent subobject: `push edi' = this+0xC), ??1CTCBase@@UAE@XZ 0x00686830 (tail). The
//   CAGraphicAppearanceComponent dtor is inline and empty (FableWin ??1CAGraphicAppearanceComponent@NGraphicAppearance@@QAE@XZ,
//   non-virtual): only its vtable store 0x0125A170 remains.
// Layout: Ego_r.pdb CTCGraphicAppearanceAnimatedMesh (no STL member, so retail's), the component base +0xC, then the members below.
//   engine/CThing.h (PTCGraphicAppearance +0x64, Dead bit 0 of +0x91, the inline IsDead / GetTCGraphicAppearance accessors),
//   engine/NGraphicAppearance_CTCGraphicAppearance.h (FABLE_RETAIL_CTCGRAPHICAPPEARANCE adds the UnregisterComponent declaration,
//   chunk 750 overlay) over engine/CTCBase.h (FABLE_RETAIL_CTCBASE). No header exists for the component classes (local models,
//   the CAGraphicAppearanceComponent vtable from the landed 0x007A66F0).
// Shape: the landed ReplaceableMesh / CTCLook destructor body `if (!ParentThing->IsDead())
//   ParentThing->GetTCGraphicAppearance()->UnregisterComponent(*this);'.
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"  // CTCGraphicAppearance : CTCBase (FABLE_RETAIL_CTCBASE), UnregisterComponent
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"                                  // PTCGraphicAppearance +0x64, Dead +0x91 bit 0

namespace std { template <class _Tp> class allocator; template <class _Tp, class _Alloc> class vector; }

namespace NGraphicAppearance {
class CGraphicAppearanceDrawInfo;
class CMeshInfo;
class CPrimitive;
class CAGraphicAppearanceComponent { public:                // vtable 0x0125A170 (5 slots)
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info) = 0;   // slot 0
    virtual void OnAppearanceClear() = 0;                                                         // slot 1
    virtual void GetMeshesUsed(std::vector<CMeshInfo, std::allocator<CMeshInfo> >& meshes) const;  // slot 2 0x006407B0
    virtual void GetTexturesUsed(std::vector<long, std::allocator<long> >& textures) const;        // slot 3 0x006407C0
    virtual bool IsAppearanceSentEveryRenderFrame();                                              // slot 4 0x006407D0
    ~CAGraphicAppearanceComponent() { }                     // FableWin ??1CAGraphicAppearanceComponent@NGraphicAppearance@@QAE@XZ
};

class CTCGraphicAppearanceAnimatedMesh : public CTCBase, public CAGraphicAppearanceComponent {   // vtables 0x0126ACDC / 0x0126ACC0
public:
    CTCGraphicAppearanceAnimatedMesh(CThing& thing);
    virtual ~CTCGraphicAppearanceAnimatedMesh();
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info);
    virtual void OnAppearanceClear();
    CPrimitive*                         PrimHandle;         // +0x10
};

CTCGraphicAppearanceAnimatedMesh::~CTCGraphicAppearanceAnimatedMesh()
{
    if (!ParentThing->IsDead())
        ParentThing->GetTCGraphicAppearance()->UnregisterComponent(*this);
}
}