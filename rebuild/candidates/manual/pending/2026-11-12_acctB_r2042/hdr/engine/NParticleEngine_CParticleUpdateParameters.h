#ifndef FABLE_ENGINE_NPARTICLEENGINE_CPARTICLEUPDATEPARAMETERS_H
#define FABLE_ENGINE_NPARTICLEENGINE_CPARTICLEUPDATEPARAMETERS_H
// New shared header (w22 agent2042, row 0x007F2DC0 CTCLightningNode::FrameUpdate): NParticleEngine::CParticleUpdateParameters<K,V>, the
// keyed vector / update parameters a particle emitter is fed with. Ego_r.pdb / FableWin.pdb names: the nested comparators
// CParticleUpdateCCompareVector<K> / CParticleUpdateCCompareParamDesc<K>, the two CVectorMap members (VectorParams +0x04, UpdateParams
// +0x14: retail 16-byte CVectorMap, Dirty at +0x0D, so VectorParams.Dirty is +0x11: FrameUpdate's `mov byte ptr [esp+85h],1` after each
// insert into the object at [esp+74h]), the base CBaseClass (vtable 0x0125FFDC), ??0 (inline, out-of-line copy 0x006E24D0, landed), the
// virtual ??1 (0x006E2500) and ?Update@?$CParticleUpdateParameters@KK@NParticleEngine@@QAEXXZ (0x0076C750, landed). CParticleUpdateParamDesc
// <K,V> : CBaseClass { Callback, Index } (Ego_r.pdb, as the landed 0x00854B90 model). Needs FABLE_RETAIL_CVECTORMAP_INSERT and
// FABLE_RETAIL_CVECTORMAP_CTOR for the inline Insert / constructor. sizeof 0x24.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"
#include "engine/C3DVector.h"
#include "engine/CVectorMap.h"
namespace NParticleEngine {
template<class K,class V> class CParticleUpdateParamCallback;
template<class K,class V> class CParticleUpdateParamDesc : public CBaseClass {
private:
    CParticleUpdateParamCallback<K,V>* Callback;
    V Index;
};
template<class K,class V> class CParticleUpdateParameters : public CBaseClass {
public:
    template<class T> class CParticleUpdateCCompareVector : public CKeyPairCompareLess<T,C3DVector> { };
    template<class T> class CParticleUpdateCCompareParamDesc : public CKeyPairCompareLess<T,CParticleUpdateParamDesc<K,V> > { };
    CParticleUpdateParameters() { }                                     // 0x006E24D0 (<unsigned long, unsigned long>)
    virtual ~CParticleUpdateParameters();                               // 0x006E2500
    void Update();                                                      // 0x0076C750
    CVectorMap<K,C3DVector,CParticleUpdateCCompareVector<K> > VectorParams;                                  // +0x04
    CVectorMap<K,CParticleUpdateParamDesc<K,V>,CParticleUpdateCCompareParamDesc<K> > UpdateParams;           // +0x14
};
}
#endif
