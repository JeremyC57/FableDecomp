// 00b16aa0 NParticleEngine::CPSCOrbit::GetUpdateParam
// Leaf: ?GetUpdateParam@CPSCOrbit@NParticleEngine@@UAE_NAAVC3DVector@@KM_NABV?$CParticleUpdateParameters@KK@2@K@Z
// Flags: /d2inlT1000
// Inline-budget: both interpolation helpers expand at the two original call sites in retail. The default retains calls to both.
#define FABLE_RETAIL_CPSCBASE_GETUPDATESTARTED_PASS27
#define FABLE_RETAIL_C3DVECTOR

#define FABLE_RETAIL_C3DVECTOR_INTERPOLATE
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ

#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#include "engine/NParticleEngine_CPSCOrbit.h"

namespace NParticleEngine {
inline C3DVector CPSCOrbit::COrbitState::GetSplinePosition(float t) const
{
    float t2 = t*t;
    float t3 = t2*t;
    float wpos_from = 2.0f*t3 - 3.0f*t2 + 1.0f;
    float wpos_to = -2.0f*t3 + 3.0f*t2;
    float wtan_from = t3 - 2.0f*t2 + t;
    float wtan_to = t3 - t2;
    C3DVector result = FromPosition*wpos_from + ToPosition*wpos_to + FromTangent*wtan_from + ToTangent*wtan_to;
    return result;
}

bool CPSCOrbit::GetUpdateParam(C3DVector& dest, unsigned long index, float interp,
                               bool tangent, const CParticleUpdateParameters<unsigned long,unsigned long>& params,
                               unsigned long nested)
{
    if (!GetUpdateStarted())
        StartUpdate(params);
    COrbitState* orbit_state;
    switch (index) {
    case 0: orbit_state = &OrbitStates[0]; break;
    case 1: orbit_state = &OrbitStates[1]; break;
    case 2: orbit_state = &OrbitStates[2]; break;
    default: return false;
    }
    if (tangent) {
        dest = orbit_state->FromTangent.Interpolate(orbit_state->ToTangent,interp);
        if (dest.IsNonZero())
            dest.Normalise();
    } else {
        dest = orbit_state->GetSplinePosition(interp);
    }
    return true;
}
}