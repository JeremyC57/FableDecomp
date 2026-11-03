#ifndef FABLE_ENGINE_CENGINEPRIMITIVEPREPAREINFO_H
#define FABLE_ENGINE_CENGINEPRIMITIVEPREPAREINFO_H
// New shared header (chunk 728, row 0x0074BC20): Ego_r.pdb CEnginePrimitivePrepareInfo, sizeof 0x74 (struct_layouts_egor.tsv):
// Interpolate +0x00 (CInterpolationInfo, 0x34; no shared header: opaque), Engine +0x34 (CIEngine*), Edit2DEngine +0x38, Camera +0x3C.
// No STL member, so the offsets hold in retail (every DrawPreparePrimitivesForRendering reads Engine at +0x34: DrawDebugInfo 0x0074BDC4,
// the landed 0x007DCE40 / 0x0071AB20).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CCamera.h"
class CIEngine;
class CEnginePrimitivePrepareInfo {
public:
    unsigned char Interpolate[0x34];   // +0x00 CInterpolationInfo
    CIEngine*     Engine;              // +0x34
    bool          Edit2DEngine;        // +0x38
    CCamera       Camera;              // +0x3C
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitivePrepareInfo) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitivePrepareInfo, Engine) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitivePrepareInfo, Camera) == 0x3C);
#endif
