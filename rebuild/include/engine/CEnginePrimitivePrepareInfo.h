#if defined(FABLE_RETAIL_STEALTH_DRAW) || defined(FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO)
#ifndef FABLE_ENGINE_CENGINEPRIMITIVEPREPAREINFO_H
#define FABLE_ENGINE_CENGINEPRIMITIVEPREPAREINFO_H
#include "engine/CInterpolationInfo.h"
#include "engine/CCamera.h"
class CIEngine;
// Layout: complete original Ego2ff81/FW32b67,size116; Engine is a genuine reference.
class CEnginePrimitivePrepareInfo{
public:
 const CInterpolationInfo Interpolate;
 CIEngine & Engine;
 bool Edit2DEngine;
 CCamera Camera;
 CEnginePrimitivePrepareInfo(const CEnginePrimitivePrepareInfo & p0);
 CEnginePrimitivePrepareInfo(const CInterpolationInfo & p0, CIEngine & p1, bool p2, const CCamera & p3);
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitivePrepareInfo)==116);
#endif
#else
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

#endif
