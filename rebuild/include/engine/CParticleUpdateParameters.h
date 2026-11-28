#ifndef FABLE_ENGINE_CPARTICLEUPDATEPARAMETERS_H
#define FABLE_ENGINE_CPARTICLEUPDATEPARAMETERS_H
#include "engine/CPSCBase.h"
#include "engine/CVectorMap.h"
// Layout: both original PDB type records and donor Update02F3F4C0;
// native B0DC20 member accesses use the original retail container sizes.
#ifdef FABLE_RETAIL_ORBIT_UPDATE_PARAMS_PASS30
#include <string.h>
#include "engine/CCRC.h"
#endif
namespace NParticleEngine {
#ifdef FABLE_RETAIL_ORBIT_UPDATE_PARAMS_PASS30
// FableWin 02F3ED00 calls strlen and then CCRC::Calc. Retail folds the three string lengths.
inline unsigned long FABLE_FASTCALL MAKE_UPDATE_PARAM_FROM_BASE(unsigned long base, const char* update_param)
{
    return CCRC::Calc(base, update_param, (unsigned long)strlen(update_param));
}
#endif
template<class K,class V>class CParticleUpdateParameters:public CBaseClass {
public:
 template<class T>class CParticleUpdateCCompareVector:public CKeyPairCompareLess<T,C3DVector>{};
 template<class T>class CParticleUpdateCCompareParamDesc:public CKeyPairCompareLess<T,CParticleUpdateParamDesc<K,V> >{};
#ifdef FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_GETVECTOR_PASS26
 // Original02f36de0: nonzero key, Find, end check, output assignment; no extra locals.
 bool GetVectorParam(C3DVector& dest,const K& id)const {
  if(id){
   typename CVectorMap<K,C3DVector,CParticleUpdateCCompareVector<K> >::const_iterator found=VectorParams.Find(id);
   if(found!=VectorParams.end()){dest=found->second;return true;}
  }
  return false;
 }
#else
 bool GetVectorParam(C3DVector&,const K&)const;
#endif
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
