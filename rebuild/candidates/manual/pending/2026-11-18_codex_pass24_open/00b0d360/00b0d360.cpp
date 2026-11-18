// NParticleEngine::CPSCUpdateNormal::DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&) -- persist the normal particle update settings
// Grade: DIFFER(2240v2240) -- At 0x00B0DAA0 and 0x00B0DAB0 the following float default occupies +0x14 instead of the retired context argument word +0x28.
// Leaf: ?DatabaseSerialise@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Retail: 0x00B0D360, 2240 bytes, virtual thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CPSCUPDATENORMAL_DATABASE_SERIALISE
#include "fable_string.h"
#include "engine/CPSCUpdateNormal.h"
#include "engine/CPersistContext.h"

namespace {
inline CCharString EmptyCCharString()
{
    CCharString empty;
    return empty;
}
}

namespace NParticleEngine {
#define TRANSFER_BOOL_BIT(name, member) { bool temp = member; context.Transfer(name, temp, false); member = temp; }
void CPSCUpdateNormal::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info)
{
    if (context.IsSaving()) {
        context.Transfer("AccelerationParam", AccelerationParam, 0UL);
        context.Transfer("OrientFromGameParam", OrientFromGameParam, 0UL);
    } else {
        CCharString temp;
        context.Transfer("AccelerationParam", temp, EmptyCCharString());
        AccelerationParam = temp.GetCRC();
        context.Transfer("OrientFromGameParam", temp, EmptyCCharString());
        OrientFromGameParam = temp.GetCRC();
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
    { long temp = FadeInEndInteger; context.Transfer("FadeInEndInteger", temp, 0L); FadeInEndInteger = temp; }
    { long temp = FadeOutBeginInteger; context.Transfer("FadeOutBeginInteger", temp, 1000L); FadeOutBeginInteger = temp; }
    TRANSFER_BOOL_BIT("SystemAlphaFadeEnable", SystemAlphaFadeEnable);
    context.Transfer("SystemAlphaFadeMinimum", SystemAlphaFadeMinimum, 0.0f);
    TRANSFER_BOOL_BIT("EmissionFadeEnable", EmissionFadeEnable);
    context.Transfer("EmissionFadeMinimum", EmissionFadeMinimum, 0.0f);
    TRANSFER_BOOL_BIT("RandomisePosEnable", RandomisePosEnable);
    context.Transfer("RandomisePosSpeed", RandomisePosSpeed, C3DVector(0.0f, 0.0f, 0.0f));
    context.Transfer("RandomisePosScale", RandomisePosScale, C3DVector(0.0f, 0.0f, 0.0f));
    {
        bool temp = RandomisePosDistVaryEnable;
        context.Transfer("RandomisePosDistVaryEnable", temp, false);
        RandomisePosDistVaryEnable = temp;
        const float defaultScale = 0.0f;
        context.Transfer("RandomisePosDistVaryScale", RandomisePosDistVaryScale, defaultScale);
    }
    TRANSFER_BOOL_BIT("StayWithEmitter", StayWithEmitter);
    TRANSFER_BOOL_BIT("SetOrientationFromDirection", SetOrientationFromDirection);
    TRANSFER_BOOL_BIT("SetOrientationFromGame", SetOrientationFromGame);
    TRANSFER_BOOL_BIT("ParticleCreateDecal", ParticleCreateDecal);
    TRANSFER_BOOL_BIT("ParticleCreateDecalEmitter", ParticleCreateDecalEmitter);
    context.Transfer("DecalEmitterName", DecalEmitterName, EmptyCCharString());
}
}
