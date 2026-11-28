// 00b158a0 NParticleEngine::CPSCEmitterGeneric::CollectUpdateParameters
// Leaf: ?CollectUpdateParameters@CPSCEmitterGeneric@NParticleEngine@@UAEXAAV?$CParticleUpdateParameters@KK@2@ABK@Z
// Flags: /Gr
// Inline-budget: the original CVectorMap insertion expands at the .Centre registration.
#define FABLE_RETAIL_CPSCEMITTERGENERIC_CTOR
#define FABLE_RETAIL_CPSCEMITTERGENERIC_ALLOC
#define FABLE_RETAIL_CPSCEMITTERGENERIC_COLLECT_PARAMS_PASS30
#define FABLE_RETAIL_ORBIT_UPDATE_PARAMS_PASS30
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#include "engine/CPSCEmitterGeneric.h"
#include "engine/CParticleUpdateParameters.h"

namespace NParticleEngine {
void CPSCEmitterGeneric::CollectUpdateParameters(
    CParticleUpdateParameters<unsigned long,unsigned long>& dest, const unsigned long& instance_name)
{
    dest.UpdateParams.Insert(MAKE_UPDATE_PARAM_FROM_BASE(instance_name, ".Centre"),
        CParticleUpdateParamDesc<unsigned long,unsigned long>(this, 0));
}
}
