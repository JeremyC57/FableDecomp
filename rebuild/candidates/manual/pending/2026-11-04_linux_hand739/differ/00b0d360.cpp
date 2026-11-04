// NParticleEngine::CPSCUpdateNormal::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info) -- vtable slot 17: persist the normal-update settings by name (the two parameter keys as CRCs when saving and as names hashed to CRCs when loading, then every flag, float, vector and the decal emitter name with its default)
// Grade: DIFFER(2242v2240) -- one 2-byte residue at 0x00B0DA8E: the bit-0 merge of RandomisePosDistVaryEnable; retail `mov cl,[esi+10h] ; xor cl,[esp+28h]` (the transferred bool as a memory operand), ours loads the bool into dl first. Function-scope / outer-block bools give retail's merge but move the bool or the next float default out of the dead `context` slot. Next idea: a shape where the bool outlives its store yet shares the slot with the RandomisePosDistVaryScale default.
// Leaf: ?DatabaseSerialise@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0D360, 2240 bytes, thiscall, `ret 8`.
// Identity: FableWin.pdb ?DatabaseSerialise@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z;
//   slot 17 of the CPSCUpdateNormal vtable 0x0129FC1C (slot 17 = DatabaseSerialise as the landed CPSCSpline 0x00B1B2E0 / CPSCAttractor 0x00B18110).
// Callees: CPersistContext::Transfer<T>(const char*, T&, const T&) for unsigned long 0x004106F0, CCharString 0x004109A0, bool 0x004045C0,
//   float 0x00410620, C3DVector 0x004859AD, long 0x00410BE0; CCharString() 0x0099E4B0, CCharString(const CCharString&) 0x0099EC30,
//   ~CCharString 0x0099EAE0; CCRC::Calc 0x004014A0 (the inline CCharString::GetCRC, FableWin ?GetCRC@CCharString@@QBEKXZ).
// Data: the 45 member-name literals 0x0129FFF8 .. 0x0129FCA0 (read from Fable.exe .rdata; FadeInEndInteger / FadeOutBeginInteger /
//   StayWithEmitter reuse the shared literals 0x0129F80C / 0x0129F7F8 / 0x0129FB40), empty string 0x0129AAF4.
// Defaults: 0 / false everywhere except ParticleAccelerationScale 1.0f and FadeOutBeginInteger 1000.
// Shape: the bit-fields go through `{ bool value = member; context.Transfer(name, value, false); member = value; }` blocks (the landed
//   CParticleEmitter::DatabaseSerialise 0x00AEF1F0 idiom), the 10-bit integers through long locals.
#include "engine/CPersistContext.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C3DVector.h"
class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
class CCharStringData { public:
    const char* PData;                                  // +0
    unsigned long StringLength;                         // +4
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                                  // 0x0099E4B0
    CCharString(const CCharString& other);                          // 0x0099EC30
    ~CCharString();                                                 // 0x0099EAE0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    unsigned long GetCRC() const { return CCRC::Calc(0, GetPString(), GetLength()); }
};
static inline CCharString EmptyCCharString()
{
    CCharString empty;
    return empty;
}
namespace NParticleEngine {
class CParticleSystem;
class CEmitterDatabaseSerialiseInfo;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&) = 0;
};
class CPSCUpdateNormal : public CPSCBase {
public:
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
    void* DecalEmitterList;                        // +0x9c
    virtual void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);
};
#define TRANSFER_BOOL_BIT(name, member) { bool value = member; context.Transfer(name, value, false); member = value; }
void CPSCUpdateNormal::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo&)
{
    if (context.IsSaving()) {
        context.Transfer("AccelerationParam", AccelerationParam, 0UL);
        context.Transfer("OrientFromGameParam", OrientFromGameParam, 0UL);
    } else {
        CCharString name;
        context.Transfer("AccelerationParam", name, EmptyCCharString());
        AccelerationParam = name.GetCRC();
        context.Transfer("OrientFromGameParam", name, EmptyCCharString());
        OrientFromGameParam = name.GetCRC();
    }
    TRANSFER_BOOL_BIT("UseParticleLifeSecs", UseParticleLifeSecs);
    context.Transfer("ParticleLifeSecs", ParticleLifeSecs, 0.0f);
    TRANSFER_BOOL_BIT("UseSystemLifeSecs", UseSystemLifeSecs);
    context.Transfer("SystemLifeSecs", SystemLifeSecs, 0.0f);
    context.Transfer("ParticleSystemOffset", ParticleSystemOffset, C3DVector(0.0f, 0.0f, 0.0f));
    context.Transfer("WindFactor", WindFactor, 0.0f);
    context.Transfer("GravityFactor", GravityFactor, 0.0f);
    context.Transfer("AirResistance", AirResistance, 0.0f);
    context.Transfer("ParticleAcceleration", ParticleAcceleration, C3DVector(0.0f, 0.0f, 0.0f));
    context.Transfer("ParticleAccelerationScale", ParticleAccelerationScale, 1.0f);
    TRANSFER_BOOL_BIT("UseAccelerationParam", UseAccelerationParam);
    TRANSFER_BOOL_BIT("UseAllAttractors", UseAllAttractors);
    TRANSFER_BOOL_BIT("UseRandomInitialRotation", UseRandomInitialRotation);
    context.Transfer("InitialRotationX", InitialRotationX, 0.0f);
    context.Transfer("InitialRotationY", InitialRotationY, 0.0f);
    context.Transfer("InitialRotationZ", InitialRotationZ, 0.0f);
    TRANSFER_BOOL_BIT("UseRandomRotationAxis", UseRandomRotationAxis);
    context.Transfer("RotationAxis", RotationAxis, C3DVector(0.0f, 0.0f, 0.0f));
    context.Transfer("RotationMinAngleSpeed", RotationMinAngleSpeed, 0.0f);
    context.Transfer("RotationMaxAngleSpeed", RotationMaxAngleSpeed, 0.0f);
    TRANSFER_BOOL_BIT("ParticleCollideWithGround", ParticleCollideWithGround);
    TRANSFER_BOOL_BIT("ParticleCollideWithAnything", ParticleCollideWithAnything);
    context.Transfer("ParticleBounce", ParticleBounce, 0.0f);
    TRANSFER_BOOL_BIT("ParticleDieOnCollision", ParticleDieOnCollision);
    { long value = FadeInEndInteger; context.Transfer("FadeInEndInteger", value, 0L); FadeInEndInteger = value; }
    { long value = FadeOutBeginInteger; context.Transfer("FadeOutBeginInteger", value, 1000L); FadeOutBeginInteger = value; }
    TRANSFER_BOOL_BIT("SystemAlphaFadeEnable", SystemAlphaFadeEnable);
    context.Transfer("SystemAlphaFadeMinimum", SystemAlphaFadeMinimum, 0.0f);
    TRANSFER_BOOL_BIT("EmissionFadeEnable", EmissionFadeEnable);
    context.Transfer("EmissionFadeMinimum", EmissionFadeMinimum, 0.0f);
    TRANSFER_BOOL_BIT("RandomisePosEnable", RandomisePosEnable);
    context.Transfer("RandomisePosSpeed", RandomisePosSpeed, C3DVector(0.0f, 0.0f, 0.0f));
    context.Transfer("RandomisePosScale", RandomisePosScale, C3DVector(0.0f, 0.0f, 0.0f));
    TRANSFER_BOOL_BIT("RandomisePosDistVaryEnable", RandomisePosDistVaryEnable);
    context.Transfer("RandomisePosDistVaryScale", RandomisePosDistVaryScale, 0.0f);
    TRANSFER_BOOL_BIT("StayWithEmitter", StayWithEmitter);
    TRANSFER_BOOL_BIT("SetOrientationFromDirection", SetOrientationFromDirection);
    TRANSFER_BOOL_BIT("SetOrientationFromGame", SetOrientationFromGame);
    TRANSFER_BOOL_BIT("ParticleCreateDecal", ParticleCreateDecal);
    TRANSFER_BOOL_BIT("ParticleCreateDecalEmitter", ParticleCreateDecalEmitter);
    context.Transfer("DecalEmitterName", DecalEmitterName, EmptyCCharString());
}
}
