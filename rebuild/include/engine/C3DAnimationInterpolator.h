#ifndef FABLE_ENGINE_C3DANIMATIONINTERPOLATOR_H
#define FABLE_ENGINE_C3DANIMATIONINTERPOLATOR_H
#include "engine/C3DAnimationBlendState.h"
// Original complete20-byte class and public states, both PDBs; native ctor4C2E20/dtor4C2E90.
class C3DAnimationInterpolator : public CBaseClass {
public:
 C3DAnimationInterpolator();
 virtual ~C3DAnimationInterpolator();
 C3DAnimationBlendState StartState,EndState;
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimationInterpolator)==20);
#endif
