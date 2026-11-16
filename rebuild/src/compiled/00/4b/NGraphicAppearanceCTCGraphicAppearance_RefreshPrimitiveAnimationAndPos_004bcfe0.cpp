// NGraphicAppearance::CTCGraphicAppearance::RefreshPrimitiveAnimationAndPos(CPrimitive* primitive, const C3DAnimationInterpolator& animation, const C3DPositionInterpolator& position, const C3DOrientationInterpolator& orientation) -- hand the primitive manager a temporary CEnginePrimitiveAnimation built from the three interpolators
// Leaf: ?RefreshPrimitiveAnimationAndPos@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAVCPrimitive@2@ABVC3DAnimationInterpolator@@ABVC3DPositionInterpolator@@ABVC3DOrientationInterpolator@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BCFE0, length 92 bytes. Callees: CEnginePrimitiveAnimation::CEnginePrimitiveAnimation
// @ 0x004C3D30, the primitive manager's virtual slot 24 (+0x60), the interpolator base destructor @ 0x0099A2E0 (manifest
// "CBase::CBase", propagated; run for the orientation and position members), C3DAnimationInterpolator::~C3DAnimationInterpolator
// @ 0x004C2E90. The manager is +0x34 of the engine object the global @ 0x013B8A18 points to.
// Layout (Ego_r.pdb CEnginePrimitiveAnimation, 100 bytes): Anim (C3DAnimationInterpolator, 20) +0x00, WorldPos
// (C3DPositionInterpolator, 28) +0x14, Orientation (C3DOrientationInterpolator, 52) +0x30.
// Shape: one statement -- the virtual call with the animation temporary built in the 0x64-byte frame; VC fetches the
// manager's vtable (esi) before building the temporary and re-reads the manager into ecx after it; the temporary's implicit
// destructor runs its members' destructors in reverse order.
// Layout (Ego_r.pdb CTCGraphicAppearance, 116 bytes; retail 0x5c: the two STLport maps 12 bytes, the two STLport lists one
// The global 0x013B8A18 is g_pCurrentPrimitivePrepareInfo (descriptive: no PDB, FSE or EgoCore name; SetPrimitive 0x004BD6E8 stores
//   &info, a const CEnginePrimitivePrepareInfo&, into it), read through CEnginePrimitivePrepareInfo::Engine (+0x34, CIEngine*): the
//   "primitive manager" is CIEngine slot 24 ?SetPrimitiveAnimation@CIEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitiveAnimation@@@Z.
// CEnginePrimitiveAnimation has no shared header: modelled here from Ego_r.pdb (100 bytes, Anim +0, WorldPos +0x14, Orientation +0x30;
//   ??0CEnginePrimitiveAnimation@@QAE@ABVC3DAnimationInterpolator@@ABVC3DPositionInterpolator@@ABVC3DOrientationInterpolator@@@Z).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_RANDOMMORPH
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FULLSCALE
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_BASE
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_IMPLICIT_DTOR
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_IMPLICIT_DTOR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/C3DAnimationInterpolator.h"
#include "engine/C3DPositionInterpolator.h"
#include "engine/C3DOrientationInterpolator.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"

class CEnginePrimitiveAnimation {
public:
    CEnginePrimitiveAnimation(const C3DAnimationInterpolator& anim, const C3DPositionInterpolator& world_pos,
                              const C3DOrientationInterpolator& orientation);   // 0x004C3D30
    C3DAnimationInterpolator   Anim;                // +0x00
    C3DPositionInterpolator    WorldPos;            // +0x14
    C3DOrientationInterpolator Orientation;         // +0x30
};

extern const CEnginePrimitivePrepareInfo* g_pCurrentPrimitivePrepareInfo;   // 0x013B8A18

namespace NGraphicAppearance {

void CTCGraphicAppearance::RefreshPrimitiveAnimationAndPos(CPrimitive* primitive, const C3DAnimationInterpolator& anim,
                                                           const C3DPositionInterpolator& pos, const C3DOrientationInterpolator& orientation)
{
    g_pCurrentPrimitivePrepareInfo->Engine->SetPrimitiveAnimation(primitive->PrimitiveHandle,
                                                                 CEnginePrimitiveAnimation(anim, pos, orientation));
}

}
