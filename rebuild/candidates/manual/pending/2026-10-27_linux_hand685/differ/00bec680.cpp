// CEngineInternalPrimitiveMeshBase::CollectStats(const C3DMeshInfo& meshInfo, std::map<unsigned long, unsigned long>& stats, EEnginePrimitiveStatIndex statIndex, unsigned long) -- static: fetch the mesh of LOD 0 and record one statistic in the map: its vertex count or triangle count keyed by the mesh info, or every material texture's bank data size keyed by the texture bank index
// Grade: DIFFER(545v545) -- instruction-identical except frame-slot homing: retail homes the six operator[] key temporaries and the operator[] result spill in the dead `statIndex` argument slot (frame 0x1c), the build gives them frame words (frame 0x28; probes 1-17 in probe/00bec680.log: block-scope keys, one function-scope key, conversion temporaries, count-first call order). Next: find the form that lets VC7.1 reuse a dead by-value enum argument's slot for several successive temporaries (an `enum` parameter never read after the switch; try passing the key through a by-value helper whose parameter slot is the enum's, or a `switch` on a copied local so the argument dies before the first temporary).
// Leaf: ?CollectStats@CEngineInternalPrimitiveMeshBase@@SIXABVC3DMeshInfo@@AAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@W4EEnginePrimitiveStatIndex@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEC680, 545 bytes, static __fastcall (ecx = meshInfo, edx = stats,
//   statIndex and the unused unsigned long on the stack, `ret 8`).
// Identity: Ego_r.pdb `?CollectStats@CEngineInternalPrimitiveMeshBase@@SIXABVC3DMeshInfo@@AAV?$map@KK...@std@@W4EEnginePrimitiveStatIndex@@K@Z`
//   (the static overload; the virtual `UBEX` overload is the landed forwarder 0x00BEC8B0). Callers: that forwarder and
//   CEngineInternalPrimitiveRepeatedStaticMesh::CollectStats 0x00B98DE0 (passes its MeshInfo); both landed under the
//   invented spellings `Inner_CollectStats` / `SubMesh_CollectStats` (NAME_FIXES). The catalog's "_global::CollectStats"
//   is the bare leaf.
// Callees: C3DMeshInfo::GetMesh(CCountedPointer<const C3DMesh2>&, unsigned long lod, unsigned long flags) const
//   0x00A25E70 (lod 0, flags 4), std::map<unsigned long,unsigned long>::operator[] 0x00553E60, C3DMesh2::GetVertexCount
//   0x00A89320 / GetTriangleCount 0x00A89350 (`?GetVertexCount@C3DMesh2@@QBEKXZ`, `?GetTriangleCount@C3DMesh2@@QBEKXZ`; the
//   catalog's "GetNumberOfAbilityCasts" is propagated), CBankFile::GetEntryDataSize 0x009CC240 (on the mesh bank's
//   texture bank), CCountedPointer<const C3DMesh2>::~CCountedPointer 0x00A36EC0 (byte-identical COMDAT; the catalog's
//   "_Dest_val<...COptimisedPrimitive>" is propagated), operator delete 0x00BFE9BC.
// Layout: C3DMeshInfo Bank +4, Index +8 (retail, as the landed BeginLoading 0x00A256F0 / cloth Render 0x00B81640; the
//   generated engine/C3DMeshInfo.h carries +4 as the opaque `f4` -- header debt); retail CMeshDataBank::TextureBank +0x3d4
//   (Ego_r +0x460, the shipped build's containers are smaller); retail C3DMesh2 MaterialCount +0x88 / Materials +0x8c
//   (Ego_r +0x90/+0x94, as the landed GetVertexCount shift); C3DMeshMaterial 0x30 bytes, DiffuseMapBankIndex +0x10,
//   BumpMapBankIndex +0x14, ReflectionMapBankIndex +0x18, IlluminationMapBankIndex +0x1c (Ego_r). The enumerator names
//   are descriptive (the enum's values are not in the PDB dump): 0 vertices, 1 triangles, 2 texture bytes.
// Shape: the counted pointer's destructor is a same-TU out-of-line body that VC7.1 inlines at the early-return and the
//   shared epilogue but calls at the `default` return (the "inlined everywhere but once" shape). The key temporaries of
//   operator[] live in the dead argument slots.
#include "engine/CCountedPointer.h"
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

class CBankFile { public:
    unsigned long GetEntryDataSize(unsigned long index) const;      // 0x009CC240
};
class CGraphicDataBank : public CBankFile { };
class CMeshDataBank { public:
    unsigned char     _pad_0x00[0x3d4];
    CGraphicDataBank* TextureBank;                          // +0x3d4 (retail)
    const CGraphicDataBank* GetTextureBank() const { return TextureBank; }   // ?GetTextureBank@CMeshDataBank@@QBEPBVCGraphicDataBank@@XZ (inline)
};

struct C3DMeshMaterial {                                    // 0x30 bytes (Ego_r.pdb)
    unsigned char _pad_0x00[0x10];
    unsigned long DiffuseMapBankIndex;                      // +0x10
    unsigned long BumpMapBankIndex;                         // +0x14
    unsigned long ReflectionMapBankIndex;                   // +0x18
    unsigned long IlluminationMapBankIndex;                 // +0x1c
    unsigned char _pad_0x20[0x10];
};
class C3DMesh2 { public:
    unsigned char    _pad_0x00[0x88];
    unsigned long    MaterialCount;                         // +0x88 (retail; PDB +0x90)
    C3DMeshMaterial* Materials;                             // +0x8c (retail; PDB +0x94)
    unsigned long GetVertexCount() const;                   // 0x00A89320
    unsigned long GetTriangleCount() const;                 // 0x00A89350
};

class CCountedMeshPointer {                                 // CCountedPointer<const C3DMesh2> (engine/CCountedPointer.h layout)
public:
    const C3DMesh2* Data;                                   // +0
    CCPPointerInfo* Info;                                   // +4
    CCountedMeshPointer() : Data(0), Info(0) { }
    ~CCountedMeshPointer();                                 // 0x00A36EC0
    const C3DMesh2* operator->() const { return Data; }
    const C3DMesh2* Get() const { return Data; }
    const C3DMesh2& operator*() const { return *Data; }
};
CCountedMeshPointer::~CCountedMeshPointer()
{
    if (Info) {
        --Info->RefCount;
        if (Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Data = 0;
    Info = 0;
}

class C3DMeshInfo { public:
    void*          __vftable;                               // +0
    CMeshDataBank* Bank;                                    // +4
    long           Index;                                   // +8
    unsigned char  _pad_0x0c[0x54];
    const CGraphicDataBank* PeekTextureBank() const { return Bank->GetTextureBank(); }   // ?PeekTextureBank@C3DMeshInfo@@QBEPBVCGraphicDataBank@@XZ (inline)
    bool GetMesh(CCountedMeshPointer& mesh, unsigned long lod, unsigned long flags) const;   // 0x00A25E70
};

namespace std {
template <class _Key, class _Tp> class map { public:
    unsigned char _rep[0xc];                                // STLport 4.6 _Rb_tree (retail 12 bytes)
    _Tp& operator[](const _Key& __k);                       // 0x00553E60
};
}

enum EEnginePrimitiveStatIndex {                            // descriptive enumerator names
    EEnginePrimitiveStat_VertexCount = 0,
    EEnginePrimitiveStat_TriangleCount = 1,
    EEnginePrimitiveStat_TextureBytes = 2
};

class CEngineInternalPrimitiveMeshBase { public:
    static void __fastcall CollectStats(const C3DMeshInfo& meshInfo, std::map<unsigned long, unsigned long>& stats,
                                        EEnginePrimitiveStatIndex statIndex, unsigned long);
};

void __fastcall CEngineInternalPrimitiveMeshBase::CollectStats(const C3DMeshInfo& meshInfo,
                                                               std::map<unsigned long, unsigned long>& stats,
                                                               EEnginePrimitiveStatIndex statIndex, unsigned long)
{
    CCountedMeshPointer mesh;
    if (!meshInfo.GetMesh(mesh, 0, 4))
        return;
    switch (statIndex) {
    case EEnginePrimitiveStat_VertexCount: {
        unsigned long vertexCount = mesh->GetVertexCount();
        stats[(unsigned long)&meshInfo] = vertexCount;
        break;
    }
    case EEnginePrimitiveStat_TriangleCount: {
        unsigned long triangleCount = mesh->GetTriangleCount();
        stats[(unsigned long)&meshInfo] = triangleCount;
        break;
    }
    case EEnginePrimitiveStat_TextureBytes: {
        const C3DMesh2* pMesh = mesh.Get();
        for (unsigned long i = 0; i < pMesh->MaterialCount; ++i) {
            const C3DMeshMaterial& material = pMesh->Materials[i];
            {
                long textureIndex = material.DiffuseMapBankIndex;
                const unsigned long& key = textureIndex;
                if (textureIndex)
                    stats[key] = meshInfo.PeekTextureBank()->GetEntryDataSize(textureIndex);
            }
            {
                long textureIndex = material.BumpMapBankIndex;
                const unsigned long& key = textureIndex;
                if (textureIndex)
                    stats[key] = meshInfo.PeekTextureBank()->GetEntryDataSize(textureIndex);
            }
            {
                long textureIndex = material.ReflectionMapBankIndex;
                const unsigned long& key = textureIndex;
                if (textureIndex)
                    stats[key] = meshInfo.PeekTextureBank()->GetEntryDataSize(textureIndex);
            }
            {
                long textureIndex = material.IlluminationMapBankIndex;
                const unsigned long& key = textureIndex;
                if (textureIndex)
                    stats[key] = meshInfo.PeekTextureBank()->GetEntryDataSize(textureIndex);
            }
        }
        break;
    }
    default:
        return;
    }
}
