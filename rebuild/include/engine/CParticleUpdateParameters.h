#ifndef FABLE_ENGINE_CPARTICLEUPDATEPARAMETERS_H
#define FABLE_ENGINE_CPARTICLEUPDATEPARAMETERS_H
#include "engine/CPSCBase.h"
#include "engine/CVectorMap.h"
// Layout: both original PDB type records and donor Update02F3F4C0;
// native B0DC20 member accesses use the original retail container sizes.
namespace NParticleEngine {
template<class K,class V>class CParticleUpdateParameters:public CBaseClass {
public:
 template<class T>class CParticleUpdateCCompareVector:public CKeyPairCompareLess<T,C3DVector>{};
 template<class T>class CParticleUpdateCCompareParamDesc:public CKeyPairCompareLess<T,CParticleUpdateParamDesc<K,V> >{};
 bool GetVectorParam(C3DVector&,const K&)const;
 CVectorMap<K,C3DVector,CParticleUpdateCCompareVector<K> > VectorParams;
 CVectorMap<K,CParticleUpdateParamDesc<K,V>,CParticleUpdateCCompareParamDesc<K> > UpdateParams;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>)==36);
#endif
