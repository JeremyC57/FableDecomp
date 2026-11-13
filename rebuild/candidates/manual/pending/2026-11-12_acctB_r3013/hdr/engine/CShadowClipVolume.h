#ifndef FABLE_ENGINE_CSHADOWCLIPVOLUME_H
#define FABLE_ENGINE_CSHADOWCLIPVOLUME_H
// CShadowClipVolume (chunk 727): Ego_r.pdb class, 0x98 bytes -- ClipPlanes[8] +0x00, ClipPlaneCount +0x80, BoundingSphere +0x84,
// ClipToSphere +0x94 (struct_layouts_egor.tsv; retail agrees: 0x00B4CE20 / 0x00B4BCC0 write +0x80 / +0x94 of the volumes at
// CEngineShadowRenderer +0x11C / +0x1B4). Reset / AddPlane / SetSphere: FableWin ?Reset@CShadowClipVolume@@QAEXXZ,
// ?AddPlane@CShadowClipVolume@@QAEXABVCPlane@@@Z, ?SetSphere@CShadowClipVolume@@QAEXABVC3DBoundingSphere@@@Z, inline in retail
// (0x00B4BF59: count 0 and the flag cleared; 0x00B4C07B: the plane copied to ClipPlanes[count], then count + 1; 0x00B4C510: the
// sphere copied, then the flag set).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CPlane.h"
#ifndef FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#endif
#include "engine/C3DBoundingSphere.h"
#ifdef FABLE_RETAIL_CSHADOWCLIPVOLUME_CLIPBOUNDINGBOX
#ifndef FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX
#endif
#include "engine/C3DBoundingBox.h"
#endif
class C3DBoundingBox;
class CShadowClipVolume {
public:
    CPlane            ClipPlanes[8];                                     // +0x00
    long              ClipPlaneCount;                                    // +0x80
    C3DBoundingSphere BoundingSphere;                                    // +0x84
    bool              ClipToSphere;                                      // +0x94
    void Reset()
    {
        ClipPlaneCount = 0;
        ClipToSphere = false;
    }
    void AddPlane(const CPlane& plane)
    {
        ClipPlanes[ClipPlaneCount] = plane;
        ClipPlaneCount++;
    }
    void SetSphere(const C3DBoundingSphere& sphere)
    {
        BoundingSphere = sphere;
        ClipToSphere = true;
    }
#ifdef FABLE_RETAIL_CSHADOWCLIPVOLUME_CLIPBOUNDINGBOX
    // Retail opt-in (chunk 2028, row 0x00B9A4A0 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive): Ego_r.pdb / FableWin.pdb
    // ?ClipBoundingBox@CShadowClipVolume@@QBE_NABVC3DBoundingBox@@@Z, inline: expanded at 0x00B9A743..0x00B9A946 while this TU keeps an
    // unreferenced out-of-line copy (landed 0x00B9C2E0, whose body this is). C3DBoundingBox::CalcShortestDistanceToPointSquared (the
    // hand-SSE kernel, engine/C3DBoundingBoxSSE.h) is defined by the including row. FABLE_FORCEINLINE: the c2 inline score at the
    // row's site is 534 v the threshold 74 (plain inline leaves it called), and the TU's out-of-line copy has no caller.
    FABLE_FORCEINLINE bool ClipBoundingBox(const C3DBoundingBox& box) const
    {
        if (ClipToSphere) {
            if (box.CalcShortestDistanceToPointSquared(BoundingSphere.Center) > BoundingSphere.Radius * BoundingSphere.Radius)
                return false;
        }
        for (long i = 0; i < ClipPlaneCount; ++i) {
            const CPlane& plane = ClipPlanes[i];
            float nx = plane.Normal.X, ny = plane.Normal.Y, nz = plane.Normal.Z;
            float x = nx > 0.0f ? box.PeekMinCoord().X : box.PeekMaxCoord().X;
            float y = ny > 0.0f ? box.PeekMinCoord().Y : box.PeekMaxCoord().Y;
            float z = nz > 0.0f ? box.PeekMinCoord().Z : box.PeekMaxCoord().Z;
            if (x * nx + y * ny + z * nz > plane.Distance)
                return false;
        }
        return true;
    }
#endif
};
FABLE_STATIC_ASSERT(sizeof(CShadowClipVolume) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CShadowClipVolume, ClipPlaneCount) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CShadowClipVolume, ClipToSphere) == 0x94);
#endif // FABLE_ENGINE_CSHADOWCLIPVOLUME_H
