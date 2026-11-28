#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_GETUPDATEPARAM
#define FABLE_RETAIL_CVECTORMAP_CTOR
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

#include <stdio.h>
#include <stdlib.h>
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
namespace NParticleEngine {
template<class K,class V> CParticleUpdateParamCallback<K,V>::~CParticleUpdateParamCallback() {}
template<class K,class V> bool CParticleUpdateParamCallback<K,V>::GetUpdateParam(C3DVector&,V,float,bool,const CParticleUpdateParameters<K,V>&,unsigned long) { return false; }
class FillProbe : public CParticleUpdateParamCallback<unsigned long,unsigned long> {
public:
    unsigned long expected;
    unsigned int calls;
    bool GetUpdateParam(C3DVector&,unsigned long index,float,bool,const CParticleUpdateParameters<unsigned long,unsigned long>&,unsigned long) {
        ++calls;
        return index == expected;
    }
};
}
int main()
{
    using namespace NParticleEngine;
    CParticleUpdateParameters<unsigned long,unsigned long> params;
    FillProbe probe;
    probe.expected = 23;
    probe.calls = 0;
    ParamEntry value(31, ParamDesc(&probe,probe.expected));
    ParamEntry* data = static_cast<ParamEntry*>(malloc(4 * sizeof(ParamEntry)));
    if (data == 0) return 1;
    for (unsigned int count = 0; count != 5; ++count) {
        if (std::__uninitialized_fill_n(data,count,value,std::__false_type()) != data + count) return 2;
        for (unsigned int i = 0; i < count; ++i) {
            C3DVector output(0,0,0);
            if (data[i].first != 31 || !data[i].second.GetUpdateParam(output,0,false,params,0)) return 3;
            data[i].~ParamEntry();
        }
    }
    free(data);
    if (probe.calls != 10) return 4;
    puts("00b0f6b0_TEST PASS");
    return 0;
}
