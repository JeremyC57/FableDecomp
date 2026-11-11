// NParticleEngine::CPSCDecalRenderer::GetDecalLifeSecs(float&) -- the decal life in seconds (DecalLifeSecs +0x0C), always available
// Leaf: ?GetDecalLifeSecs@CPSCDecalRenderer@NParticleEngine@@UAE_NAAM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1FF70, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 23 of NParticleEngine::CPSCDecalRenderer's vtable 0x012A09BC (the slot holds this body in 1 vtable(s), owner NParticleEngine::CPSCDecalRenderer); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts GetDecalLifeSecs(float &) at this slot [gap kind GSC]; Ego_r/FableWin name set: ?GetDecalLifeSecs@CPSCDecalRenderer@NParticleEngine@@UAE_NAAM@Z; `ret` matches the signature.
// Was: _unattributed::vfunc_23 (rebuild/src/compiled/00/b1/unattributed_vfunc23_00b1ff70.cpp).
// No shared header for NParticleEngine::CParticleUpdateParamCallback / CPSCBase: minimal local definitions on the RTTI bases
// (CPSCBase : CParticleUpdateParamCallback<unsigned long,unsigned long> : CBaseClassNonCopyable), Ego_r.pdb member names; the
// declaration order of the virtuals is not the slot order (leaf bodies only).
// CPSCDecalRenderer : CPSCBase (RTTI); DecalLifeSecs at +0x0C as in Ego_r.pdb (no container before it).
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
class CPSCDecalRenderer : public CPSCBase {
public:
    virtual bool GetDecalLifeSecs(float&);                   // slot 23
    float DecalLifeSecs;                                     // +0x0C
};
bool CPSCDecalRenderer::GetDecalLifeSecs(float& seconds)
{
    seconds = DecalLifeSecs;
    return true;
}
}
