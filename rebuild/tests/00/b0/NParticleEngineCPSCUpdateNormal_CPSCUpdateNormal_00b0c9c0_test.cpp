// Behaviour test for NParticleEngine::CPSCUpdateNormal::CPSCUpdateNormal (0x00B0C9C0): flag word, defaults and the
// self-linked list sentinel, built over garbage-filled storage.
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <new>
struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable() {}
    CBaseClassNonCopyable() {}
};

struct CCharString {
    void* m_pData;
    CCharString() : m_pData(0) {}
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

namespace NParticleEngine {
struct CParticleSystem;

struct CPSCBase : CBaseClassNonCopyable {          // vtable 0x0129F3F4
    CParticleSystem* ParticleSystem;               // +0x04
    bool             UpdateStarted : 1;            // +0x08 bit 0
    bool             Initialised : 1;              // +0x08 bit 1
    CPSCBase() : ParticleSystem(0) { UpdateStarted = false; Initialised = false; }
};

struct SEmitterListNode { SEmitterListNode* m_pNext; SEmitterListNode* m_pPrev; unsigned char _value[8]; };  // 16 bytes
struct CEmitterList {                              // list<CIntelligentPointer<CParticleEmitter>> (retail: sentinel only)
    SEmitterListNode* m_pHead;
    CEmitterList()
    {
        m_pHead = 0;
        SEmitterListNode* node = (SEmitterListNode*)malloc(0x10);
        node->m_pNext = node;
        node->m_pPrev = node;
        m_pHead = node;
    }
};

struct CPSCUpdateNormal : CPSCBase {               // vtable 0x0129FC1C
    unsigned long FadeInEndInteger : 10;           // +0x0c
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
    CEmitterList DecalEmitterList;                 // +0x9c
    CPSCUpdateNormal();
};

CPSCUpdateNormal::CPSCUpdateNormal()
    : FadeInEndInteger(0)
    , FadeOutBeginInteger(1000)
    , UseParticleLifeSecs(1)
    , UseRandomRotationAxis(0)
    , UseRandomInitialRotation(0)
    , UseAccelerationParam(0)
    , StayWithEmitter(0)
    , UseSystemLifeSecs(0)
    , SystemAlphaFadeEnable(1)
    , EmissionFadeEnable(0)
    , ParticleCollideWithGround(0)
    , ParticleDieOnCollision(0)
    , UseAllAttractors(1)
    , RandomisePosEnable(0)
    , RandomisePosDistVaryEnable(false)
    , SetOrientationFromDirection(false)
    , SetOrientationFromGame(false)
    , ParticleCreateDecal(false)
    , ParticleCreateDecalEmitter(false)
    , ParticleCollideWithAnything(false)
    , SystemLifeSecs(60.0f)
    , ParticleLifeSecs(5.0f)
    , WindFactor(0.0f)
    , GravityFactor(0.0f)
    , AirResistance(0.0f)
    , ParticleAccelerationScale(1.0f)
    , InitialRotationX(0.0f)
    , InitialRotationY(0.0f)
    , InitialRotationZ(0.0f)
    , RotationMinAngleSpeed(0.0f)
    , RotationMaxAngleSpeed(0.0f)
    , ParticleBounce(0.5f)
    , SystemAlphaFadeMinimum(0.0f)
    , RandomisePosDistVaryScale(1.0f)
    , EmissionFadeMinimum(0.0f)
    , ParticleSystemOffset(0.0f, 0.0f, 0.0f)
    , ParticleAcceleration(0.0f, 0.0f, 0.0f)
    , RotationAxis(0.0f, 0.0f, 1.0f)
    , RandomisePosSpeed(0.0f, 0.0f, 0.0f)
    , RandomisePosScale(0.0f, 0.0f, 0.0f)
    , LifeTimer(0)
{
}
}

int main()
{
    using namespace NParticleEngine;
    void* mem = malloc(sizeof(CPSCUpdateNormal)); memset(mem, 0xcd, sizeof(CPSCUpdateNormal));
    CPSCUpdateNormal* p = new (mem) CPSCUpdateNormal;
    int bad = 0;
    if (p->FadeInEndInteger != 0 || p->FadeOutBeginInteger != 1000 || !p->UseParticleLifeSecs || p->UseRandomRotationAxis) bad = 1;
    if (!p->SystemAlphaFadeEnable || !p->UseAllAttractors || p->RandomisePosEnable || p->EmissionFadeEnable) bad = 2;
    if (p->RandomisePosDistVaryEnable || p->ParticleCreateDecal || p->ParticleCollideWithAnything) bad = 3;
    if (p->SystemLifeSecs != 60.0f || p->ParticleLifeSecs != 5.0f || p->ParticleAccelerationScale != 1.0f) bad = 4;
    if (p->ParticleBounce != 0.5f || p->RandomisePosDistVaryScale != 1.0f || p->WindFactor != 0.0f) bad = 5;
    if (p->RotationAxis.Z != 1.0f || p->RotationAxis.X != 0.0f || p->RandomisePosScale.Y != 0.0f || p->LifeTimer != 0) bad = 6;
    SEmitterListNode* n = p->DecalEmitterList.m_pHead;
    if (!n || n->m_pNext != n || n->m_pPrev != n) bad = 7;
    if (bad) { std::printf("00b0c9c0_TEST FAIL\n"); return 1; }
    std::printf("00b0c9c0_TEST PASS\n");
    return 0;
}