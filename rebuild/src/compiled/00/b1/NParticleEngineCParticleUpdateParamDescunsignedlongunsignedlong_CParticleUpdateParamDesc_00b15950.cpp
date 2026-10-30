// NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long>::CParticleUpdateParamDesc() -- a parameter description with no callback
// Leaf: ??0?$CParticleUpdateParamDesc@KK@NParticleEngine@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B15950, 25 bytes, thiscall, plain `ret`.
// Identity (proven): it stores vtable 0x0125FEE4 (RTTI .?AV?$CParticleUpdateParamDesc@KK@NParticleEngine@@) after the
//   CBaseClass constructor and clears Callback (+4; Ego_r.pdb CParticleUpdateParamDesc<unsigned long, unsigned long> =
//   {vptr, Callback, Index}). The manifest's "C3DAnimationBlendState::C3DAnimationBlendState" is propagated.
// Callees: CBaseClass::CBaseClass() 0x0099A2D0.
// Shape (family M193, representative 0x006920F0): `CParticleUpdateParamDesc() : Callback(0) { }`. Speed TU.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
namespace NParticleEngine {
template <class T1, class T2> class CParticleUpdateParamCallback;
template <class T1, class T2> class CParticleUpdateParamDesc : public CBaseClass {
public:
    CParticleUpdateParamDesc();
    virtual ~CParticleUpdateParamDesc();
    CParticleUpdateParamCallback<T1, T2>* Callback;       // +0x04
    unsigned long Index;                                  // +0x08
};
template <class T1, class T2> CParticleUpdateParamDesc<T1, T2>::CParticleUpdateParamDesc()
    : Callback(0)
{
}
template CParticleUpdateParamDesc<unsigned long, unsigned long>::CParticleUpdateParamDesc();
}