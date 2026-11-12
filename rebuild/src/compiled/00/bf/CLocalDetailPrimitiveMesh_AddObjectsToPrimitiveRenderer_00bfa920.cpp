// CLocalDetailPrimitiveMesh::AddObjectsToPrimitiveRenderer() -- when the collection type has a mesh, add it to the engine's primitive renderer as a persistent static mesh and keep the returned primitive handle
// Inline-budget: /d2inlT119 -- CEnginePrimitiveHandle::operator= (Clear + the counted Data assignment) and the result temporary's ~CEnginePrimitiveHandle are expanded in retail (0x00BFA9D1..0x00BFAA6F, no call but ~CBaseClass); VC7.1's default threshold leaves both as calls (199v346), T100..T118 expand only the destructor (267v346), T119 is the smallest value that expands both (bisected 100..200).
// Leaf: ?AddObjectsToPrimitiveRenderer@CLocalDetailPrimitiveMesh@@UAEXXZ
// Flags: /d2inlT119
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFA920, length 346 bytes; thiscall, no arguments, no frame pointer, no EH frame.
// Identity: vtable 0x012A7E04 slot 5 (rebuild/manifest/rtti-vtables.tsv); Ego_r.pdb and FableWin.pdb both list
//   ?AddObjectsToPrimitiveRenderer@CLocalDetailPrimitiveMesh@@UAEXXZ.
// Callees: CEnginePrimitiveRenderer::AddStaticMesh 0x00B32F20 on g_pEnginePrimitiveRenderer (0x01436E80), result by hidden
//   pointer; the persistent primitive's virtual slot 3 NotifyReleased 0x00B8FFC0 and slot 1 Delete 0x0042D930 (through the
//   inline handle Clear / counted-pointer release); CBaseClass::~CBaseClass 0x0099A2E0 on the result temporary (its vtable
//   0x0122F598 stored first). Float pool: 0.0f @ 0x0122DEDC.
// Descriptive (made-up) name, already landed: g_pEnginePrimitiveRenderer 0x01436E80 (no PDB data symbol).
// Shape: `if (!Type->PeekMesh()) return;` then a local CEngineFadeDistance from the type's raw fade start / end (PeekFade*BeforeVideoOptionModifier), flags 1 (0x101 for
//   boolean alpha, | 0x1000 when the type's receives-shadows value is 0), and one statement
//   `PrimitiveHandle = renderer->AddStaticMesh(mesh, shadow mesh, Matrix, Scale, fade, 255, CRGBColour(255,255,255,255),
//   alpha ref, alpha mip bias, true, flags)`.
//   The type is read through its PDB inline accessors (FableWin.pdb Peek*): PeekReceivesShadows returns FLOAT, which is the
//   `movzx ; fild ; fld 0.0 ; fucompp` at 0x00BFA959, and the float-returning PeekFadeEndBeforeVideoOptionModifier / PeekAlphaMipBias give the
//   `fld/fstp` copy of the fade end and the mip bias spilled through a frame word (0x00BFA984). The colour temporary is
//   stored R,G,B,A over the B,G,R,A layout (0x00BFA9A5) with the 0xFF shared in ebx with the `long 255` alpha argument.
//   The handle assignment is Clear() (notify the primitive when this is the last handle, release, two zero stores at
//   0x00BFA9FD / 0x00BFAA00) and then the counted-pointer assignment with its own unfolded release (0x00BFAA03).
// Layout: Ego_r.pdb CLocalDetailPrimitiveMesh (116 bytes): CLocalDetailPrimitiveBase (56: CMovableResource vptr, Next +4,
//   BoundingBox +8, BoundingSphere +0x20, Type +0x30, Map +0x34), Matrix +0x38, Scale +0x68, PrimitiveHandle +0x6C;
//   CLocalDetailObjectCollectionType (84): CBaseClassNonCopyable vptr +0 (the PDB extractor's `_padding_`), Mesh +4, ShadowMesh +0xC, PlainDataBlock +0x20. Neither local-detail class has a
//   generated engine header (the landed siblings 0x00BFABA0 / 0x00BFACC0 model them in the row as well).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_DTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CENGINEFADEDISTANCE_INLINE_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER_ADDSTATICMESH
#include "fable_compiler.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CMovableResource.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineFadeDistance.h"
#include "engine/CRGBColour.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DBoundingBox.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DMeshInfo.h"
class CEngineCamera;
class CDataInputStream;
class CLocalDetailCacheMap;
enum LOCAL_DETAIL_PRIMITIVE_TYPE { LocalDetailPrimitiveUnspecified = 0 };
extern CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer;   // 0x01436E80

class CLocalDetailObjectCollectionType : public CBaseClassNonCopyable {   // Ego_r.pdb, 84 bytes; vtable 0x012A7D94 (vptr +0x00)
public:
    // FableWin.pdb inline accessors (`QBE`, inlined here). The fade pair is read through the raw
    // Peek*BeforeVideoOptionModifier accessors; PeekFadeStart 0x00BE2740 / PeekFadeEnd 0x00BE2720 are out-of-line
    // functions that also apply CEngineLocalDetailGenerator::ModifyFadeDistanceForVideoOptions 0x00B597E0 and are not used here.
    const C3DMeshInfo* PeekMesh() const { return Mesh; }
    const C3DMeshInfo* PeekShadowMesh() const { return ShadowMesh; }
    float PeekFadeStartBeforeVideoOptionModifier() const { return PlainDataBlock.FadeStart; }
    float PeekFadeEndBeforeVideoOptionModifier() const { return PlainDataBlock.FadeEnd; }
    long PeekAlphaRef() const { return PlainDataBlock.AlphaRef; }
    float PeekAlphaMipBias() const { return PlainDataBlock.AlphaMipBias; }
    bool PeekAlphaIsBoolean() const { return PlainDataBlock.AlphaIsBoolean; }
    float PeekReceivesShadows() const { return PlainDataBlock.ReceivesShadows; }   // ?PeekReceivesShadows@..@@QBEMXZ: float
    C3DMeshInfo* Mesh;                                         // +0x04
    C3DMeshInfo* ZSpriteMesh;                                  // +0x08
    C3DMeshInfo* ShadowMesh;                                   // +0x0C
    long MeshPolyCount, MeshBankIndex, ZSpriteMeshBankIndex, ShadowMeshBankIndex;   // +0x10 .. +0x1C
    struct {
        float FadeStart, FadeEnd;                              // +0x20, +0x24
        long AlphaRef;                                         // +0x28
        float AlphaMipBias;                                    // +0x2C
        float WindSkewConstantFactor, WindSkewRandomFactor, WindSkewSpeedFactor;   // +0x30 .. +0x38
        float ZSpriteFadeStart, ZSpriteFadeEnd;                // +0x3C, +0x40
        LOCAL_DETAIL_PRIMITIVE_TYPE PrimitiveType;             // +0x44
        long CacheGroup;                                       // +0x48
        bool AlphaIsBoolean, LandscapeNormalLighting, HasWindSkew, CastsShadows, ReceivesShadows;   // +0x4C .. +0x50
    } PlainDataBlock;                                          // +0x20
};
FABLE_STATIC_ASSERT(sizeof(CLocalDetailObjectCollectionType) == 84);

class CLocalDetailPrimitiveBase : public CMovableResource {   // Ego_r.pdb, 56 bytes; ten virtual slots
public:
    virtual void RenderUpdate(const CEngineCamera&, bool, unsigned long, const CLocalDetailObjectCollectionType*, float);
    virtual void AddObjectsToPrimitiveRenderer() = 0;          // slot 5
    virtual bool NeedsRenderUpdate();
    virtual void OnPostRelocation();
    virtual bool Load(CDataInputStream&) = 0;
    virtual LOCAL_DETAIL_PRIMITIVE_TYPE GetType() const = 0;
    virtual bool CanRelocateData(long) const;                  // 0x00BF9A80
    CLocalDetailPrimitiveBase* Next;                           // +0x04
    C3DBoundingBox BoundingBox;                                // +0x08
    C3DBoundingSphere BoundingSphere;                          // +0x20
    const CLocalDetailObjectCollectionType* Type;              // +0x30
    CLocalDetailCacheMap* Map;                                 // +0x34
};
FABLE_STATIC_ASSERT(sizeof(CLocalDetailPrimitiveBase) == 56);

class CLocalDetailPrimitiveMesh : public CLocalDetailPrimitiveBase {   // Ego_r.pdb, 116 bytes; vtable 0x012A7E04
public:
    virtual void AddObjectsToPrimitiveRenderer();              // 0x00BFA920
    virtual void RelocateData(long, void*, void*, long);
    virtual bool Load(CDataInputStream&);                      // 0x00BFA0F0
    virtual LOCAL_DETAIL_PRIMITIVE_TYPE GetType() const;
    CMatrix3x4 Matrix;                                         // +0x38
    float Scale;                                               // +0x68
    CEnginePrimitiveHandle PrimitiveHandle;                    // +0x6C
};
FABLE_STATIC_ASSERT(sizeof(CLocalDetailPrimitiveMesh) == 116);

void CLocalDetailPrimitiveMesh::AddObjectsToPrimitiveRenderer()
{
    if (!Type->PeekMesh())
        return;
    CEngineFadeDistance fade(Type->PeekFadeStartBeforeVideoOptionModifier(), Type->PeekFadeEndBeforeVideoOptionModifier());
    unsigned long flags = 1;
    if (Type->PeekAlphaIsBoolean())
        flags = 0x101;
    if (Type->PeekReceivesShadows() == 0.0f)
        flags |= 0x1000;
    PrimitiveHandle = g_pEnginePrimitiveRenderer->AddStaticMesh(Type->PeekMesh(), Type->PeekShadowMesh(), Matrix, Scale, fade, 255,
                                                                CRGBColour(255, 255, 255, 255), Type->PeekAlphaRef(),
                                                                Type->PeekAlphaMipBias(), true, flags);
}