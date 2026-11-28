// 00b1b380 NParticleEngine::CPSCSpline::CollectUpdateParameters
// Leaf: ?CollectUpdateParameters@CPSCSpline@NParticleEngine@@UAEXAAV?$CParticleUpdateParameters@KK@2@ABK@Z
// Flags: /Gr
// Inline-budget: the original CVectorMap insertion expands for the direct instance key.
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSPLINEBASE_MEMBERS
#define FABLE_RETAIL_ORBIT_UPDATE_PARAMS_PASS30
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#include "engine/CPSCSpline.h"
#include "engine/CParticleUpdateParameters.h"

namespace NParticleEngine {
void CPSCSpline::CollectUpdateParameters(
    CParticleUpdateParameters<unsigned long,unsigned long>& dest, const unsigned long& instance_name)
{
    dest.UpdateParams.Insert(instance_name, CParticleUpdateParamDesc<unsigned long,unsigned long>(this, 0));
}
}
