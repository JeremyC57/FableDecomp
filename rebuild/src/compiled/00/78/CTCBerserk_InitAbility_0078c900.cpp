// 0078c900 CTCBerserk::InitAbility
// Leaf: ?InitAbility@CTCBerserk@@QAEXXZ
// Flags: none
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CTCBERSERK_HIT_EFFECT
#define FABLE_RETAIL_CTCBERSERK_INITABILITY_PASS29
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
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
#define FABLE_RETAIL_STLP_RB_GLOBAL_DECREMENT_SIZE_BODY
#define FABLE_RETAIL_QUICK_MENU_PAIR_CONSTRUCT_PASS27
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_EXTRATAG
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_FULL
#define FABLE_RETAIL_BATTLECHARGE_RADIAL_FLOAT_PASS27
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
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
#ifdef _MSC_VER
#pragma optimize("t",on)
#endif
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"

void CTCBerserk::InitAbility()
{
    AbilityInitialised = true;
    CEnvironmentSpellEffectInterpolator& interp = GetWorld().GetEnvironment().GetSpellEffectInterpolator();
    RadialBlurFadeKey = interp.RadialBlurFadeTo(
        0.0f, NScreenEffect::SCREEN_EFFECT_PRIORITY_SPELL, PDef->BerserkThreshold, PDef->BerserkLength, PDef->BerserkIntensity,
        PDef->BerserkFade, PDef->BerserkRange, PDef->BerserkTwist);

    CTCHaste* ptc_haste;
    if (!GetParentThing().GetTC(&ptc_haste))
        ptc_haste = GFDebugPDynamicCast(GetParentThing().AddTC("CTCHaste", true, 0), (CTCHaste*)0);
    ptc_haste->SetActionSpeedMultiplier(this, PDef->SpeedMultiplier);

    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    long effect = 0;
    if (GetParentThing().PeekBaseDef().GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString("SKILL_STRENGTH_INTRO", effect)) {
        CThing* pemitter = CTCSpecialAbilities::CreateParticleEffect(effect, GetParentThing().GetPos());
    }
    if (GetParentThing().PeekBaseDef().GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString("BERSERK", effect)) {
        CThing* pemitter = CTCSpecialAbilities::CreateParticleEffect(effect, GetParentThing().GetPos());
    }
    if (PDef->GetSubDef(&pspecial_effects) && pspecial_effects->GetEffectFromString("BERSERK_BODY_EFFECT", effect)) {
        BerserkBodyEffect = CTCSpecialAbilities::CreateParticleEffect(effect, GetParentThing().GetPos());
        if (IS_THING_ALIVE(BerserkBodyEffect)) {
            CTCDParticleEmitter* ptcdParticleEmitter;
            if (!BerserkBodyEffect->GetTC(&ptcdParticleEmitter)) { FABLE_RETAIL_ASSERT(false); }
            ptcdParticleEmitter->AttachToThing(GetParentThing(), 42, "", 0, 0.0f);
        }
    }

    CTCSoundPlayer* ptc_sound_player;
    if (GetParentThing().GetTC(&ptc_sound_player)) {
        ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_CAST", true, false, false, -1);
        ptc_sound_player->SetExtraTag("BERSERK");
        switch (SpellLevel) {
        case 0:
            BerserkingLoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_CONTINUOUS_NO_POWERUP", false, false, false, -1);
            break;
        case 1:
            BerserkingLoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_CONTINUOUS_LEVEL_1", false, false, false, -1);
            break;
        case 2:
            BerserkingLoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_CONTINUOUS_LEVEL_2", false, false, false, -1);
            break;
        case 3:
            BerserkingLoopingSound = ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_BEZERK_CONTINUOUS_LEVEL_3", false, false, false, -1);
            break;
        }
    }
}