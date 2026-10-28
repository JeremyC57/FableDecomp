// Behaviour test for CPSCUpdateNormal::DatabaseCopy (0x00B0B370): every copied field and flag takes the source's
// value while the fields DatabaseCopy skips (LifeTimer, the decal list, unrelated bits) keep their own.
#include <cstdio>
#include <cstring>
class CCharString { public:
    int m_id;
    const CCharString& operator=(const CCharString& other) { m_id = other.m_id; return *this; }
};
class C3DVector { public:
    float X, Y, Z;
};
struct CPSCBase { int m_dummy; };
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

int main()
{
    static CPSCUpdateNormal a, b;
    std::memset(&a, 0, sizeof(a));
    std::memset(&b, 0, sizeof(b));
    b.FadeInEndInteger = 513; b.FadeOutBeginInteger = 1000; b.UseParticleLifeSecs = 1; b.UseRandomRotationAxis = 1;
    b.UseRandomInitialRotation = 1; b.UseAccelerationParam = 1; b.StayWithEmitter = 1; b.UseSystemLifeSecs = 1;
    b.SystemAlphaFadeEnable = 1; b.EmissionFadeEnable = 1; b.ParticleCollideWithGround = 1; b.ParticleDieOnCollision = 1;
    b.UseAllAttractors = 1; b.RandomisePosEnable = 1; b.RandomisePosDistVaryEnable = true; b.SetOrientationFromDirection = true;
    b.SetOrientationFromGame = true; b.ParticleCreateDecal = true; b.ParticleCreateDecalEmitter = true; b.ParticleCollideWithAnything = true;
    b.DecalEmitterName.m_id = 42; b.SystemLifeSecs = 1.0f; b.ParticleLifeSecs = 2.0f; b.WindFactor = 3.0f; b.GravityFactor = 4.0f;
    b.AirResistance = 5.0f; b.ParticleAccelerationScale = 6.0f; b.InitialRotationX = 7.0f; b.InitialRotationY = 8.0f;
    b.InitialRotationZ = 9.0f; b.RotationMinAngleSpeed = 10.0f; b.RotationMaxAngleSpeed = 11.0f; b.ParticleBounce = 12.0f;
    b.SystemAlphaFadeMinimum = 13.0f; b.RandomisePosDistVaryScale = 14.0f; b.EmissionFadeMinimum = 15.0f;
    b.ParticleSystemOffset.X = 16.0f; b.ParticleAcceleration.Y = 17.0f; b.RotationAxis.Z = 18.0f; b.RandomisePosSpeed.X = 19.0f;
    b.RandomisePosScale.Y = 20.0f; b.AccelerationParam = 21; b.OrientFromGameParam = 22; b.LifeTimer = 23;
    a.LifeTimer = 99;
    a.DatabaseCopy(&b);
    int bad = 0;
    if (a.FadeInEndInteger != 513 || a.FadeOutBeginInteger != 1000) bad = 1;
    if (!a.UseParticleLifeSecs || !a.UseRandomRotationAxis || !a.UseRandomInitialRotation || !a.UseAccelerationParam) bad = 2;
    if (!a.StayWithEmitter || !a.UseSystemLifeSecs || !a.SystemAlphaFadeEnable || !a.EmissionFadeEnable) bad = 3;
    if (!a.ParticleCollideWithGround || !a.ParticleDieOnCollision || !a.UseAllAttractors || !a.RandomisePosEnable) bad = 4;
    if (!a.RandomisePosDistVaryEnable || !a.SetOrientationFromDirection || !a.SetOrientationFromGame) bad = 5;
    if (!a.ParticleCreateDecal || !a.ParticleCreateDecalEmitter || !a.ParticleCollideWithAnything) bad = 6;
    if (a.DecalEmitterName.m_id != 42 || a.SystemLifeSecs != 1.0f || a.EmissionFadeMinimum != 15.0f) bad = 7;
    if (a.ParticleSystemOffset.X != 16.0f || a.ParticleAcceleration.Y != 17.0f || a.RotationAxis.Z != 18.0f) bad = 8;
    if (a.RandomisePosSpeed.X != 19.0f || a.RandomisePosScale.Y != 20.0f || a.AccelerationParam != 21 || a.OrientFromGameParam != 22) bad = 9;
    if (a.LifeTimer != 99) bad = 10;
    b.UseAllAttractors = 0; b.FadeInEndInteger = 3;
    a.DatabaseCopy(&b);
    if (a.UseAllAttractors || a.FadeInEndInteger != 3 || a.FadeOutBeginInteger != 1000) bad = 11;
    if (bad) { std::printf("00b0b370_TEST FAIL\n"); return 1; }
    std::printf("00b0b370_TEST PASS\n");
    return 0;
}