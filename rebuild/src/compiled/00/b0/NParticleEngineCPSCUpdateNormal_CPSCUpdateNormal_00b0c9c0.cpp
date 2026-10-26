// NParticleEngine::CPSCUpdateNormal::CPSCUpdateNormal() -- the particle "normal update" component: CPSCBase base, packed update flags (fade-out at 1000, particle life / system alpha fade / all attractors on), decal emitter name, the default lifetimes, unit scales, a Z rotation axis and the empty decal-emitter list
// Leaf: ??0CPSCUpdateNormal
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0C9C0, 195 bytes. The catalog label "CHeroStatsDef" is propagated: the body stores
//   vtable 0x0129FC1C, which rtti-vtables.tsv names NParticleEngine::CPSCUpdateNormal, over the CPSCBase ctor.
// Callees: NParticleEngine::CPSCBase::CPSCBase @ 0x00AF2620, CCharString::CCharString() @ 0x0099E4B0, malloc @ 0x00BFEA0E
//   (the list sentinel).
// Layout (Ego_r.pdb CPSCUpdateNormal, 168 bytes; retail 0xA0 because the list is the 4-byte sentinel pointer):
//   +0x0c FadeInEndInteger:10 / FadeOutBeginInteger:10 / twelve 1-bit flags (one folded dword store 0x441FA000),
//   +0x10 six bool:1 flags (cleared with `and al,0C0h`), +0x14 DecalEmitterName, +0x18..+0x50 floats,
//   +0x54..+0x8c five C3DVectors, +0x90/+0x94 parameters (left unset), +0x98 LifeTimer, +0x9c DecalEmitterList.
// Shape: every member is an initialiser; VC folds the whole +0x0c word into one immediate, the +0x10 bits into one
//   read-modify-write, shares one zero register and one 1.0f register, and builds the list sentinel last
//   (`head = 0 ; node = malloc(16) ; node->next = node->prev = node ; head = node`).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                       // 0x0099A2F0
};

class CCharString { public:
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

namespace NParticleEngine {
class CParticleSystem;

class CPSCBase : public CBaseClassNonCopyable { public:          // vtable 0x0129F3F4
    CParticleSystem* ParticleSystem;               // +0x04
    bool             UpdateStarted : 1;            // +0x08 bit 0
    bool             Initialised : 1;              // +0x08 bit 1
    CPSCBase();                                    // 0x00AF2620
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

class CPSCUpdateNormal : public CPSCBase { public:               // vtable 0x0129FC1C
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