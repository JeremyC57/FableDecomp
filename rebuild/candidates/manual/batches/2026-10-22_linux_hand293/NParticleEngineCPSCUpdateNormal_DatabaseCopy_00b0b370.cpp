// NParticleEngine::CPSCUpdateNormal::DatabaseCopy(const CPSCBase* source) -- refresh this update component from its database template: every parameter field and flag bit, in the editor's field order, then the decal emitter name
// Leaf: DatabaseCopy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0B370, length 592 bytes (`ret 4`); slot 18 of vtable 0x0129FC1C.
// Callees: CCharString::operator= @ 0x0099EFB0 (DecalEmitterName, a sibling call).
// Layout: Ego_r.pdb CPSCUpdateNormal (0xa8) matches retail here: the ulong bit-field word +0x0c (FadeInEndInteger:10,
//   FadeOutBeginInteger:10, then twelve 1-bit flags), the bool bit-field byte +0x10, DecalEmitterName +0x14, the floats
//   +0x18..+0x50, five C3DVectors +0x54..+0x84, AccelerationParam +0x90, OrientFromGameParam +0x94. Local model (no header).
// Shape: one assignment per field in the template's transfer order (not declaration order); the source is the base
//   pointer cast down; bit-field copies are VC7.1's read-xor-mask-xor merges.
struct CCharStringData;
struct CCharString {
    CCharStringData* m_pData;
    CCharString& operator=(const CCharString& other);                  // 0x0099EFB0
};
struct C3DVector {
    float X, Y, Z;
};
namespace NParticleEngine {
struct CPSCBase {
    virtual void v00();
    unsigned char _base04[8];                                          // +0x04
};
struct CPSCUpdateNormal : CPSCBase {
    unsigned long FadeInEndInteger : 10;                               // +0x0c
    unsigned long FadeOutBeginInteger : 10;
    unsigned long UseParticleLifeSecs : 1;
    unsigned long UseRandomRotationAxis : 1;
    unsigned long UseRandomInitialRotation : 1;
    unsigned long UseAccelerationParam : 1;
    unsigned long StayWithEmitter : 1;
    unsigned long UseSystemLifeSecs : 1;
    unsigned long SystemAlphaFadeEnable : 1;
    unsigned long EmissionFadeEnable : 1;
    unsigned long ParticleCollideWithGround : 1;
    unsigned long ParticleDieOnCollision : 1;
    unsigned long UseAllAttractors : 1;
    unsigned long RandomisePosEnable : 1;
    bool RandomisePosDistVaryEnable : 1;                               // +0x10
    bool SetOrientationFromDirection : 1;
    bool SetOrientationFromGame : 1;
    bool ParticleCreateDecal : 1;
    bool ParticleCreateDecalEmitter : 1;
    bool ParticleCollideWithAnything : 1;
    CCharString DecalEmitterName;                                      // +0x14
    float SystemLifeSecs;                                              // +0x18
    float ParticleLifeSecs;                                            // +0x1c
    float WindFactor;                                                  // +0x20
    float GravityFactor;                                               // +0x24
    float AirResistance;                                               // +0x28
    float ParticleAccelerationScale;                                   // +0x2c
    float InitialRotationX;                                            // +0x30
    float InitialRotationY;                                            // +0x34
    float InitialRotationZ;                                            // +0x38
    float RotationMinAngleSpeed;                                       // +0x3c
    float RotationMaxAngleSpeed;                                       // +0x40
    float ParticleBounce;                                              // +0x44
    float SystemAlphaFadeMinimum;                                      // +0x48
    float RandomisePosDistVaryScale;                                   // +0x4c
    float EmissionFadeMinimum;                                         // +0x50
    C3DVector ParticleSystemOffset;                                    // +0x54
    C3DVector ParticleAcceleration;                                    // +0x60
    C3DVector RotationAxis;                                            // +0x6c
    C3DVector RandomisePosSpeed;                                       // +0x78
    C3DVector RandomisePosScale;                                       // +0x84
    unsigned long AccelerationParam;                                   // +0x90
    unsigned long OrientFromGameParam;                                 // +0x94
    long LifeTimer;                                                    // +0x98
    void* DecalEmitterList;                                            // +0x9c STLport list (head node)
    void DatabaseCopy(const CPSCBase* base);
};

void CPSCUpdateNormal::DatabaseCopy(const CPSCBase* base)
{
    const CPSCUpdateNormal* source = (const CPSCUpdateNormal*)base;
    UseParticleLifeSecs = source->UseParticleLifeSecs;
    ParticleLifeSecs = source->ParticleLifeSecs;
    UseSystemLifeSecs = source->UseSystemLifeSecs;
    SystemLifeSecs = source->SystemLifeSecs;
    ParticleSystemOffset = source->ParticleSystemOffset;
    WindFactor = source->WindFactor;
    GravityFactor = source->GravityFactor;
    AirResistance = source->AirResistance;
    ParticleAcceleration = source->ParticleAcceleration;
    ParticleAccelerationScale = source->ParticleAccelerationScale;
    UseAccelerationParam = source->UseAccelerationParam;
    AccelerationParam = source->AccelerationParam;
    UseAllAttractors = source->UseAllAttractors;
    UseRandomInitialRotation = source->UseRandomInitialRotation;
    InitialRotationX = source->InitialRotationX;
    InitialRotationY = source->InitialRotationY;
    InitialRotationZ = source->InitialRotationZ;
    UseRandomRotationAxis = source->UseRandomRotationAxis;
    RotationAxis = source->RotationAxis;
    RotationMinAngleSpeed = source->RotationMinAngleSpeed;
    RotationMaxAngleSpeed = source->RotationMaxAngleSpeed;
    ParticleCollideWithGround = source->ParticleCollideWithGround;
    ParticleCollideWithAnything = source->ParticleCollideWithAnything;
    ParticleBounce = source->ParticleBounce;
    ParticleDieOnCollision = source->ParticleDieOnCollision;
    FadeInEndInteger = source->FadeInEndInteger;
    FadeOutBeginInteger = source->FadeOutBeginInteger;
    SystemAlphaFadeEnable = source->SystemAlphaFadeEnable;
    SystemAlphaFadeMinimum = source->SystemAlphaFadeMinimum;
    EmissionFadeEnable = source->EmissionFadeEnable;
    EmissionFadeMinimum = source->EmissionFadeMinimum;
    RandomisePosEnable = source->RandomisePosEnable;
    RandomisePosSpeed = source->RandomisePosSpeed;
    RandomisePosScale = source->RandomisePosScale;
    RandomisePosDistVaryEnable = source->RandomisePosDistVaryEnable;
    RandomisePosDistVaryScale = source->RandomisePosDistVaryScale;
    StayWithEmitter = source->StayWithEmitter;
    SetOrientationFromDirection = source->SetOrientationFromDirection;
    SetOrientationFromGame = source->SetOrientationFromGame;
    OrientFromGameParam = source->OrientFromGameParam;
    ParticleCreateDecal = source->ParticleCreateDecal;
    ParticleCreateDecalEmitter = source->ParticleCreateDecalEmitter;
    DecalEmitterName = source->DecalEmitterName;
}
}
