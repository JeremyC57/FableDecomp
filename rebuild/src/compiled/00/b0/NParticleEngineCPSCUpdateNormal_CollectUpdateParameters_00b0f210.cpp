// 00b0f210 NParticleEngine::CPSCUpdateNormal::CollectUpdateParameters
// Leaf: ?CollectUpdateParameters@CPSCUpdateNormal@NParticleEngine@@UAEXAAV?$CParticleUpdateParameters@KK@2@ABK@Z
// Flags: /Gr
#define FABLE_RETAIL_CPSCUPDATENORMAL_COLLECT_PARAMS_PASS30
#define FABLE_RETAIL_ORBIT_UPDATE_PARAMS_PASS30
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#include "engine/CPSCUpdateNormal.h"
#include "engine/CParticleUpdateParameters.h"

namespace NParticleEngine {
void CPSCUpdateNormal::CollectUpdateParameters(CParticleUpdateParameters<unsigned long,unsigned long>& params, const unsigned long& prefix)
{
    unsigned long key = CCRC::Calc(0, "System Offset", 13);
    params.UpdateParams.Insert(key, CParticleUpdateParamDesc<unsigned long,unsigned long>(this, 0));
}
}
