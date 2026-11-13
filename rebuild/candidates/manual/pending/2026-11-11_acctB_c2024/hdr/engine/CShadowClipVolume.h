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
};
FABLE_STATIC_ASSERT(sizeof(CShadowClipVolume) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CShadowClipVolume, ClipPlaneCount) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CShadowClipVolume, ClipToSphere) == 0x94);
#endif // FABLE_ENGINE_CSHADOWCLIPVOLUME_H
