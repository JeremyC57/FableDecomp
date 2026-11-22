// CTCDivineWrath::CTCDivineWrath(CThing& thing)
// Leaf: ??0CTCDivineWrath@@QAE@AAVCThing@@@Z
// Flags:
// Inline-budget: the original sub-definition helper remains out of line, and the shared CRC and map bodies are visible to the hand-effect lookup.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_FORWARDER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_TRANSITIONTOTHEME
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CENGINEPRIMITIVETHEMECHANNELDESC_INDICES
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_GUI_DRAW_PASS23
#define FABLE_RETAIL_DIVINE_WRATH_CTOR_PASS28
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#define FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_LIGHTING
#define FABLE_RETAIL_SKYBEAM_ORIGINAL
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_INLINE
#define FABLE_RETAIL_CCRC_ORIGINAL_TYPED_LOOP_PASS27
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_NAMED_ARGS
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#define FABLE_RETAIL_STLP_ULONG_LONG_MFIND_SIZE_PASS28
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#define FABLE_RETAIL_PHYSICAL_SHIELD_EFFECT_PASS26
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCDivineWrath.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CSpecialAbilitiesDivineWrathDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/ThingAlive.h"

extern const CCharString PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;
extern const CCharString SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;

CTCDivineWrath::CTCDivineWrath(CThing& thing)
    : CTCBase(thing), PItemRemoved(0), DivineWrathActionInitialised(false),
      Timer(HERO_ABILITY_DIVINE_WRATH_SPELL, GetParentThing()), SpellLevel(0),
      NumFramesBetweenDamageHit(0), DurationOfSpellFrames(0), NumFramesForEnvChange(0),
      CurFramesForEnvChange(0), Finished(false), NumFramesToWaitForFadeOut(0),
      CutSceneCancel(false), PTarget(0), IsZTargetting(false), PerformedReleaseAction(false),
      PerFrameStaminaModify(0), AccModifyStamina(0), StaminaModify(0), BuildUpEffect(0),
      SpellExclusionChannel(5, 5, 0xff), LoopingSound(0)
{
    CTCSpecialAbilities* ptc_spec_abils;
    if (GetParentThing().GetTC(&ptc_spec_abils))
        ptc_spec_abils->LockAbilitiyMutex(HERO_ABILITY_DIVINE_WRATH_SPELL);

    long def_index = GetDefinitionManager().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_DIVINE_WRATH_DEF", 1);
    GetDefinitionManager().GetDef(def_index, &PDef);

    const CTCInventoryAbilities* ptc_inventory_abilities;
    if (GetParentThing().PeekTC(&ptc_inventory_abilities))
        SpellLevel = ptc_inventory_abilities->GetAbilityLevel(HERO_ABILITY_DIVINE_WRATH_SPELL);

    NumFramesBetweenDamageHit = static_cast<long>(CMainGameComponent::GetConstantFPS() * PDef->NumSecsBetweenDamageHit[SpellLevel]);
    DurationOfSpellFrames = static_cast<long>(CMainGameComponent::GetConstantFPS() * PDef->DurationOfSpellSecs[SpellLevel]);
    NumFramesForEnvChange = static_cast<long>(CMainGameComponent::GetConstantFPS() * PDef->EnvChangeSecs[SpellLevel]);

    long theme_index = GetDefinitionManager().GetDefGlobalIndexFromName("ENVIRONMENT_DIVINE_WRATH");
    if (theme_index > 0) {
        for (long i = 0; i < 6; ++i) {
            if (i == 5)
                continue;
            GetWorld().GetEnvironment().TransitionToTheme(i, 2, theme_index, PDef->EnvChangeSecs[SpellLevel]);
        }
    }

    NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance = GetParentThing().GetTCGraphicAppearance();
    if (ptc_graphic_appearance) {
        HeroDefaultChannel = ptc_graphic_appearance->PeekLightingChannel();
        ptc_graphic_appearance->SetLightingChannel(SpellExclusionChannel, true, true);
    }

    CTCVolumeContainmentTracker* ptc_volume_contain;
    if (GetParentThing().GetTC(&ptc_volume_contain))
        ptc_volume_contain->SetUpdateLightingChannels(false);

    float perframe = PDef->StaminaCostPerSec[SpellLevel] / CMainGameComponent::GetConstantFPS();
    StaminaModify = static_cast<long>(perframe);
    PerFrameStaminaModify = perframe - static_cast<float>(StaminaModify);

    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    long effect_index_lo = 0;
    long effect_index_hi = 0;
    if (PDef->GetSubDef(&pspecial_effects)) {
        if (pspecial_effects->GetEffectFromString("SMALL_RING_EFFECT", effect_index_lo)
            && pspecial_effects->GetEffectFromString("LARGE_RING_EFFECT", effect_index_hi)) {
            float interpolate = (PDef->RadiusOfEffect[SpellLevel] - PDef->SmallRingRadius)
                / (PDef->LargeRingRadius - PDef->SmallRingRadius);
            BuildUpEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(
                effect_index_lo, effect_index_hi, interpolate, GetParentThing().GetPos(), false));
            if (IS_THING_ALIVE(BuildUpEffect)) {
                CTCDParticleEmitter* ptcEmitter = 0;
                BuildUpEffect->GetTC(&ptcEmitter);
                ptcEmitter->AttachToThing(GetParentThing(), 8, "", 0, 0.0f);
                ptcEmitter->SetAlwaysUpdate(true);
            }
        }
    }

    long hand_effect = 0;
    if (pspecial_effects->GetEffectFromString("HAND_EFFECT", hand_effect)) {
        HandEffect1 = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(hand_effect, GetParentThing().GetPos()));
        HandEffect2 = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(hand_effect, GetParentThing().GetPos()));
        if (IS_THING_ALIVE(HandEffect1)) {
            CTCDParticleEmitter* tcParticleEmitter = 0;
            if (HandEffect1->GetTC(&tcParticleEmitter) && tcParticleEmitter)
                tcParticleEmitter->AttachToThing(GetParentThing(), 10, PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME, 0, 0.0f);
        }
        if (IS_THING_ALIVE(HandEffect2)) {
            CTCDParticleEmitter* tcParticleEmitter = 0;
            HandEffect2->GetTC(&tcParticleEmitter);
            tcParticleEmitter->AttachToThing(GetParentThing(), 10, SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME, 0, 0.0f);
        }
    }

    PlaySoundW(PDef->StartPowerupSound[SpellLevel], false);
    LoopingSound = PlaySoundW(PDef->LoopingPowerupSound[SpellLevel], false);
}