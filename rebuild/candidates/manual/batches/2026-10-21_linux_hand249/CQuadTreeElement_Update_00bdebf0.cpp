// CQuadTreeElement::Update(CEngineStaticMapFileBlockManager*, const CObjectTypeCollectionPalette&, double) -- stream this local-detail quadtree node in/out by camera distance, recurse into loaded children and its cache groups, and gather their render-update flags
// Leaf: ?Update@CQuadTreeElement
// Retail 0x00BDEBF0, 355 bytes, ret 0x10 (ebp frame, `and esp,-8`: the double deadline). Callees:
// CEngineLocalDetailGenerator::ModifyFadeDistanceForVideoOptions 0x00B597E0 (generator global 0x01436E88),
// GFGetFastSubFrameTimer 0x009E1C70 (__fastcall double), LoadStaticContents 0x00BDE390, UnloadStaticContents 0x00BDE020,
// itself (children), CObjectCacheGroupCollection::StaticUpdate 0x00BDEB50 (landed twin). Camera global 0x01436EA0
// (world position +0x54).
// Layout = Ego_r.pdb CQuadTreeElement (0x4C): ObjectCacheGroupCollections +4, Children[4] +8, BoundingSphere +0x18,
// MaxFadeDistance +0x2C, PrimitiveTypeMask +0x30, the bitfield byte +0x48 (HasDynamicAreasFlag, RenderUpdateNeeded,
// StaticContentsLoaded, Loading); collections chain through Next +4 with their RenderUpdateNeeded bit at +0x36.
// Shape: the twin of the landed StaticUpdate 0x00BDEB50 for the distance test, except the sphere centre is copied
// into a C3DVector built from its three floats (X/Y stay on the x87 stack, Z in its frame home) before the inline
// `(centre - camera).GetLength()`; `RenderUpdateNeeded = false;` precedes `bool changed = false;` (the reverse order
// puts the flag byte in cl instead of dl); the flag merges are bitfield `|=` (VC's or/xor/and/xor insert).
#include <math.h>
#pragma intrinsic(sqrt)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
struct C3DBoundingSphere {
    C3DVector Centre;
    float     Radius;
};
struct CEngineCamera {
    unsigned char _pad00[0x54];
    C3DVector     Position;                                 // +0x54
};
struct CEngineLocalDetailGenerator {
    float ModifyFadeDistanceForVideoOptions(unsigned long typeMask, float distance);   // 0x00B597E0
};
extern CEngineCamera* g_pEngineCamera;                      // 0x01436EA0
extern CEngineLocalDetailGenerator* g_pLocalDetailGenerator; // 0x01436E88
double __fastcall GFGetFastSubFrameTimer();                 // 0x009E1C70
struct CEngineStaticMapFileBlockManager;
struct CObjectTypeCollectionPalette;
struct CLocalDetailCacheMap;
struct CObjectCacheGroupCollection {
    virtual void v00();
    CObjectCacheGroupCollection* Next;                      // +0x04
    unsigned char                _pad08[0x36 - 0x08];
    bool                         RenderUpdateNeeded : 1;    // +0x36 bit 0
    bool                         Loading : 1;
    bool StaticUpdate(CEngineStaticMapFileBlockManager* blocks, const CObjectTypeCollectionPalette& palette, double deadline);   // 0x00BDEB50
};
struct CQuadTreeElement {
    virtual void v00();
    CObjectCacheGroupCollection* ObjectCacheGroupCollections;   // +0x04
    CQuadTreeElement*            Children[4];                   // +0x08
    C3DBoundingSphere            BoundingSphere;                // +0x18
    CLocalDetailCacheMap*        Map;                           // +0x28
    float                        MaxFadeDistance;               // +0x2c
    unsigned long                PrimitiveTypeMask;             // +0x30
    long                         FileBlockPos;                  // +0x34
    long                         FileBlockSize;                 // +0x38
    long                         OffsetIntoFileBlock;           // +0x3c
    unsigned short               MapX, MapY, Width, Height;     // +0x40
    bool                         HasDynamicAreasFlag : 1;       // +0x48 bit 0
    bool                         RenderUpdateNeeded : 1;        // bit 1
    bool                         StaticContentsLoaded : 1;      // bit 2
    bool                         Loading : 1;                   // bit 3
    bool LoadStaticContents(CEngineStaticMapFileBlockManager* blocks, const CObjectTypeCollectionPalette& palette);   // 0x00BDE390
    void UnloadStaticContents(CEngineStaticMapFileBlockManager* blocks);                                            // 0x00BDE020
    bool Update(CEngineStaticMapFileBlockManager* blocks, const CObjectTypeCollectionPalette& palette, double deadline);
};

bool CQuadTreeElement::Update(CEngineStaticMapFileBlockManager* blocks, const CObjectTypeCollectionPalette& palette, double deadline)
{
    RenderUpdateNeeded = false;
    bool changed = false;
    if (!HasDynamicAreasFlag) {
        C3DVector centre(BoundingSphere.Centre.X, BoundingSphere.Centre.Y, BoundingSphere.Centre.Z);
        float distance = (centre - g_pEngineCamera->Position).GetLength() - BoundingSphere.Radius;
        if (g_pLocalDetailGenerator->ModifyFadeDistanceForVideoOptions(PrimitiveTypeMask, MaxFadeDistance) > distance) {
            if (GFGetFastSubFrameTimer() < deadline)
                changed = LoadStaticContents(blocks, palette);
        } else
            UnloadStaticContents(blocks);
    }
    if (HasDynamicAreasFlag || StaticContentsLoaded) {
        for (int i = 0; i < 4; ++i) {
            if (Children[i]) {
                changed |= Children[i]->Update(blocks, palette, deadline);
                RenderUpdateNeeded |= Children[i]->RenderUpdateNeeded;
            }
        }
    }
    CObjectCacheGroupCollection* collection;
    for (collection = ObjectCacheGroupCollections; collection; collection = collection->Next)
        RenderUpdateNeeded |= collection->RenderUpdateNeeded;
    if (StaticContentsLoaded)
        for (collection = ObjectCacheGroupCollections; collection; collection = collection->Next)
            changed |= collection->StaticUpdate(blocks, palette, deadline);
    return changed;
}
