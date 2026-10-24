// NParticleEngine::CPSCUpdateNormal::GetUpdateParam(C3DVector& value, unsigned long paramType, float time, bool initial, const CParticleUpdateParameters& params, unsigned long index) -- vtable slot 1: the only parameter this component supplies is type 0, the particle-system offset (zero for the initial query)
// Leaf: ?GetUpdateParam@CPSCUpdateNormal@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0A6D0, 62 bytes (+2 int3 padding).
// Name: FableWin.pdb ?GetUpdateParam@CPSCUpdateNormal@NParticleEngine@@UAE_NAAVC3DVector@@KM_NABV?$CParticleUpdateParameters@KK@2@K@Z;
// slot 1 of the vtable 0x0129FC1C (rtti-vtables.tsv; slot 1 of CPSCSpline is the landed GetUpdateParam 0x00B1B030).
// Callees: none.
// Layout: the landed constructor 0x00B0C9C0 (retail offsets): ParticleSystemOffset (C3DVector) +0x54.
// Shape: three exits: any other parameter type returns false; the initial query zeroes the vector with a
// chain assignment (`Z = Y = X = 0`: stores +8, +4, +0 from one zero register); otherwise the offset is
// copied through C3DVector's memberwise copy (`add ecx,54h` then dword moves). `ret 18h`.
namespace NParticleEngine { template <class A, class B> struct CParticleUpdateParameters; }
struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable();
};

struct CCharString {
    void* m_pData;
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};

namespace NParticleEngine {
struct CParticleSystem;

struct CPSCBase : CBaseClassNonCopyable {          // vtable 0x0129F3F4
    CParticleSystem* ParticleSystem;               // +0x04
    bool             UpdateStarted : 1;            // +0x08 bit 0
    bool             Initialised : 1;              // +0x08 bit 1
};

struct CPSCUpdateNormal : CPSCBase {               // vtable 0x0129FC1C
    unsigned long FadeInEndInteger : 10;           // +0x0c
    unsigned long FadeOutBeginInteger : 10;
    unsigned long UseParticleLifeSecs : 1;
    unsigned long UseRandomRotationAxis : 1;
    unsigned long UseRandomInitialRotation : 1;
    unsigned long UseAccelerationParam : 1;
    unsigned long StayWithEmitter : 1;             // bit 24
    unsigned long UseSystemLifeSecs : 1;
    unsigned long SystemAlphaFadeEnable : 1;
    unsigned long EmissionFadeEnable : 1;
    unsigned long ParticleCollideWithGround : 1;
    unsigned long ParticleDieOnCollision : 1;
    unsigned long UseAllAttractors : 1;            // bit 30
    unsigned long RandomisePosEnable : 1;
    bool RandomisePosDistVaryEnable : 1;           // +0x10
    bool SetOrientationFromDirection : 1;
    bool SetOrientationFromGame : 1;
    bool ParticleCreateDecal : 1;
    bool ParticleCreateDecalEmitter : 1;
    bool ParticleCollideWithAnything : 1;
    CCharString DecalEmitterName;                  // +0x14
    float SystemLifeSecs;                          // +0x18
    float ParticleLifeSecs;                        // +0x1c
    float WindFactor;                              // +0x20
    float GravityFactor;                           // +0x24
    float AirResistance;                           // +0x28
    float ParticleAccelerationScale;               // +0x2c
    float InitialRotationX;                        // +0x30
    float InitialRotationY;                        // +0x34
    float InitialRotationZ;                        // +0x38
    float RotationMinAngleSpeed;                   // +0x3c
    float RotationMaxAngleSpeed;                   // +0x40
    float ParticleBounce;                          // +0x44
    float SystemAlphaFadeMinimum;                  // +0x48
    float RandomisePosDistVaryScale;               // +0x4c
    float EmissionFadeMinimum;                     // +0x50
    C3DVector ParticleSystemOffset;                // +0x54
    C3DVector ParticleAcceleration;                // +0x60
    C3DVector RotationAxis;                        // +0x6c
    C3DVector RandomisePosSpeed;                   // +0x78
    C3DVector RandomisePosScale;                   // +0x84
    unsigned long AccelerationParam;               // +0x90
    unsigned long OrientFromGameParam;             // +0x94
    long LifeTimer;                                // +0x98
    void* DecalEmitterList;                        // +0x9c (STLport list sentinel)
    virtual bool GetUpdateParam(C3DVector& value, unsigned long paramType, float time, bool initial,
                                const CParticleUpdateParameters<unsigned long, unsigned long>& params, unsigned long index);
};

bool CPSCUpdateNormal::GetUpdateParam(C3DVector& value, unsigned long paramType, float time, bool initial,
                                      const CParticleUpdateParameters<unsigned long, unsigned long>& params, unsigned long index)
{
    if (paramType != 0)
        return false;
    if (initial) {
        value.X = value.Y = value.Z = 0.0f;
        return true;
    }
    value = ParticleSystemOffset;
    return true;
}

} // namespace NParticleEngine