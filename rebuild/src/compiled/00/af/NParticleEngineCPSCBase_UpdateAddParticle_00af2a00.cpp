// NParticleEngine::CPSCBase::UpdateAddParticle(NParticleEngine::CParticleSystem&, NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long> const&, C3DVector, C3DVector, float) -- the base component adds nothing to a new particle
// Leaf: ?UpdateAddParticle@CPSCBase@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@VC3DVector@@2M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2A00, thiscall, `ret 36`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 12 of NParticleEngine::CPSCSplineBase's vtable 0x012A0B14 (the slot holds this body in 11 vtable(s), owner NParticleEngine::CPSCBase); the Xbox 360 Test PDB virtual order (third source) aligned on 11 TLC-named slots of that vtable puts UpdateAddParticle(NParticleEngine::CParticleSystem &, NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long> const &, C3DVector, C3DVector, float) at this slot [gap kind GSC]; Ego_r/FableWin name set: ?UpdateAddParticle@CPSCBase@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@VC3DVector@@2M@Z; `ret` matches the signature.
// Was: NParticleEngine::CPSCBase::vfunc_12 (rebuild/src/compiled/00/af/gapscan_sub00af2a00_00af2a00.cpp).
// No shared header for NParticleEngine::CParticleUpdateParamCallback / CPSCBase: minimal local definitions on the RTTI bases
// (CPSCBase : CParticleUpdateParamCallback<unsigned long,unsigned long> : CBaseClassNonCopyable), Ego_r.pdb member names; the
// declaration order of the virtuals is not the slot order (leaf bodies only).
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C3DVector.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
class CEnginePrimitivePrepareInfo;
class CEnginePrimitiveHandle;
namespace NParticleEngine {
class CParticleSystem;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable {
public:
    virtual bool GetUpdateParam(C3DVector&, K, float, bool, const CParticleUpdateParameters<K, V>&, V);   // slot 1
};
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    virtual void CollectUpdateParameters(CParticleUpdateParameters<unsigned long, unsigned long>&, const unsigned long&);     // slot 10
    virtual void UpdateAddParticle(CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, C3DVector, C3DVector, float);   // slot 12
    virtual void PreparePrimitives(float, const CEnginePrimitivePrepareInfo&, CParticleSystem&,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle&);   // slot 13
    virtual float GetApproximateSplineLength();                                                               // slot 31
    CParticleSystem* ParticleSystem;                         // +0x04
    bool Initialised : 1;                                    // +0x08
    bool UpdateStarted : 1;
};
void CPSCBase::UpdateAddParticle(CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, C3DVector, C3DVector, float)
{
}
}
