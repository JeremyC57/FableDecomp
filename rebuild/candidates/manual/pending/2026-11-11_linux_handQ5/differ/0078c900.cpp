// CTCBerserk::InitAbility() -- start the berserk: start the radial blur, register the def's speed multiplier with the creature's haste component (adding one if needed), spawn the strength intro and berserk effects, attach the body effect, and play the cast sound and the looping sound for the spell level
// Grade: DIFFER(1132v1152) -- Q5: 6 grades logged (3 hypotheses + baseline, a full-diff regrade and the final regrade; 10 in all), unchanged. Residue: the conditional-temporary flag word for the three `GetSubDef(&d) && d->GetEffectFromString("...", effect)` conditions. Retail keeps it in memory (frame +0, zeroed in the prologue at 0x0078C90A, `mov dword ptr [esp+18h],1` at 0x0078CA0B) with the && results in bl (`mov bl,1 / xor bl,bl`). Ours gives the flag word ebx and the results a frame byte, so the frame is 0x20 v 0x18 and every case-label temporary moves. c2prio --blocks: flag word prio 11 (+20 of it from its prologue init in block 1, K=10) against the && bools 10/9/9; a 2-point swing would flip it. Inert: one reused `bool found`, `effect` declared before effectsDef; chaining the interpolator call made it worse (ratio 0.626). Next: find which block-1 candidate (K=10, including a const -1 that emits no code) the retail source lacks, or a spelling that raises the bools' blocks' K.
// Leaf: ?InitAbility@CTCBerserk@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078C900, 1,152 bytes with its 4-entry jump table 0x0078CD70, thiscall, plain `ret`; Ego_r.pdb /
//   FableWin.pdb ?InitAbility@CTCBerserk@@QAEXXZ.
// Callees: CTCBase::GetWorld 0x00686A70, CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo 0x006B8730, CVectorMap::LowerBound
//   0x0040F020 (the inline GetTC), CThing::AddTC 0x004C9D60, the CTCHaste multiplier map insert 0x00580D21,
//   CTCHaste::UpdateActionSpeedMultiplier 0x00800020, CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580,
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30, CThing::GetPos 0x004C73D0, CTCSpecialAbilities::CreateParticleEffect
//   0x0072AB70, CIntelligentPointer (0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / ~ 0x00A01C10),
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0, CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00, CCharString(const char*, long)
//   0x0099EBF0 / operator= 0x0099EFB0 / ~ 0x0099EAE0.
// Header overlay (FABLE_HDR_OVERLAY=$A/hdr): engine/CTCBerserk.h FABLE_RETAIL_CTCBERSERK_INITABILITY, engine/CEnvironmentSpellEffectInterpolator.h
//   FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_RADIALBLURFLOATS.
#define FABLE_RETAIL_CTCBERSERK_HIT_EFFECT
#define FABLE_RETAIL_CTCBERSERK_INITABILITY
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CTCHASTE
#define FABLE_RETAIL_CTCHASTE_MAKE_PAIR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_EXTRATAG
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_FULL
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_RADIALBLURFLOATS
#include "engine/C3DVector.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/ThingAlive.h"
#include "engine/CSpecialAbilitiesBerserkDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCBerserk.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCHaste.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"

void CTCBerserk::InitAbility()
{
    AbilityInitialised = true;
    CEnvironmentSpellEffectInterpolator& interpolator = GetWorld().GetEnvironment().GetSpellEffectInterpolator();
    RadialBlurFadeKey = interpolator.RadialBlurFadeTo(
        0.0f, NScreenEffect::SCREEN_EFFECT_PRIORITY_SPELL, PDef->BerserkThreshold, PDef->BerserkLength, PDef->BerserkIntensity,
        PDef->BerserkFade, PDef->BerserkRange, PDef->BerserkTwist);

    // the berserk speeds the creature up through its haste component
    CTCHaste* haste;
    if (!ParentThing->GetTC(&haste))
        haste = (CTCHaste*)ParentThing->AddTC("CTCHaste", true, 0);
    haste->SetActionSpeedMultiplier(this, PDef->SpeedMultiplier);

    CDefPointer<const CSpecialEffectsDef> effectsDef;
    long effect = 0;
    if (ParentThing->PeekBaseDef().GetSubDef(&effectsDef) && effectsDef->GetEffectFromString("SKILL_STRENGTH_INTRO", effect))
        CTCSpecialAbilities::CreateParticleEffect(effect, ParentThing->GetPos());
    if (ParentThing->PeekBaseDef().GetSubDef(&effectsDef) && effectsDef->GetEffectFromString("BERSERK", effect))
        CTCSpecialAbilities::CreateParticleEffect(effect, ParentThing->GetPos());
    if (PDef->GetSubDef(&effectsDef) && effectsDef->GetEffectFromString("BERSERK_BODY_EFFECT", effect)) {
        BerserkBodyEffect = CTCSpecialAbilities::CreateParticleEffect(effect, ParentThing->GetPos());
        if (IS_THING_ALIVE(BerserkBodyEffect)) {
            CTCDParticleEmitter* emitter;
            BerserkBodyEffect->GetTC(&emitter);
            emitter->AttachToThing(*ParentThing, 42, "", 0, 0.0f);
        }
    }

    CTCSoundPlayer* sound;
    if (ParentThing->GetTC(&sound)) {
        sound->PlayAttachedCriteria("HERO_ABILITY_BEZERK_CAST", true, false, false, -1);
        sound->SetExtraTag("BERSERK");
        switch (SpellLevel) {
        case 0:
            BerserkingLoopingSound = sound->PlayAttachedCriteria("HERO_ABILITY_BEZERK_CONTINUOUS_NO_POWERUP", false, false, false, -1);
            break;
        case 1:
            BerserkingLoopingSound = sound->PlayAttachedCriteria("HERO_ABILITY_BEZERK_CONTINUOUS_LEVEL_1", false, false, false, -1);
            break;
        case 2:
            BerserkingLoopingSound = sound->PlayAttachedCriteria("HERO_ABILITY_BEZERK_CONTINUOUS_LEVEL_2", false, false, false, -1);
            break;
        case 3:
            BerserkingLoopingSound = sound->PlayAttachedCriteria("HERO_ABILITY_BEZERK_CONTINUOUS_LEVEL_3", false, false, false, -1);
            break;
        }
    }
}
