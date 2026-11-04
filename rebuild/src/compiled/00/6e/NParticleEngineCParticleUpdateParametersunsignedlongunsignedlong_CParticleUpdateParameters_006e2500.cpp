// NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::~CParticleUpdateParameters() -- compiler-generated dtor: ~UpdateParams (each param desc), ~VectorParams, then ~CBaseClass
// Leaf: ??1?$CParticleUpdateParameters@KK@NParticleEngine@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E2500 (compiler-generated: no user body).
// Identity: Ego_r.pdb ??1?$CParticleUpdateParameters@KK@NParticleEngine@@UAE@XZ; VectorParams CVectorMap +4, UpdateParams +0x14.
// Shape: emitted through a qualified destructor call helper; members destroyed in reverse order.
#include "engine/CBaseClass.h"
#include "engine/C3DVector.h"
#include "engine/CVectorMap.h"

namespace NParticleEngine {
template <class T1, class T2> class CParticleUpdateParamCallback;

template <class T1, class T2> class CParticleUpdateParamDesc : public CBaseClass {   // RTTI .?AV?$CParticleUpdateParamDesc@KK@NParticleEngine@@ : CBaseClass
public:
    CParticleUpdateParamCallback<T1, T2>* Callback;                  // +0x04
    unsigned long                         Index;                     // +0x08
};

template <class T1, class T2> class CParticleUpdateParameters : public CBaseClass {   // RTTI .?AV?$CParticleUpdateParameters@KK@NParticleEngine@@ : CBaseClass
public:
    template <class K> class CParticleUpdateCCompareVector { };
    template <class K> class CParticleUpdateCCompareParamDesc { };
    CVectorMap<unsigned long, C3DVector, CParticleUpdateCCompareVector<unsigned long> >                             VectorParams;   // +0x04
    CVectorMap<unsigned long, CParticleUpdateParamDesc<T1, T2>, CParticleUpdateCCompareParamDesc<unsigned long> > UpdateParams;   // +0x14
};
}

typedef NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> CParticleUpdateParametersKK;
FABLE_STATIC_ASSERT(sizeof(CParticleUpdateParametersKK) == 0x24);

// The retail callers (28, e.g. 0x006E2553) call the compiler-generated destructor out of line; an explicit call emits it here.
void DestroyParticleUpdateParameters(CParticleUpdateParametersKK* parameters)
{
    parameters->CParticleUpdateParametersKK::~CParticleUpdateParametersKK();
}