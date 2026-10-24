// NParticleEngine::CPSCUpdateNormal::Initialise(CIEngine* engine, CParticleSystem& system, const CParticleUpdateParameters& params, CEnginePrimitiveHandle& handle) -- vtable slot 28: hand the attractor and stay-with-emitter settings to the owning particle system and mark the component initialised
// Leaf: ?Initialise@CPSCUpdateNormal@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0A710, 60 bytes (+4 int3 padding).
// Name: FableWin.pdb ?Initialise@CPSCUpdateNormal@NParticleEngine@@UAE_NPAVCIEngine@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z;
// slot 28 of the vtable 0x0129FC1C (rtti-vtables.tsv; slot 28 of CPSCSpline is the landed CPSCSplineBase::Initialise
// 0x00B215A0; the landed CParticleSystem::Initialise 0x00AF4010 calls it per component).
// Callees: none.
// Layout: the landed constructor 0x00B0C9C0: the +0x0C flag word (StayWithEmitter bit 24, UseAllAttractors bit 30);
// CParticleSystem (retail, as 0x00AF4010) flag byte +0x50: UseAllAttractors bit 4, StayWithEmitter bit 6;
// CPSCBase::Initialised is bit 1 of +8.
// Shape: two bit-field copies into the system's flag byte (the second merged into the first's register),
// then `Initialised = true` and the flag re-read for the result (`or al,2 ; shr al,1 ; and al,1`). `ret 10h`.
struct CIEngine;
struct CEnginePrimitiveHandle;
namespace NParticleEngine {
template <class A, class B> struct CParticleUpdateParameters;
struct CParticleSystem {
    unsigned char _pad00[0x50];
    bool Initialised : 1;                          // +0x50 bit 0
    bool Enabled : 1;
    bool Dead : 1;
    bool NormalisedTimeEnabled : 1;
    bool UseAllAttractors : 1;                     // bit 4
    bool Visible : 1;
    bool StayWithEmitter : 1;                      // bit 6
    bool ScaleParticles : 1;
};
}
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
    virtual bool Initialise(CIEngine* engine, CParticleSystem& system,
                            const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle);
};

bool CPSCUpdateNormal::Initialise(CIEngine* engine, CParticleSystem& system,
                                  const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& handle)
{
    system.UseAllAttractors = UseAllAttractors;
    system.StayWithEmitter = StayWithEmitter;
    Initialised = true;
    return Initialised;
}

} // namespace NParticleEngine