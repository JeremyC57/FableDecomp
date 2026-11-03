#ifndef FABLE_ENGINE_CRIGHTHANDEDSET_H
#define FABLE_ENGINE_CRIGHTHANDEDSET_H
#include "engine/C3DVector.h"
// Ego_r original class0x9f08: protected Up+0,Forward+12,size24.
// Native CThingCreatureBase::OnHit gets GetRHSet then reads Forward at+12.
class CRightHandedSet { protected: C3DVector Up,Forward; public: const C3DVector& GetForward() const { return Forward; } };
#endif
