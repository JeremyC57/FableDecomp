// NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh::~CTCGraphicAppearanceReplaceableMesh() -- the replaceable-mesh appearance component's destructor: its two vtables, unless the parent thing is dead unregister the component from the parent's graphic appearance TC, release the mesh def pointer, then the CAGraphicAppearanceComponent part and ~CTCBase
// Leaf: ??1CTCGraphicAppearanceReplaceableMesh@NGraphicAppearance@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A66F0, 79 bytes, thiscall, tail `jmp ~CTCBase` 0x00686830.
// Identity (proven, two sources): it stores vtable 0x0126CB7C first and 0x0126CB60 at +0xC, both RTTI
//   .?AVCTCGraphicAppearanceReplaceableMesh@NGraphicAppearance@@ (rtti-vtables.tsv; rtti_bases: CTCBase +0,
//   NGraphicAppearance::CAGraphicAppearanceComponent +0xC); its only caller is the scalar deleting dtor 0x007A6DF0, slot 0 of
//   0x0126CB7C. Ego_r.pdb ??1CTCGraphicAppearanceReplaceableMesh@NGraphicAppearance@@UAE@XZ (0x007AB4A0, right after ??0 0x007AB450)
//   and FableWin.pdb (0x026C4140). It stores its own vtables, so the destructor is user-declared.
// Callees: ?UnregisterComponent@CTCGraphicAppearance@NGraphicAppearance@@QAEXAAVCAGraphicAppearanceComponent@2@@Z 0x004C0000 (the
//   CAGraphicAppearanceComponent subobject: `push edi` = this+0xC), the def's virtual slot 1 Delete on its last release (CDefPointer<T>
//   inline Release), ??1CTCBase@@UAE@XZ 0x00686830 (tail). The CAGraphicAppearanceComponent dtor is inline and empty (FableWin
//   ??1CAGraphicAppearanceComponent@NGraphicAppearance@@QAE@XZ, non-virtual): only its vtable store 0x0125A170 remains.
// Layout: Ego_r.pdb CTCGraphicAppearanceReplaceableMesh (0x20, no STL member, so retail's): the component base +0xC, Replaced +0x10,
//   MeshIndex +0x14, PrimHandle +0x18, PDef (CDefPointer<const CReplaceableMeshDef>) +0x1C. CThing (retail 0x98 = Ego_r less 0x10 past
//   Components): PTCGraphicAppearance +0x64, Dead bit 0 of +0x91, the rest opaque. engine/CTCBase.h (FABLE_RETAIL_CTCBASE) and
//   engine/CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE); no header exists for the appearance classes (local models).
// Shape: the landed CTCLook / CTCGraphicAppearanceStaticMesh destructor body `if (!ParentThing->IsDead())
//   ParentThing->PTCGraphicAppearance->UnregisterComponent(*this);`, then the def pointer's inline release.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                                 // CTCBase : CBaseClassNonCopyable (vtable 0x0125BE0C)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                             // CDefPointer<T> (inline Release)

namespace std { template <class _Tp> class allocator; template <class _Tp, class _Alloc> class vector; }

namespace NGraphicAppearance {
class CTCGraphicAppearance;
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
class CTCGraphicAppearance { public:
    void UnregisterComponent(CAGraphicAppearanceComponent& component);   // 0x004C0000
};
}

class CThing { public:                                      // retail offsets (Ego_r minus 0x10 past Components), sizeof 0x98
    unsigned long                               Members00_[0x64 / 4];   // +0x00 vptr .. PhysicsTC (not modelled)
    NGraphicAppearance::CTCGraphicAppearance*   PTCGraphicAppearance;   // +0x64
    unsigned long                               Members68_[(0x90 - 0x68) / 4];   // +0x68 WorldFrameCreatedAt .. PlayerNumber
    unsigned char                               Members90_;             // +0x90 PlayerNumber
    unsigned char                               Dead : 1;               // +0x91 bit 0
    unsigned char                               Flags91_ : 7;           // +0x91 CreatedByOwnerThingCreation .. CreatedBySerialisation
    unsigned char                               Members92_[6];          // +0x92 the other flag bytes
    bool IsDead() const { return Dead; }                    // FableWin ?IsDead@CThing@@QBE_NXZ (inline)
};

FABLE_STATIC_ASSERT(sizeof(CThing) == 0x98);

class CReplaceableMeshDef;

namespace NGraphicAppearance {
class CTCGraphicAppearanceReplaceableMesh : public CTCBase, public CAGraphicAppearanceComponent {   // vtables 0x0126CB7C / 0x0126CB60
public:
    CTCGraphicAppearanceReplaceableMesh(CThing& thing);     // ??0CTCGraphicAppearanceReplaceableMesh@NGraphicAppearance@@QAE@AAVCThing@@@Z
    virtual ~CTCGraphicAppearanceReplaceableMesh();
    virtual void OnCreate();
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info);
    virtual void OnAppearanceClear();
    virtual void GetMeshesUsed(std::vector<CMeshInfo, std::allocator<CMeshInfo> >& meshes) const;
    bool                                Replaced;           // +0x10
    unsigned long                       MeshIndex;          // +0x14
    CPrimitive*                         PrimHandle;         // +0x18
    CDefPointer<const CReplaceableMeshDef> PDef;            // +0x1C
};
FABLE_STATIC_ASSERT(sizeof(CTCGraphicAppearanceReplaceableMesh) == 0x20);

CTCGraphicAppearanceReplaceableMesh::~CTCGraphicAppearanceReplaceableMesh()
{
    if (!ParentThing->IsDead())
        ParentThing->PTCGraphicAppearance->UnregisterComponent(*this);
}
}