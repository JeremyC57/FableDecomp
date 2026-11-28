#ifndef FABLE_ENGINE_NEXPLOSIVETRAILHELPERS_H
#define FABLE_ENGINE_NEXPLOSIVETRAILHELPERS_H
#ifdef FABLE_RETAIL_ICICLE_ASSAULT_PASS30
class CThing;
class C3DVector;
namespace NExplosiveTrailHelpers {
void FABLE_FASTCALL RandomiseThingOrientationAroundDirection(CThing& thing, const C3DVector& direction);
}
#endif
#endif
