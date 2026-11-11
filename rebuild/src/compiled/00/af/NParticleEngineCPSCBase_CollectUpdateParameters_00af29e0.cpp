// NParticleEngine::CPSCBase::CollectUpdateParameters(NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>&, unsigned long const&) -- the base component collects no update parameters
// Leaf: ?CollectUpdateParameters@CPSCBase@NParticleEngine@@UAEXAAV?$CParticleUpdateParameters@KK@2@ABK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF29E0, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 10 of NParticleEngine::CPSCSplineBase's vtable 0x012A0B14 (the slot holds this body in 8 vtable(s), owner NParticleEngine::CPSCBase); the Xbox 360 Test PDB virtual order (third source) aligned on 11 TLC-named slots of that vtable puts CollectUpdateParameters(NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long> &, unsigned long const &) at this slot [gap kind GSC]; Ego_r/FableWin name set: ?CollectUpdateParameters@CPSCBase@NParticleEngine@@UAEXAAV?$CParticleUpdateParameters@KK@2@ABK@Z; `ret` matches the signature.
// Was: NParticleEngine::CPSCBase::vfunc_10 (rebuild/src/compiled/00/af/gapscan_sub00af29e0_00af29e0.cpp).
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
void CPSCBase::CollectUpdateParameters(CParticleUpdateParameters<unsigned long, unsigned long>&, const unsigned long&)
{
}
}
