// 00b0fb40 CVectorMap::Insert for particle update descriptors
// Leaf: ?Insert@?$CVectorMap@KV?$CParticleUpdateParamDesc@KK@NParticleEngine@@V?$CParticleUpdateCCompareParamDesc@K@?$CParticleUpdateParameters@KK@2@@@QAEXABKABV?$CParticleUpdateParamDesc@KK@NParticleEngine@@@Z
// Flags: /Gr
#define FABLE_RETAIL_ORBIT_UPDATE_PARAMS_PASS30
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#include "engine/CParticleUpdateParameters.h"

using namespace NParticleEngine;
typedef CParticleUpdateParamDesc<unsigned long,unsigned long> CParamDesc;
typedef CParticleUpdateParameters<unsigned long,unsigned long>::CParticleUpdateCCompareParamDesc<unsigned long> CCompareParamDesc;

template void CVectorMap<unsigned long,CParamDesc,CCompareParamDesc>::Insert(
    const unsigned long&, const CParamDesc&);
