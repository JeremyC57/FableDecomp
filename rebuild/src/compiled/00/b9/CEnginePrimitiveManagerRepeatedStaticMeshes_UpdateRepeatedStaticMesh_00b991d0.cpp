// CEnginePrimitiveManagerRepeatedStaticMeshes::UpdateRepeatedStaticMesh(const CEnginePrimitiveHandle&, const NEnginePrimitiveManagerRepeatedStaticMeshes::CAddMeshDesc&) -- replace a repeated-static-mesh primitive's descriptor fields, derived fade values, and conditional mesh load
// Retail body: 0x00B991D0, 0x127 bytes. Native callers: 0x00B9AB96 (new repeated-mesh primitive) and 0x00B31FE6 (renderer forwards to manager at +0x290).
// PDB signatures in both ghidra_out/egor_pdb_names.tsv (0x00A513C0) and fablewin_pdb_names.tsv (0x02ECF4D0) identify this method and its const handle/descriptor references; donor addresses are not treated as retail addresses.
// Actual layout headers are included below. The base and repeated-mesh headers are separate ABI views of the same native object: the target writes the named base prefix through +0x28 and named repeated-mesh fields from +0x50; the eight bytes at +0x48 remain opaque. No inheritance claim is needed for these field accesses.
// CAddMeshDesc fields follow ghidra_out/struct_layouts_egor.tsv (size 0x68), including CEngineFadeDistance.FadeBeginDistance/MaxDrawDistance. CEnginePrimitiveHandle has no generated header: its actual RTTI vtable is 0x0122F598 and native callers establish its Data pointer at +4. This is a data-only ABI view; the selected function does not call its virtuals or construct it.
// 0x01436E80 is a global object pointer used elsewhere as the view record. This body reads only its byte at +4; the byte's native meaning is unresolved, so it stays an unnamed descriptive view field.
// 0x00A256F0 is the native C3DMeshInfo::BeginLoading(C3DMeshInfo*, unsigned long, bool, bool) call, confirmed by manifest signature. 0x00A23C50 is only known from the call sequence as ECX=MeshInfo, one stack argument, and a three-byte RET 4 body; the neutral call view below does not claim its owner or meaning.
#include "engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveRepeatedStaticMesh.h"
#define C3DMeshInfo C3DMeshInfoLayout
#include "engine/C3DMeshInfo.h"
#undef C3DMeshInfo

class C3DMeshInfo : public C3DMeshInfoLayout
{
public:
    void BeginLoading(unsigned long, bool, bool) const;   // 0x00A256F0
};

class C3DVector { public:
    float X;
    float Y;
    float Z;
};

class C3DBoundingBox { public:
    C3DVector MinCoord;
    C3DVector MaxCoord;
};

class C3DBoundingSphere { public:
    C3DVector Center;
    float Radius;
};

class C4DVector { public:
    float X;
    float Y;
    float Z;
    float W;
};

struct CSubsectionElement {
    float BoundingSphereCenterX[4];
    float BoundingSphereCenterY[4];
    float BoundingSphereCenterZ[4];
    float BoundingSphereRadius[4];
    unsigned char ObjectCount[4];
    unsigned char StartIndex[4];
    unsigned char SubSectionOffset[4];
    unsigned char Pad[4];
};

class CEngineFadeDistance { public:
    float FadeBeginDistance;
    float MaxDrawDistance;
};

namespace NEnginePrimitiveManagerRepeatedStaticMeshes {
struct CAddMeshDesc {
    C3DMeshInfo* MeshInfo;
    float MaxScale;
    float* VectorNormals;
    long ObjectCount;
    CSubsectionElement* SubSections;
    C3DBoundingBox BoundingBox;
    C3DBoundingSphere BoundingSphere;
    CEngineFadeDistance FadeDist;
    C4DVector* ObjectOffsets;
    C4DVector* ObjectMatricies;
    unsigned long EnginePrimitiveFlags;
    long AlphaRef;
    float AlphaMipBias;
    bool Persistent;
    unsigned long EPFFlags;
    unsigned char* WindDelayArray;
    float WindScaleFactor;
};
}

class CEnginePrimitiveHandle { public:
    void* NativeVTable; // actual RTTI vtable 0x0122F598; this body only reads Data
    CEngineInternalPrimitiveRepeatedStaticMesh* Data;
};

struct C3DMeshInfoLoadingCallView : C3DMeshInfo {
};

struct C3DMeshInfoRet4CallView {
    void UnknownRet4(unsigned long argument); // 0x00A23C50; owner/meaning unknown
};

struct SViewRecord_01436E80 {
    unsigned char UnknownPrefix[4];
    unsigned char NativeByteAt4;
};
extern SViewRecord_01436E80* g_pViewRecord_01436E80; // 0x01436E80
extern float g_fOne_0122DED8;                         // 0x0122DED8
extern float g_fReciprocal255_01231724;               // 0x01231724

struct CEnginePrimitiveManagerRepeatedStaticMeshes_Methods : CEnginePrimitiveManagerRepeatedStaticMeshes {
    void UpdateRepeatedStaticMesh(const CEnginePrimitiveHandle& handle,
        const NEnginePrimitiveManagerRepeatedStaticMeshes::CAddMeshDesc& desc);
};

void CEnginePrimitiveManagerRepeatedStaticMeshes_Methods::UpdateRepeatedStaticMesh(
    const CEnginePrimitiveHandle& handle,
    const NEnginePrimitiveManagerRepeatedStaticMeshes::CAddMeshDesc& desc)
{
    CEngineInternalPrimitiveRepeatedStaticMesh* primitive = handle.Data;
    CEngineInternalPrimitiveBase* base = reinterpret_cast<CEngineInternalPrimitiveBase*>(primitive);

    primitive->MeshInfo = desc.MeshInfo;
    primitive->VectorNormals = desc.VectorNormals;
    primitive->ObjectCount = desc.ObjectCount;
    base->EnginePrimitiveFlags = desc.EnginePrimitiveFlags;
    base->Alpha = 0xff;
    base->AlphaRef = (unsigned char)desc.AlphaRef;
    base->AlphaMipBias = desc.AlphaMipBias;
    primitive->SubSections = desc.SubSections;
    *reinterpret_cast<C3DBoundingBox*>(primitive->BoundingBox) = desc.BoundingBox;
    *reinterpret_cast<C3DBoundingSphere*>(primitive->BoundingSphere) = desc.BoundingSphere;
    primitive->MaxScale = desc.MaxScale;
    primitive->WindDelayArray = desc.WindDelayArray;
    primitive->WindScaleFactor = desc.WindScaleFactor;
    primitive->ObjectMatricies = desc.ObjectMatricies;
    primitive->ObjectOffsets = desc.ObjectOffsets;

    primitive->AlphaTransformOffset = desc.FadeDist.FadeBeginDistance /
        (desc.FadeDist.MaxDrawDistance - desc.FadeDist.FadeBeginDistance) + g_fOne_0122DED8;
    primitive->AlphaTransformMult = g_fOne_0122DED8 /
        (desc.FadeDist.FadeBeginDistance - desc.FadeDist.MaxDrawDistance);
    float alpha = (float)desc.AlphaRef * g_fReciprocal255_01231724;
    base->FadeDistance_MaxDrawDistance = (g_fOne_0122DED8 - alpha) * desc.FadeDist.MaxDrawDistance +
        alpha * desc.FadeDist.FadeBeginDistance;
    base->FadeDistance_FadeBeginDistance = desc.FadeDist.FadeBeginDistance;

    C3DMeshInfo* meshInfo = primitive->MeshInfo;
    if (!g_pViewRecord_01436E80->NativeByteAt4) {
        reinterpret_cast<C3DMeshInfoLoadingCallView*>(meshInfo)->BeginLoading(0, false, false);
        reinterpret_cast<C3DMeshInfoRet4CallView*>(meshInfo)->UnknownRet4(1);
    }
}

#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(C3DBoundingBox) == 0x18);
FABLE_STATIC_ASSERT(sizeof(C3DBoundingSphere) == 0x10);
FABLE_STATIC_ASSERT(sizeof(C4DVector) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CSubsectionElement) == 0x50);
FABLE_STATIC_ASSERT(sizeof(NEnginePrimitiveManagerRepeatedStaticMeshes::CAddMeshDesc) == 0x68);
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveHandle) == 8);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveHandle, Data) == 4);
#endif