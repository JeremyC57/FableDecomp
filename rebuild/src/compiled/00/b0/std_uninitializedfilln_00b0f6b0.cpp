// 00b0f6b0 std::__uninitialized_fill_n
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@KV?$CParticleUpdateParamDesc@KK@NParticleEngine@@@std@@IU12@@std@@YIPAU?$pair@KV?$CParticleUpdateParamDesc@KK@NParticleEngine@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Flags: /Gr
#define FABLE_RETAIL_ORBIT_UPDATE_PARAMS_PASS30
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C3DVECTOR
#include "engine/CParticleUpdateParameters.h"

typedef NParticleEngine::CParticleUpdateParamDesc<unsigned long,unsigned long> ParamDesc;
typedef std::pair<unsigned long,ParamDesc> ParamEntry;
template ParamEntry* std::__uninitialized_fill_n(ParamEntry*,unsigned int,const ParamEntry&,const std::__false_type&);
