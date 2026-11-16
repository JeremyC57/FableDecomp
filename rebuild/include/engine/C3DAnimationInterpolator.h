#ifndef FABLE_ENGINE_C3DANIMATIONINTERPOLATOR_H
#define FABLE_ENGINE_C3DANIMATIONINTERPOLATOR_H
#include "engine/C3DAnimationBlendState.h"
// Original complete20-byte class and public states, both PDBs; native ctor4C2E20/dtor4C2E90.
class C3DAnimationInterpolator : public CBaseClass {
public:
#ifndef FABLE_RETAIL_C3DANIMATIONINTERPOLATOR_IMPLICIT
 C3DAnimationInterpolator();
#ifndef FABLE_RETAIL_C3DANIMATIONINTERPOLATOR_GENERATED_DTOR_PASS23
 virtual ~C3DAnimationInterpolator();
#endif // Both original dtor0x107 generated; default/two-state ctors are user0x3.
#ifdef FABLE_RETAIL_C3DANIMATIONINTERPOLATOR_GENERATED_DTOR_PASS23
 C3DAnimationInterpolator(const C3DAnimationBlendState&,const C3DAnimationBlendState&);
#endif
#endif
 C3DAnimationBlendState StartState,EndState;
};
FABLE_STATIC_ASSERT_AT(13, sizeof(C3DAnimationInterpolator)==20);
#endif
