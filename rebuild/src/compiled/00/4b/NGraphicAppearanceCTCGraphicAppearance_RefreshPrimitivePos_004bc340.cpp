// NGraphicAppearance::CTCGraphicAppearance::RefreshPrimitivePos(CPrimitive* primitive, const C3DPositionInterpolator& position, const C3DOrientationInterpolator& orientation) -- hand the engine the primitive's placement at the start of the interpolation (t = 0)
// Leaf: ?RefreshPrimitivePos@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAVCPrimitive@2@ABVC3DPositionInterpolator@@ABVC3DOrientationInterpolator@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BC340, length 202 bytes (+ int3 padding). __fastcall, `ret 0Ch`.
//         Ego_r.pdb: `?RefreshPrimitivePos@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAVCPrimitive@2@ABVC3DPosition
//         Interpolator@@ABVC3DOrientationInterpolator@@@Z`.
// Callees: CRightHandedSet::Interpolate(const CRightHandedSet&, float) const @ 0x00A23190 (by hidden pointer), the engine's
//          virtual SetPrimitivePosition(CEnginePrimitiveHandle&, const CEnginePrimitivePosition&) (slot 25, +0x64) on
//          g_pEngineGlobals->+0x34 (global @ 0x013B8A18). Float pool 0x0122DEDC = 0.0f.
// Shape: one statement -- a CEnginePrimitivePosition temporary built from the interpolated orientation (evaluated first)
//        and position `Start + (End - Start) * t` through named-result C3DVector operators.
// The global 0x013B8A18 is g_pCurrentPrimitivePrepareInfo (descriptive; see row 0x004BCFE0), read through
//   CEnginePrimitivePrepareInfo::Engine (+0x34, CIEngine*): CIEngine slot 25 SetPrimitivePosition.
// CEnginePrimitivePosition has no shared header: modelled here from Ego_r.pdb (36 bytes: WorldPos +0, RightHandedSet +0x0C).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_INTERPOLATE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_REFRESHPRIMITIVEPOS
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_BASE
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_PASS22_GETINTERPOLATED
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_PASS22_GETINTERPOLATED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_INTERPOLATE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CRightHandedSet.h"
#include "engine/C3DPositionInterpolator.h"
#include "engine/C3DOrientationInterpolator.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/NGraphicAppearance_CPrimitive.h"

class CEnginePrimitivePosition {                    // 36 bytes (Ego_r.pdb)
public:
    C3DVector       WorldPos;                       // +0x00
    CRightHandedSet RightHandedSet;                 // +0x0C
    CEnginePrimitivePosition(const C3DVector& world_pos, const CRightHandedSet& right_handed_set)
        : WorldPos(world_pos), RightHandedSet(right_handed_set) {}
};

extern const CEnginePrimitivePrepareInfo* g_pCurrentPrimitivePrepareInfo;   // 0x013B8A18

namespace NGraphicAppearance {

void CTCGraphicAppearance::RefreshPrimitivePos(CPrimitive* primitive, const C3DPositionInterpolator& position,
                                               const C3DOrientationInterpolator& orientation)
{
    g_pCurrentPrimitivePrepareInfo->Engine->SetPrimitivePosition(primitive->PrimitiveHandle,
        CEnginePrimitivePosition(position.GetInterpolated(0.0f), orientation.GetInterpolated(0.0f)));
}

}
