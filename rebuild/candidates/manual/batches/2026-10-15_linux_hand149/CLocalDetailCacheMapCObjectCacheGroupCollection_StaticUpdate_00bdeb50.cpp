// CLocalDetailCacheMap::CObjectCacheGroupCollection::StaticUpdate(CEngineStaticMapFileBlockManager*, const CObjectTypeCollectionPalette&, double) -- stream this cache group in while the camera is inside its (video-option adjusted) fade range and there is frame time left, otherwise unload it
// Leaf: ?StaticUpdate@CObjectCacheGroupCollection
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDEB50, 143 bytes (+ int3 padding; `ret 10h`).
// Callees: CEngineLocalDetailGenerator::ModifyFadeDistanceForVideoOptions(unsigned long, float) @ 0x00B597E0 (generator global
//   0x01436E88), GFGetFastSubFrameTimer @ 0x009E1C70 (`__fastcall`, double), LoadContents @ 0x00BDE860, UnloadContents @ 0x00BDD9A0.
// Layout = Ego_r.pdb CObjectCacheGroupCollection: BoundingSphere +8 (centre, radius +0x14), MaxFadeDistance +0x18,
//   PrimitiveTypeMask +0x1c; the camera global 0x01436EA0 keeps its world position at +0x54.
// Shape: the distance from the camera to the sphere surface is a float local (the `push ecx` word); `fade > distance` then
//   `GFGetFastSubFrameTimer() < deadline` (`test ah,5 ; jp`) guard the load, whose bool is returned; the unload path returns false.
#include <math.h>
#pragma intrinsic(sqrt)
struct C3DVector {
    float X, Y, Z;
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
struct CMovableResource {
    virtual void v00();
};
struct CLocalDetailCacheMap {
    struct CObjectTypeCollectionPalette;
    struct CObjectCacheGroupCollection : CMovableResource {
        CObjectCacheGroupCollection* Next;                  // +0x04
        C3DBoundingSphere            BoundingSphere;        // +0x08
        float                        MaxFadeDistance;       // +0x18
        unsigned long                PrimitiveTypeMask;     // +0x1c
        bool LoadContents(CEngineStaticMapFileBlockManager* blocks, const CObjectTypeCollectionPalette& palette);   // 0x00BDE860
        void UnloadContents(CEngineStaticMapFileBlockManager* blocks);                                            // 0x00BDD9A0
        bool StaticUpdate(CEngineStaticMapFileBlockManager* blocks, const CObjectTypeCollectionPalette& palette, double deadline);
    };
};

bool CLocalDetailCacheMap::CObjectCacheGroupCollection::StaticUpdate(CEngineStaticMapFileBlockManager* blocks,
                                                                     const CObjectTypeCollectionPalette& palette, double deadline)
{
    const C3DVector& camera = g_pEngineCamera->Position;
    float dx = BoundingSphere.Centre.X - camera.X;
    float dy = BoundingSphere.Centre.Y - camera.Y;
    float dz = BoundingSphere.Centre.Z - camera.Z;
    float distance = (float)sqrt(dx * dx + dy * dy + dz * dz) - BoundingSphere.Radius;
    if (g_pLocalDetailGenerator->ModifyFadeDistanceForVideoOptions(PrimitiveTypeMask, MaxFadeDistance) > distance) {
        if (GFGetFastSubFrameTimer() < deadline)
            return LoadContents(blocks, palette);
        return false;
    }
    UnloadContents(blocks);
    return false;
}
