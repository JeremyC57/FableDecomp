// CLocalDetailPrimitiveMeshZSpriteBatch::SetupPrimitiveDesc() -- build the z-sprite batch's primitive descriptor: bounding sphere and box, the largest object scale, the type's z-sprite mesh, fade distance, z-sprite fade-in range, the batch's object entries and count, alpha reference, persistent
// Leaf: ?SetupPrimitiveDesc@CLocalDetailPrimitiveMeshZSpriteBatch@@AAE?AUCAddPrimitiveDesc@NEnginePrimitiveManagerRepeatedZSprites@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF9FA0, 325 bytes, `ret 4` (the descriptor is returned through the hidden pointer).
//   No callees. Float pool: 0.0f @ 0x0122DEDC (the scale accumulator's start).
// Names: Ego_r.pdb 00aab480 ?SetupPrimitiveDesc@CLocalDetailPrimitiveMeshZSpriteBatch@@AAE?AUCAddPrimitiveDesc@
//   NEnginePrimitiveManagerRepeatedZSprites@@XZ (private, descriptor in the namespace); the descriptor constructor is
//   Ego_d ??0CAddPrimitiveDesc@NEnginePrimitiveManagerRepeatedZSprites@@QAE@ABVC3DBoundingSphere@@ABVC3DBoundingBox@@
//   PBVC3DMeshInfo@@MVCEngineFadeDistance@@MMPAUCObjectEntry@1@JJ_N@Z (mesh BEFORE the scale, fade by value); the type
//   accessors are the Ego_r/Ego_d CLocalDetailObjectCollectionType::PeekFadeStart / PeekFadeEnd / PeekZSpriteMesh /
//   PeekZSpriteFadeStart / PeekZSpriteFadeEnd / PeekAlphaRef (all QBE). Member names from struct_layouts_egor.tsv
//   (CLocalDetailPrimitiveBase, CLocalDetailPrimitiveMeshZSpriteBatch, CLocalDetailObjectCollectionType, CEngineFadeDistance).
// Layout: local models. engine/CMovableResource.h flattens the vptr (`void* __vftable`) and the batch/base classes have no
//   engine headers; the vtable model (CMovableResource -> CLocalDetailPrimitiveBase) follows the landed sibling 0x00BFACC0.
// Shape: the fade distance is built first (its two loads precede the loop); the scale scan is
//   `maxScale = maxScale > s ? maxScale : s` (unrolled by four by VC7.1); the descriptor is built by the inline
//   constructor in the PDB parameter order. The Peek accessors are what fixed the last residue: with direct member reads
//   VC7.1 kept ZSpriteEntries in esi and the spilled fade-in fields in edi; through the inline accessors it picks
//   retail's edi (entries) / esi (scratch) colouring.
#include "fable_compiler.h"
class C3DMeshInfo;
class CEngineCamera;
class CDataInputStream;
class CLocalDetailCacheMap;
enum LOCAL_DETAIL_PRIMITIVE_TYPE { LocalDetailPrimitiveUnspecified = 0 };
class C3DBoundingBox { public:
    float MinX, MinY, MinZ, MaxX, MaxY, MaxZ;
    C3DBoundingBox(const C3DBoundingBox& b) : MinX(b.MinX), MinY(b.MinY), MinZ(b.MinZ), MaxX(b.MaxX), MaxY(b.MaxY), MaxZ(b.MaxZ) { }
};
class C3DBoundingSphere { public:
    float X, Y, Z, Radius;
    C3DBoundingSphere(const C3DBoundingSphere& s) : X(s.X), Y(s.Y), Z(s.Z), Radius(s.Radius) { }
};
class CEngineFadeDistance { public:
    float FadeBeginDistance, MaxDrawDistance;
    CEngineFadeDistance(float begin, float maxDraw) : FadeBeginDistance(begin), MaxDrawDistance(maxDraw) { }
};
class CLocalDetailObjectCollectionType { public:
    virtual ~CLocalDetailObjectCollectionType();
    C3DMeshInfo *Mesh, *ZSpriteMesh, *ShadowMesh;                                   // +0x04 .. +0x0c
    long MeshPolyCount, MeshBankIndex, ZSpriteMeshBankIndex, ShadowMeshBankIndex;   // +0x10 .. +0x1c
    // PlainDataBlock (+0x20):
    float FadeStart, FadeEnd;                                                       // +0x20, +0x24
    long AlphaRef;                                                                  // +0x28
    float AlphaMipBias, WindSkewConstantFactor, WindSkewRandomFactor, WindSkewSpeedFactor;   // +0x2c .. +0x38
    float ZSpriteFadeStart, ZSpriteFadeEnd;                                         // +0x3c, +0x40
    float PeekFadeStart() const { return FadeStart; }
    float PeekFadeEnd() const { return FadeEnd; }
    long PeekAlphaRef() const { return AlphaRef; }
    const C3DMeshInfo* PeekZSpriteMesh() const { return ZSpriteMesh; }
    float PeekZSpriteFadeStart() const { return ZSpriteFadeStart; }
    float PeekZSpriteFadeEnd() const { return ZSpriteFadeEnd; }
};
namespace NEnginePrimitiveManagerRepeatedZSprites {
struct CObjectEntry;
struct CAddPrimitiveDesc {                          // 80 bytes
    C3DBoundingSphere   BoundingSphere;             // +0x00
    C3DBoundingBox      BoundingBox;                // +0x10
    float               MaxScale;                   // +0x28
    const C3DMeshInfo*  Mesh;                       // +0x2c
    CEngineFadeDistance FadeDist;                   // +0x30
    float               FadeInStart;                // +0x38
    float               FadeInEnd;                  // +0x3c
    CObjectEntry*       ObjectEnts;                 // +0x40
    long                ObjectCount;                // +0x44
    long                AlphaRef;                   // +0x48
    bool                Persistent;                 // +0x4c
    FABLE_FORCEINLINE CAddPrimitiveDesc(const C3DBoundingSphere& sphere, const C3DBoundingBox& box, const C3DMeshInfo* mesh,
                                    float maxScale, CEngineFadeDistance fade, float fadeInStart, float fadeInEnd,
                                    CObjectEntry* entries, long count, long alphaRef, bool persistent)
        : BoundingSphere(sphere), BoundingBox(box), MaxScale(maxScale), Mesh(mesh), FadeDist(fade), FadeInStart(fadeInStart),
          FadeInEnd(fadeInEnd), ObjectEnts(entries), ObjectCount(count), AlphaRef(alphaRef), Persistent(persistent) { }
};
}
class CMovableResource { public:
    virtual ~CMovableResource() { }
    virtual void RelocateData(long, void*, void*, long) = 0;
    virtual bool CanRelocateData(long) const;
    virtual unsigned long Verify() const;
};
class CLocalDetailPrimitiveBase : public CMovableResource { public:
    virtual void RenderUpdate(const CEngineCamera&, bool, unsigned long, const CLocalDetailObjectCollectionType*, float);
    virtual void AddObjectsToPrimitiveRenderer() = 0;
    virtual bool NeedsRenderUpdate();
    virtual void OnPostRelocation();
    virtual bool Load(CDataInputStream&) = 0;
    virtual LOCAL_DETAIL_PRIMITIVE_TYPE GetType() const = 0;
    CLocalDetailPrimitiveBase*        Next;            // +0x04
    C3DBoundingBox                    BoundingBox;     // +0x08
    C3DBoundingSphere                 BoundingSphere;  // +0x20
    CLocalDetailObjectCollectionType* Type;            // +0x30
    CLocalDetailCacheMap*             Map;             // +0x34
};
class CLocalDetailPrimitiveMeshZSpriteBatch : public CLocalDetailPrimitiveBase { public:
    class CObject { public:
        float Matrix[12];                              // +0x00
        float Scale;                                   // +0x30
        float BoundingSphere[4];                       // +0x34
        void* PrimitiveHandle[2];                      // +0x44 (CEnginePrimitiveHandle, not touched here)
    };
    CObject*                                               Objects;         // +0x38
    NEnginePrimitiveManagerRepeatedZSprites::CObjectEntry* ZSpriteEntries;  // +0x3c
    long                                                   ObjectCount;     // +0x40
    virtual void AddObjectsToPrimitiveRenderer();
    virtual bool Load(CDataInputStream&);
    virtual LOCAL_DETAIL_PRIMITIVE_TYPE GetType() const;
    virtual void RelocateData(long, void*, void*, long);
private:
    NEnginePrimitiveManagerRepeatedZSprites::CAddPrimitiveDesc SetupPrimitiveDesc();
};

NEnginePrimitiveManagerRepeatedZSprites::CAddPrimitiveDesc CLocalDetailPrimitiveMeshZSpriteBatch::SetupPrimitiveDesc()
{
    CEngineFadeDistance fade(Type->PeekFadeStart(), Type->PeekFadeEnd());
    float maxScale = 0.0f;
    for (long i = 0; i < ObjectCount; ++i)
        maxScale = maxScale > Objects[i].Scale ? maxScale : Objects[i].Scale;
    return NEnginePrimitiveManagerRepeatedZSprites::CAddPrimitiveDesc(BoundingSphere, BoundingBox, Type->PeekZSpriteMesh(), maxScale,
        fade, Type->PeekZSpriteFadeStart(), Type->PeekZSpriteFadeEnd(), ZSpriteEntries, ObjectCount, Type->PeekAlphaRef(), true);
}