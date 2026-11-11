// NParticleEngine::CPSCSingleSprite::EnterTimelineEnd() -- the sprite is entering the end of its timeline (EnteringTimelineEnd, retail +0x97)
// Leaf: ?EnterTimelineEnd@CPSCSingleSprite@NParticleEngine@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0A130, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 25 of NParticleEngine::CPSCSingleSprite's vtable 0x0129FB84 (the slot holds this body in 1 vtable(s), owner NParticleEngine::CPSCSingleSprite); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts EnterTimelineEnd(void) at this slot [gap kind GSC]; Ego_r/FableWin name set: ?EnterTimelineEnd@CPSCSingleSprite@NParticleEngine@@UAEXXZ; `ret` matches the signature.
// Was: _unattributed::vfunc_25 (rebuild/src/compiled/00/b0/unattributed_vfunc25_00b0a130.cpp).
// No shared header for NParticleEngine::CParticleUpdateParamCallback / CPSCBase: minimal local definitions on the RTTI bases
// (CPSCBase : CParticleUpdateParamCallback<unsigned long,unsigned long> : CBaseClassNonCopyable), Ego_r.pdb member names; the
// declaration order of the virtuals is not the slot order (leaf bodies only).
// CPSCSingleSprite : CPSCSplineBase : CPSCBase (RTTI). Ego_r.pdb: CPSCSplineBase 0x38 bytes (SplineAnimSpeed +0x0C, CCardinalSpline
// Spline +0x10, two CCountedPointers), CPSCSingleSprite's EnteringTimelineEnd at Ego_r +0x9B; retail is 4 lower (the spline's STLport
// vector is 12 bytes, not 16), so the members between are opaque here.
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
class CPSCSplineBase : public CPSCBase {
public:
    unsigned char _members_0x0c[0x28];                       // +0x0C SplineAnimSpeed .. SplinePosParamData (retail 0x34 bytes)
};
class CPSCSingleSprite : public CPSCSplineBase {
public:
    virtual void EnterTimelineEnd();                         // slot 25
    unsigned char _members_0x34[0x63];                       // +0x34 Ego_r CPSCSingleSprite members +0x38 .. +0x9A
    bool EnteringTimelineEnd;                                // +0x97
};
void CPSCSingleSprite::EnterTimelineEnd()
{
    EnteringTimelineEnd = true;
}
}
