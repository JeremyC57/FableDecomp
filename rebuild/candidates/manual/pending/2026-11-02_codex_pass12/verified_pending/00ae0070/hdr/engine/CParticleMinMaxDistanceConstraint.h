#ifndef FABLE_ENGINE_CPARTICLEMINMAXDISTANCECONSTRAINT_H
#define FABLE_ENGINE_CPARTICLEMINMAXDISTANCECONSTRAINT_H
// Layout: original Ego_r complete class, native stride16 and accesses0/4/8/12.
class CParticleMinMaxDistanceConstraint {public: unsigned long Index1; unsigned long Index2; float MinDistance; float MaxDistance;};
FABLE_STATIC_ASSERT(sizeof(CParticleMinMaxDistanceConstraint)==16);
#endif
