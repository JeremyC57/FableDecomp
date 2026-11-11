// NParticleEngine::CPSCBase::GetApproximateSplineLength() -- the base component has no spline: length 0
// Leaf: ?GetApproximateSplineLength@CPSCBase@NParticleEngine@@UAEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2AE0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 31 of NParticleEngine::CPSCLight's vtable 0x012A0624 (the slot holds this body in 8 vtable(s), owner NParticleEngine::CPSCBase); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts GetApproximateSplineLength(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetApproximateSplineLength@CPSCBase@NParticleEngine@@UAEMXZ; `ret` matches the signature.
// Was: NParticleEngine::CPSCBase::vfunc_31 (rebuild/src/compiled/00/af/NParticleEngineCPSCBase_vfunc31_00af2ae0.cpp).
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
float CPSCBase::GetApproximateSplineLength()
{
    return 0.0f;
}
}
