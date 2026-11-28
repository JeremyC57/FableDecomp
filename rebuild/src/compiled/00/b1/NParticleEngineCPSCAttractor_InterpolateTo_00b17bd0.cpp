// 00b17bd0 NParticleEngine::CPSCAttractor::InterpolateTo
// Leaf: ?InterpolateTo@CPSCAttractor@NParticleEngine@@UAEXABVCPSCBase@2@M@Z
// Flags: /Gr
// Retail-asm-helper: GFFloatToLongNear
// Retail-asm: the integer interpolation at 00B17CB2 uses the shared rounding helper. The editor's long overload calls GFFloatToLongNear.
#define FABLE_RETAIL_CPSCATTRACTOR_INTERPOLATE_PASS30
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR
#include "rebuild_abi.h"
#include "engine/CPSCAttractor.h"
#include "engine/NParticleEngine_ParticleInterpolation.h"
#include "engine/GFDebugPDynamicCast.h"

namespace NParticleEngine {
void CPSCAttractor::InterpolateTo(const CPSCBase& to, const float interpolate)
{
    const CPSCAttractor* psc = GFDebugPDynamicCast(&to, static_cast<const CPSCAttractor*>(0));
    AttractorEnabled = GFInterpolate(AttractorEnabled, psc->AttractorEnabled, interpolate);
    AttractorPositionParam = GFInterpolateUpdateParams(AttractorPositionParam, psc->AttractorPositionParam, interpolate);
    AttractorUseParamPosition = GFInterpolate(AttractorUseParamPosition, psc->AttractorUseParamPosition, interpolate);
    AttractorPositionParamName = GFInterpolateVectorParams(AttractorPositionParamName, psc->AttractorPositionParamName, interpolate);
    AttractorInfluenceRadius = GFInterpolate(AttractorInfluenceRadius, psc->AttractorInfluenceRadius, interpolate);
    AttractorInfluenceForce = GFInterpolate(AttractorInfluenceForce, psc->AttractorInfluenceForce, interpolate);
    AttractorInfluenceFallOff = GFInterpolate(AttractorInfluenceFallOff, psc->AttractorInfluenceFallOff, interpolate);
    unsigned long size = AttractorUserPointsArray.size();
    for (long ci = 0; ci < size; ++ci) {
        AttractorUserPointsArray[ci] = GFInterpolate(AttractorUserPointsArray[ci], psc->AttractorUserPointsArray[ci], interpolate);
    }
}
}