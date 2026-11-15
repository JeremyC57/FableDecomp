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
#ifdef FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_GETUPDATEPARAM
 // Opt-in (wfD6, row 0x00B087C0): FableWin.pdb / Ego_r.pdb ?GetUpdateParam@?$CParticleUpdateParameters@KK@NParticleEngine@@QBE_NAAVC3DVector@@ABKM_NK@Z
 // (inline in retail: key test, CVectorMap::Find -> out-of-line LowerBound 0x00AF2C50, descriptor GetUpdateParam). Needs
 // FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_GETUPDATEPARAM. No layout change.
 bool GetUpdateParam(C3DVector& out,const K& key,float time,bool local,unsigned long flags)const{
  if(key){
   typename CVectorMap<K,CParticleUpdateParamDesc<K,V>,CParticleUpdateCCompareParamDesc<K> >::const_iterator it=UpdateParams.Find(key);
   if(it!=UpdateParams.end()) return it->second.GetUpdateParam(out,time,local,*this,flags);
  }
  return false;
 }
#endif
 CVectorMap<K,C3DVector,CParticleUpdateCCompareVector<K> > VectorParams;
 CVectorMap<K,CParticleUpdateParamDesc<K,V>,CParticleUpdateCCompareParamDesc<K> > UpdateParams;
};
}
FABLE_STATIC_ASSERT_AT(17, sizeof(NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>)==36);
#endif
