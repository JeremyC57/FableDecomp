// CTCHeroStats::ApplyBonusItemEffects(const CBonusItemDef& bonus_item_def) -- apply a bonus item to the hero: over-eating turns into fatness, health now or over time, stamina, the three trainable experiences, morality, the three appearance stats, capped max health and max stamina, the moonfish / golden-carrot time-of-day change, the speed multiplier and its duration, the particle effect, drink
// Leaf: ?ApplyBonusItemEffects@CTCHeroStats@@QAEXABVCBonusItemDef@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057C4F1, 1192 bytes, ebp frame, `leave ; ret 4` (the size-optimised frame-pointer TU).
// Retail-asm: 0x0057C8E5 `fld dword ptr [ebp-0Ch]` / 0x0057C8E8 `fadd dword ptr [ebp-8]` / 0x0057C8EB `fistp dword ptr [ebp+8]` ..
//   0x0057C8FF `dec dword ptr [ebp+8]` -- the engine's inline-assembly ceiling, through the shared engine/GFFloatToLongCeil.h helper
//   (the speed-multiplier frame count). No asm in this source.
// TU mode: `#pragma optimize("s", on)` (the size-optimised TU of this class, as 50 landed rows of it): bitset::test 0x00410DE0,
//   IS_THING_ALIVE 0x0040DCE0 and GetTCDriver<CTCDParticleEmitter> 0x004ACB50 are called out of line; without it the row is 1200v1192.
//   Ego_d agrees (its `ptr = 0` lines 646 / 660 / 692 are the 4-byte `and [m],0` of size-favoured code). The delivered text also had
//   `#pragma optimize("y", off)`: byte-neutral for this row (the size mode keeps the ebp frame by itself), removed by audit.
// Source shape: Ego_d.pdb locals (bonus_item_def, parent_physical, extra_health_points, ptc_hero_experience, max_extra_max_health,
//   capped_max_health_increase, capped_max_stamina_increase, max_extra_max_stamina, time_manager / ptc_sound_player twice, ptc_haste,
//   pparticle, ptcd_particle_emitter) and line table (lines 570..702), one statement per entry: 571 parent_physical, 572..577 the
//   over-eating fatness, 581..587 health now / over time, 590 stamina, 592..599 the three experiences, 603..606 morality and the three
//   appearance stats, 608..622 capped max health, 625..639 capped max stamina (ExtraMaxStamina 27 bytes, then MaxStamina and Stamina
//   21 each), 644..669 the two time-of-day arms, 673..679 the speed multiplier, 684/685 the two stats-frame resets, 687..694 the
//   particle effect, 698/700 drink. Zero tests are `> DELTA_NOTIONAL_ZERO` on fabsf (see the `// Inferred:` note below).
#pragma optimize("s", on)
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_APPLYBONUSITEMEFFECTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL_MAXHEALTH
#define FABLE_RETAIL_CTHINGPHYSICAL_HEALTHMODIFIERS
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_PEEKHEROSTATSDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMETIMEMANAGER_FULL
#define FABLE_RETAIL_CTCHASTE
#define FABLE_RETAIL_CTCHASTE_SETACTIONSPEEDMULTIPLIER_OUTOFLINE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#include <math.h>
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThingPhysical.h"
#include "engine/CTCHeroStats.h"
#include "engine/CBonusItemDef.h"
#include "engine/CHeroStatsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CGameTimeManager.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCHaste.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CMainGameComponent.h"
#include "engine/ThingAlive.h"
#include "engine/GFFloatToLongCeil.h"

// Inferred: the three float absolute values are spelled `fabsf(x)` (VC7.1 <math.h>: `inline float fabsf(float _X) { return ((float)fabs((double)_X)); }`).
//   Retail has the x87 `fabs` with the value kept across the compare (0x0057C571, 0x0057C652, 0x0057C844: `fabs ; fld st(0) ; fcomp ; ..
//   fstp st(0)`), the shape of an inline wrapper round the intrinsic. It is NOT GFFloatAbs: that function is 21 bytes in Ego_d.pdb and in
//   FableWin.pdb (lines 323..326 = 3 / 11 / 3 / 4: `mov eax,[n] ; and eax,7FFFFFFFh ; mov [n],eax`, then `fld [n]`), the sign-bit mask the
//   landed rows 0x006E3440 / 0x006823D0 / 0x006D1250 expand as `and [m],7FFFFFFFh`; with that body this row is DIFFER 1204v1192.
//   <math.h>'s `abs(x)` and `fabs(x)` float overloads give the same retail bytes (measured); FableWin.pdb has all three wrappers
//   (@fabsf@4, ?abs@@YAMM@Z, ?fabs@@YAMM@Z). fabsf is the fastcall one in that build, which is what the debug line sizes sit closest to.

void CTCHeroStats::ApplyBonusItemEffects(const CBonusItemDef& bonus_item_def)
{
    CThingPhysical& parent_physical = static_cast<CThingPhysical&>(GetParentThing());
    if (bonus_item_def.AddsFatness)
    {
        float extra_health_points = bonus_item_def.HealthModifier - (parent_physical.GetMaxHealth() - parent_physical.GetHealth());
        if (extra_health_points > DELTA_NOTIONAL_ZERO)
        {
            // Inferred: the product first, Fatness after it -- retail calls GetDefinitionManager (0x0057C52A) before it reads
            // Fatness (`fadd dword ptr [esi+38h]`, 0x0057C53E); with `GetFatness() + ..` VC7.1 copies Fatness to a frame word first
            // (DIFFER 1198v1192). The debug statement is 77 bytes either way.
            SetFatness(extra_health_points * GetDefinitionManager().PeekHeroStatsDef().AmountFatnessIncreasesPerPointOfOverEating + GetFatness());
        }
    }

    if (bonus_item_def.SecondsToApplyHealthIncreaseOver < DELTA_NOTIONAL_ZERO)
    {
        parent_physical.ModifyHealth(bonus_item_def.HealthModifier, true);
    }
    else if (fabsf(bonus_item_def.HealthModifier) > DELTA_NOTIONAL_ZERO)
    {
        parent_physical.ModifyHealthOverTime(bonus_item_def.HealthModifier, (long)(CMainGameComponent::GetConstantFPS() * bonus_item_def.SecondsToApplyHealthIncreaseOver));
    }

    ModifyStaminaBy(bonus_item_def.StaminaModifier);

    if (bonus_item_def.StrengthExperience > 0 || bonus_item_def.SkillExperience > 0 || bonus_item_def.WillExperience > 0)
    {
        CTCHeroExperience* ptc_hero_experience;
        if (GetParentThing().GetTC(&ptc_hero_experience))
        {
            ptc_hero_experience->AddTrainableStatExperience(HERO_STAT_STRENGTH, bonus_item_def.StrengthExperience);
            ptc_hero_experience->AddTrainableStatExperience(HERO_STAT_SKILL, bonus_item_def.SkillExperience);
            ptc_hero_experience->AddTrainableStatExperience(HERO_STAT_WILL, bonus_item_def.WillExperience);
        }
    }

    SetMorality(GetMorality() + bonus_item_def.MoralityChange);
    Agreeableness += bonus_item_def.AgreeablenessChange;
    InherentAttractiveness += bonus_item_def.AttractivenessChange;
    InherentScariness += bonus_item_def.ScarinessChange;

    if (fabsf(bonus_item_def.MaxHealthIncrease) > DELTA_NOTIONAL_ZERO)
    {
        float capped_max_health_increase = bonus_item_def.MaxHealthIncrease;
        float max_extra_max_health = GetMaxExtraMaxHealth();
        if (capped_max_health_increase + ExtraMaxHealth > max_extra_max_health)
        {
            capped_max_health_increase = max_extra_max_health - ExtraMaxHealth;
        }
        if (capped_max_health_increase > 0.0f)
        {
            ExtraMaxHealth += capped_max_health_increase;
            parent_physical.SetMaxHealth(parent_physical.GetMaxHealth() + capped_max_health_increase, false);
            parent_physical.ModifyHealth(capped_max_health_increase, false);
        }
    }

    if (abs(bonus_item_def.MaxStaminaIncrease) > 0)
    {
        long capped_max_stamina_increase = bonus_item_def.MaxStaminaIncrease;
        long max_extra_max_stamina = GetMaxExtraMaxStamina();
        if (capped_max_stamina_increase + ExtraMaxStamina > max_extra_max_stamina)
        {
            capped_max_stamina_increase = max_extra_max_stamina - ExtraMaxStamina;
        }
        if (capped_max_stamina_increase > 0)
        {
            ExtraMaxStamina += capped_max_stamina_increase;
            MaxStamina += capped_max_stamina_increase;
            Stamina += capped_max_stamina_increase;
        }
    }

    if (bonus_item_def.ChangeTimeOfDayToNight)
    {
        CTCSoundPlayer* ptc_sound_player = 0;
        if (GetParentThing().GetTC(&ptc_sound_player))
        {
            ptc_sound_player->PlayAttachedCriteria("MOONFISH", true, false, false, -1);
        }
        CGameTimeManager& time_manager = GetWorld().GetGameTimeManager();
        if (!time_manager.IsNightTime())
        {
            time_manager.SetTimeOfDay(0.875f);
        }
    }
    else if (bonus_item_def.ChangeTimeOfDayToDay)
    {
        CTCSoundPlayer* ptc_sound_player = 0;
        if (GetParentThing().GetTC(&ptc_sound_player))
        {
            ptc_sound_player->PlayAttachedCriteria("GOLDEN_CARROT", true, false, false, -1);
        }
        CGameTimeManager& time_manager = GetWorld().GetGameTimeManager();
        if (!time_manager.IsDay())
        {
            time_manager.SetTimeOfDay(10.0f / 24.0f);
        }
    }

    if (fabsf(1.0f - bonus_item_def.SpeedMultiplier) > DELTA_NOTIONAL_ZERO && bonus_item_def.SpeedMultiplier > DELTA_NOTIONAL_ZERO && bonus_item_def.SecondsForSpeedMultiplierToLast > DELTA_NOTIONAL_ZERO)
    {
        CTCHaste* ptc_haste;
        if (GetParentThing().GetTC(&ptc_haste))
        {
            ptc_haste->SetActionSpeedMultiplier(this, bonus_item_def.SpeedMultiplier);
            FramesLeftForSpeedMultiplier = GFFloatToLongCeil(CMainGameComponent::GetConstantFPS() * bonus_item_def.SecondsForSpeedMultiplierToLast);
        }
    }

    FrameUpdatedClothingStats = 0;
    FrameUpdatedTattooStats = 0;

    if (bonus_item_def.ParticleEffect > 0)
    {
        CThing* pparticle = CTCDParticleEmitter::Create(bonus_item_def.ParticleEffect, GetParentThing().GetPos(), false);
        if (IS_THING_ALIVE(pparticle))
        {
            CTCDParticleEmitter* ptcd_particle_emitter = 0;
            pparticle->GetTCDriver(&ptcd_particle_emitter);
            ptcd_particle_emitter->AttachToThing(GetParentThing(), 8, "", 0, 0.0f);
        }
    }

    if (bonus_item_def.AddsDrunkenness)
    {
        Drink(bonus_item_def.AlcoholicStrength);
    }
}