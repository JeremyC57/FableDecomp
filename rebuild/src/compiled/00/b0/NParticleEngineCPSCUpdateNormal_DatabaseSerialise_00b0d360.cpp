// NParticleEngine::CPSCUpdateNormal::DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&) -- persist the normal particle update settings
// Grade: RELOCATION_MATCH (2240v2240, 108 relocation symbols equal to FINAL_OPERANDS.md) -- the visible CPersistContext::Transfer<T> body (as in the landed sibling 0x00B1A870) lets VC7.1 fold the bool temporary at 0x00B0DA91 while the 0.0f default keeps the context word +0x28 (0x00B0DAB0).
// Leaf: ?DatabaseSerialise@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Retail: 0x00B0D360, 2240 bytes, virtual thiscall.
// Opt-ins: the landed sibling CPSCLight::DatabaseSerialise (0x00B1A870) preamble, reused unchanged. FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
//   is that row's existing opt-in; here it is used only for what it also gates in CPersistContext.h / CPersistTraits.h (the
//   CPersistContext::Transfer<T> body and the C3DVector / bool / float trait declarations), not for the CPSCLight class.
//   The visible Transfer<T> body is what lets VC7.1 fold the bool temporary (`xor cl,[esp+28h]` at 0x00B0DA91); retail still
//   calls the instances (0x004045C0 bool, 0x00410620 float). Without it: DIFFER 2242v2240.
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BODY
#define FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CPSCUPDATENORMAL_DATABASE_SERIALISE
#include "fable_string.h"
#include "engine/CPersistContext.h"
#include "engine/C3DVector.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPSCUpdateNormal.h"

namespace NParticleEngine {
#define TRANSFER_BOOL_BIT(name, member) { bool temp = member; context.Transfer(name, temp); member = temp; }
void CPSCUpdateNormal::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info)
{
    if (context.IsSaving()) {
        context.Transfer("AccelerationParam", AccelerationParam);
        context.Transfer("OrientFromGameParam", OrientFromGameParam);
    } else {
        CCharString temp;
        context.Transfer("AccelerationParam", temp);
        AccelerationParam = temp.GetCRC();
        context.Transfer("OrientFromGameParam", temp);
        OrientFromGameParam = temp.GetCRC();
    }
    TRANSFER_BOOL_BIT("UseParticleLifeSecs", UseParticleLifeSecs);
    context.Transfer("ParticleLifeSecs", ParticleLifeSecs);
    TRANSFER_BOOL_BIT("UseSystemLifeSecs", UseSystemLifeSecs);
    context.Transfer("SystemLifeSecs", SystemLifeSecs);
    context.Transfer("ParticleSystemOffset", ParticleSystemOffset);
    context.Transfer("WindFactor", WindFactor);
    context.Transfer("GravityFactor", GravityFactor);
    context.Transfer("AirResistance", AirResistance);
    context.Transfer("ParticleAcceleration", ParticleAcceleration);
    context.Transfer("ParticleAccelerationScale", ParticleAccelerationScale, 1.0f);
    TRANSFER_BOOL_BIT("UseAccelerationParam", UseAccelerationParam);
    TRANSFER_BOOL_BIT("UseAllAttractors", UseAllAttractors);
    TRANSFER_BOOL_BIT("UseRandomInitialRotation", UseRandomInitialRotation);
    context.Transfer("InitialRotationX", InitialRotationX);
    context.Transfer("InitialRotationY", InitialRotationY);
    context.Transfer("InitialRotationZ", InitialRotationZ);
    TRANSFER_BOOL_BIT("UseRandomRotationAxis", UseRandomRotationAxis);
    context.Transfer("RotationAxis", RotationAxis);
    context.Transfer("RotationMinAngleSpeed", RotationMinAngleSpeed);
    context.Transfer("RotationMaxAngleSpeed", RotationMaxAngleSpeed);
    TRANSFER_BOOL_BIT("ParticleCollideWithGround", ParticleCollideWithGround);
    TRANSFER_BOOL_BIT("ParticleCollideWithAnything", ParticleCollideWithAnything);
    context.Transfer("ParticleBounce", ParticleBounce);
    TRANSFER_BOOL_BIT("ParticleDieOnCollision", ParticleDieOnCollision);
    { long temp = FadeInEndInteger; context.Transfer("FadeInEndInteger", temp); FadeInEndInteger = temp; }
    { long temp = FadeOutBeginInteger; context.Transfer("FadeOutBeginInteger", temp, 1000L); FadeOutBeginInteger = temp; }
    TRANSFER_BOOL_BIT("SystemAlphaFadeEnable", SystemAlphaFadeEnable);
    context.Transfer("SystemAlphaFadeMinimum", SystemAlphaFadeMinimum);
    TRANSFER_BOOL_BIT("EmissionFadeEnable", EmissionFadeEnable);
    context.Transfer("EmissionFadeMinimum", EmissionFadeMinimum);
    TRANSFER_BOOL_BIT("RandomisePosEnable", RandomisePosEnable);
    context.Transfer("RandomisePosSpeed", RandomisePosSpeed);
    context.Transfer("RandomisePosScale", RandomisePosScale);
    TRANSFER_BOOL_BIT("RandomisePosDistVaryEnable", RandomisePosDistVaryEnable);
    context.Transfer("RandomisePosDistVaryScale", RandomisePosDistVaryScale);
    TRANSFER_BOOL_BIT("StayWithEmitter", StayWithEmitter);
    TRANSFER_BOOL_BIT("SetOrientationFromDirection", SetOrientationFromDirection);
    TRANSFER_BOOL_BIT("SetOrientationFromGame", SetOrientationFromGame);
    TRANSFER_BOOL_BIT("ParticleCreateDecal", ParticleCreateDecal);
    TRANSFER_BOOL_BIT("ParticleCreateDecalEmitter", ParticleCreateDecalEmitter);
    context.Transfer("DecalEmitterName", DecalEmitterName);
}
}