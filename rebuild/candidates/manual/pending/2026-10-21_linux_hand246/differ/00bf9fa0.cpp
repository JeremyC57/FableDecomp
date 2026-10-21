// Grade: DIFFER(325v325) -- register swap only: ZSpriteEntries in esi and the spilled type fields through edi (retail edi/esi), with the matching store reorder at the end; tried a named fade-in local (worse)
// CLocalDetailPrimitiveMeshZSpriteBatch::SetupPrimitiveDesc() -- build the z-sprite batch's primitive descriptor: bounding sphere and box, the largest object scale, the type's z-sprite mesh, fade distance, fade-in range and alpha reference, the batch's object entries and count, persistent
// Leaf: SetupPrimitiveDesc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF9FA0, length 323 bytes, `ret 4` (CAddPrimitiveDesc returned through the
//   hidden pointer). No callees. Float pool: 0.0f @ 0x0122DEDC (the scale accumulator's start).
// Layout: as the landed sibling CLocalDetailPrimitiveRepeatedMesh::SetupPrimitiveDesc 0x00BF9BC0 (the local-detail
//   primitive base {vptr, Next +4, BoundingBox +8, BoundingSphere +0x20, Type +0x30, Map +0x34} and the collection type
//   record); Ego_r.pdb CLocalDetailPrimitiveMeshZSpriteBatch: Objects +0x38 (0x4C-byte records, scale at +0x30),
//   ZSpriteEntries +0x3C, ObjectCount +0x40; CAddPrimitiveDesc (80 bytes) per Ego_r.pdb. No engine headers.
// Shape: the fade distance is built first (its two loads precede the loop); the scale scan is
//   `maxScale = maxScale > s ? maxScale : s` (unrolled by four by VC7.1); the descriptor is built by an inline
//   memberwise constructor.
struct C3DMeshInfo;
struct CObjectEntry;
struct C3DBoundingBox {
    float MinX, MinY, MinZ, MaxX, MaxY, MaxZ;
    C3DBoundingBox(const C3DBoundingBox& b) : MinX(b.MinX), MinY(b.MinY), MinZ(b.MinZ), MaxX(b.MaxX), MaxY(b.MaxY), MaxZ(b.MaxZ) { }
};
struct C3DBoundingSphere {
    float X, Y, Z, Radius;
    C3DBoundingSphere(const C3DBoundingSphere& s) : X(s.X), Y(s.Y), Z(s.Z), Radius(s.Radius) { }
};
struct CEngineFadeDistance {
    float FadeStart, FadeEnd;
    CEngineFadeDistance(float s, float e) : FadeStart(s), FadeEnd(e) { }
};
struct CollectionType {
    void* vtable;
    C3DMeshInfo *Mesh, *ZSpriteMesh, *ShadowMesh;              // +0x04 .. +0x0c
    long MeshPolyCount, MeshBankIndex, ZSpriteMeshBankIndex, ShadowMeshBankIndex;   // +0x10 .. +0x1c
    float FadeStart, FadeEnd;                                   // +0x20, +0x24
    long AlphaRef;                                              // +0x28
    float AlphaMipBias, WindSkewConstantFactor, WindSkewRandomFactor, WindSkewSpeedFactor;   // +0x2c .. +0x38
    float ZSpriteFadeStart, ZSpriteFadeEnd;                     // +0x3c, +0x40
};
struct CAddPrimitiveDesc {                         // Ego_r.pdb, 80 bytes
    C3DBoundingSphere   BoundingSphere;            // +0x00
    C3DBoundingBox      BoundingBox;               // +0x10
    float               MaxScale;                  // +0x28
    C3DMeshInfo*        Mesh;                      // +0x2c
    CEngineFadeDistance FadeDist;                  // +0x30
    float               FadeInStart;               // +0x38
    float               FadeInEnd;                 // +0x3c
    CObjectEntry*       ObjectEnts;                // +0x40
    long                ObjectCount;               // +0x44
    long                AlphaRef;                  // +0x48
    bool                Persistent;                // +0x4c
    __forceinline CAddPrimitiveDesc(const C3DBoundingSphere& sphere, const C3DBoundingBox& box, float maxScale, C3DMeshInfo* mesh,
                                    const CEngineFadeDistance& fade, float fadeInStart, float fadeInEnd, CObjectEntry* entries,
                                    long count, long alphaRef, bool persistent)
        : BoundingSphere(sphere), BoundingBox(box), MaxScale(maxScale), Mesh(mesh), FadeDist(fade), FadeInStart(fadeInStart),
          FadeInEnd(fadeInEnd), ObjectEnts(entries), ObjectCount(count), AlphaRef(alphaRef), Persistent(persistent) { }
};
struct CZSpriteObject {                            // 0x4c bytes
    unsigned char _pad[0x30];
    float         Scale;                           // +0x30
    unsigned char _pad34[0x4c - 0x34];
};
struct ZSpriteBatchView {
    void*             vtable;                      // +0x00
    void*             Next;                        // +0x04
    C3DBoundingBox    BoundingBox;                 // +0x08
    C3DBoundingSphere BoundingSphere;              // +0x20
    CollectionType*   Type;                        // +0x30
    void*             Map;                         // +0x34
    CZSpriteObject*   Objects;                     // +0x38
    CObjectEntry*     ZSpriteEntries;              // +0x3c
    long              ObjectCount;                 // +0x40
    CAddPrimitiveDesc SetupPrimitiveDesc();
};

CAddPrimitiveDesc ZSpriteBatchView::SetupPrimitiveDesc()
{
    CEngineFadeDistance fade(Type->FadeStart, Type->FadeEnd);
    float maxScale = 0.0f;
    for (long i = 0; i < ObjectCount; ++i)
        maxScale = maxScale > Objects[i].Scale ? maxScale : Objects[i].Scale;
    return CAddPrimitiveDesc(BoundingSphere, BoundingBox, maxScale, Type->ZSpriteMesh, fade, Type->ZSpriteFadeStart,
                             Type->ZSpriteFadeEnd, ZSpriteEntries, ObjectCount, Type->AlphaRef, true);
}
