// CTCHeroRangedAttacks::FrameUpdate() -- per-frame ranged attack: fire a queued shot; or, while the fire cycle aims, play the bow draw sound, advance or relax the pull-back, cap the pull-back timer at the root of the pull-back curve, set decapitation / damage / accuracy and load and fire; otherwise finish or start loading the carried projectile weapon, else cancel aiming
// Leaf: ?FrameUpdate@CTCHeroRangedAttacks@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078A690, 1097 bytes, `ret`.
// Source shape: Ego_d.pdb locals (roots, ptc_carrying, ptc_sound_player, THRESHOLD_BUTTON_PRESSURE, weight, max_aiming_timer,
// pcarried_thing / ptc_weapon / ptc_carry per arm, player) and line table (lines 356..526): one if / else-if chain whose arms end in
// `return`, CancelAimingWeapon() as the common tail (line 525).
// TU mates (tc_hero_ranged_attacks.cpp): the anonymous-namespace pull-back helpers and functor and the NMaths::GetRootsNumerically<F>
// body (landed 0x0078B7C0 text; out of line in retail too) are visible, so the roots vector and the functor temporary do not escape:
// frame 0x14, `_M_start` in ebp (declaration only: frame 0x1C, start reloaded from the frame).
// GetShotBuildupTime (0x00789A60) is an ordinary member defined later in the TU (Ego_d lines 751..753, no local) and expanded here
// by /Ob2 (0x0078A861 `fild` / 0x0078A868 `fidiv`); its body follows the row.
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_CTCHERORANGEDATTACKS_GETSHOTBUILDUPTIME_OUT_OF_LINE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCWEAPON_LOADING
#define FABLE_RETAIL_CTCWEAPON_AUTOLOADING
#define FABLE_RETAIL_CTCCARRYING_MEMBERS
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_PLAYATTACHEDSOUNDEVENT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#include "stlport/fable_containers.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CHeroCombatDef.h"
#include "engine/CPlayer.h"
#include "engine/EPlayerMode.h"
#include "engine/ThingAlive.h"
#include "engine/GFTanh.h"
#include "engine/C3DVector.h"
#include "engine/CTCHeroExperience.h"
#include <math.h>

namespace {
// The TU's file-local helpers (texts of the landed 0x0078B7C0 / 0x00789CE0 rows).
inline float GetBowPullBackAmountFromPullBackTimer(long pullback_timer) { float tanh_weight = 0.05f; return (float)GFTanh(pullback_timer * tanh_weight); }
class CAnalyseRootsOfBowPullBackFunction {
    float DesiredPullBackAmount;
public:
    CAnalyseRootsOfBowPullBackFunction(float desired_pullback_amount) : DesiredPullBackAmount(desired_pullback_amount) { }
    float GetValue(float x_axis) const { return DesiredPullBackAmount - GetBowPullBackAmountFromPullBackTimer((long)x_axis); }
    bool IsValid() const;
};
// Fidelity: same-TU strong definition of 0x00789CE0 (retail TU-mate)
// (the landed row's text verbatim; out of line in retail, called at 0x0078A883; an anonymous-namespace function can only be defined in its own TU).
float FABLE_FASTCALL GetDamageMultiplierFromThingAtBuildUpTime(const CThing& thing, CDefPointer<const CHeroCombatDef> pdef, float buildup_time)
{
    float fractional_strength = 1.0f;
    const CTCHeroExperience* ptc_hero_exp;
    if (thing.PeekTC(&ptc_hero_exp))
        fractional_strength = pdef->RangedDamageMultiplierWeighting.GetParameter(*ptc_hero_exp);
    float comp1 = GFTanh(buildup_time * pdef->RangedDamageMultiplierSpeedNominalBuildup) * pdef->RangedDamageMultiplierScaleNominalBuildup;
    float comp2 = GFSqr(buildup_time * pdef->RangedDamageMultiplierSpeedSuperBuildup) * pdef->RangedDamageMultiplierScaleSuperBuildup;
    float damage_m = (pdef->RangedDamageMultiplierSpeedWithStatParameter * fractional_strength + 1.0f) * (comp1 + comp2);
    return damage_m;
}
}
// Template text (re-port, audit 2026-11-20, with row 0x008185B0): NMaths::GetRootsNumerically<F> is now the Ego_d-shaped text of that
//   row (line table 32..168 and block-scoped locals; evidence and the measurements in its header), so that every instance of the
//   template is compiled from one text. Byte-neutral here: FrameUpdate is RELOCATION_MATCH with either text and the instance this object emits equals retail 0x0078B7C0 (1390 / 1390).
// Inferred: in the template, the do-loop's first value is `func.GetValue(r_start)` (not `mod_range_start`; the same value, Ego_d's line
//   54 is 21 bytes either way) -- fitted by row 0x008185B0, carried here with the rest of the text.
// NMaths::GetRootsNumerically<F>: the template body is visible in the TU and stays out of line, as in retail (0x0078B7C0; the text of
// that landed row). Seeing it, VC7.1 knows the call keeps neither the result vector's nor the functor's address.
extern const float DELTA_NOTIONAL_ZERO;
namespace NMaths {
template<class F> std::vector<float> FABLE_FASTCALL GetRootsNumerically(float range_start, float range_end, const F& func, long num_roots, long num_asymptotes)
{
    float mod_range_start = range_start - DELTA_NOTIONAL_ZERO;
    float mod_range_end = range_end + DELTA_NOTIONAL_ZERO;

    std::vector<std::pair<float, float> > roots_between;

    long found_roots = 0;
    long num_steps = num_roots + num_asymptotes - 1;

    do
    {
        roots_between.clear();
        found_roots = 0;
        num_steps++;

        float step = (mod_range_end - mod_range_start) / num_steps;
        float r_start = mod_range_start;
        float r_end = r_start + step;

        float val_a = func.GetValue(r_start);
        for (long c1 = 0; c1 != num_steps; c1++)
        {
            float val_b = func.GetValue(r_end);

            if ((val_a >= 0.0f && val_b <= 0.0f) || (val_a <= 0.0f && val_b >= 0.0f))
            {
                found_roots++;
                roots_between.push_back(std::make_pair(r_start, r_end));
            }

            val_a = val_b;
            r_start = r_end;
            r_end += step;
        }

    } while (found_roots < num_roots + num_asymptotes);

    std::vector<float> roots;
    for (std::vector<std::pair<float, float> >::iterator it = roots_between.begin(); it != roots_between.end(); ++it)
    {
        const std::pair<float, float>& roots_pair = *it;

        float r_start = roots_pair.first;
        float r_end = roots_pair.second;

        const float accuracy = 0.001f;

        float val_a = func.GetValue(r_start);
        float val_b = func.GetValue(r_end);

        std::pair<bool, float> val_a_converging;
        val_a_converging.first = true;
        val_a_converging.second = val_a;
        std::pair<bool, float> val_b_converging;
        val_b_converging.first = true;
        val_b_converging.second = val_b;

        while (r_end - r_start > accuracy)
        {
            float r_mid = (r_start + r_end) * 0.5f;

            val_b = func.GetValue(r_mid);
            if (val_a <= 0.0f)
            {
                if (val_b <= 0.0f)
                {
                    val_a = val_b;
                    r_start = r_mid;

                    val_a_converging.first = !((float)fabs(val_a) > (float)fabs(val_a_converging.second) + DELTA_NOTIONAL_ZERO);
                    val_a_converging.second = val_a;
                }
                else
                {
                    r_end = r_mid;

                    val_b_converging.first = !((float)fabs(val_b) > (float)fabs(val_b_converging.second) + DELTA_NOTIONAL_ZERO);
                    val_b_converging.second = val_b;
                }
            }
            else
            {
                if (val_b >= 0.0f)
                {
                    val_a = val_b;
                    r_start = r_mid;

                    val_a_converging.first = !((float)fabs(val_a) > (float)fabs(val_a_converging.second) + DELTA_NOTIONAL_ZERO);
                    val_a_converging.second = val_a;
                }
                else
                {
                    r_end = r_mid;

                    val_b_converging.first = !((float)fabs(val_b) > (float)fabs(val_b_converging.second) + DELTA_NOTIONAL_ZERO);
                    val_b_converging.second = val_b;
                }
            }
        }

        if (val_a_converging.first && val_b_converging.first)
            roots.push_back(r_start);
    }

    for (std::vector<float>::iterator it = roots.begin(); it != roots.end(); )
    {
        float root = *it;

        if (root >= range_start && root < range_end)
        {
            it++;
        }
        else
        {
            it = roots.erase(it);
        }
    }

    return roots;
}
}

void CTCHeroRangedAttacks::FrameUpdate()
{
    DamageMultiplier = 0.0f;
    Accuracy = 0.0f;
    Decapitate = false;

    if (IsAbleToUseRangedWeapon() && FireCycleInProgress && !Aiming && !FireQueued)
    {
        AttemptToFireRangedWeapon();
        return;
    }
    else if (IsAbleToUseRangedWeapon() && FireCycleInProgress && (Aiming || FireQueued))
    {
        if (AimingTimer >= 0)
        {
            CTCSoundPlayer* ptc_sound_player;
            if (GetParentThing().GetTC(&ptc_sound_player) && BowDrawbackSoundID == 0)
            {
                BowDrawbackSoundID = ptc_sound_player->PlayAttachedSoundEvent("SE_DRAW_BOW", false, false, false);
            }
        }

        if (ButtonPressure >= LastButtonPressure)
        {
            if (AimingTimer >= 0)
            {
                AimingTimer++;
                PullBackTimer++;
            }
            PullBackAmount = GetBowPullBackAmountFromPullBackTimer(PullBackTimer);
        }
        else
        {
            const float THRESHOLD_BUTTON_PRESSURE = 0.3f;
            if (ButtonPressure < THRESHOLD_BUTTON_PRESSURE)
            {
                if (++ButtonReleaseCancelTimer > 2)
                {
                    CancelAimingWeapon();
                    return;
                }
            }

            float weight = 3.0f;
            PullBackAmount = (PullBackAmount * weight + ButtonPressure) / (weight + 1.0f);
            if (PullBackAmount < 0.0f) PullBackAmount = 0.0f; else if (PullBackAmount > 1.0f) PullBackAmount = 1.0f;
        }

        std::vector<float> roots = NMaths::GetRootsNumerically(0.0f, 10000.0f, CAnalyseRootsOfBowPullBackFunction(ButtonPressure), 1, 0);
        if (!roots.empty())
        {
            float max_aiming_timer = roots.front();
            if (PullBackTimer > max_aiming_timer)
            {
                PullBackTimer = (long)max_aiming_timer;
            }
        }

        if (PullBackAmount > 0.95f)
        {
            Decapitate = true;
        }

        if (AimingTimer >= 0)
        {
            DamageMultiplier = GetDamageMultiplierFromThingAtBuildUpTime(GetParentThing(), PHeroCombatDef, GetShotBuildupTime());
        }

        Accuracy = 100.0f;

        const CTCCarrying* ptc_carrying;
        if (GetParentThing().PeekTC(&ptc_carrying))
        {
            CThing* pcarried_thing = ptc_carrying->GetThingInPrimarySlot();
            const CTCWeapon* ptc_weapon;
            if (IS_THING_ALIVE(pcarried_thing) && pcarried_thing->PeekTC(&ptc_weapon) && ptc_weapon->IsProjectileWeaponLoaded() && AimingTimer < 0)
            {
                AimingTimer = 0;
                PullBackTimer = 0;
                Loading = false;
            }
        }

        LoadRangedWeapon();
        FireRangedWeapon();

        LastButtonPressure = ButtonPressure;

        if (!FireQueued)
        {
            Aiming = false;
        }
        return;
    }
    else if (IsAbleToUseRangedWeapon())
    {
        CTCCarrying* ptc_carry;
        GetParentThing().GetTC(&ptc_carry);
        CThing* pcarried_thing = ptc_carry->GetThingInPrimarySlot();
        CTCWeapon* ptc_weapon;
        pcarried_thing->GetTC(&ptc_weapon);

        if (ptc_weapon->IsProjectileWeaponLoaded())
        {
            Loading = false;
            return;
        }
        else if (ptc_weapon->IsProjectileWeaponAutoLoading())
        {
            LoadRangedWeapon();
            return;
        }
    }
    else
    {
        CTCCarrying* ptc_carry;
        GetParentThing().GetTC(&ptc_carry);
        CThing* pcarried_thing = ptc_carry->GetThingInPrimarySlot();
        if (IS_THING_ALIVE(pcarried_thing))
        {
            CTCWeapon* ptc_weapon;
            if (pcarried_thing->GetTC(&ptc_weapon) && ptc_weapon->IsProjectileWeapon())
            {
                if (!ptc_weapon->IsProjectileWeaponLoaded() && ptc_weapon->IsProjectileWeaponAutoLoading())
                {
                    CPlayer& player = GetParentThing().GetPlayer();
                    if (player.GetCurrentMode() == PLAYER_MODE_CONTROL_CREATURE)
                    {
                        player.AddMode(PLAYER_MODE_USE_PROJECTILE_WEAPON_THIRD_PERSON, false);
                    }
                }
            }
        }
    }

    CancelAimingWeapon();
}

// Fidelity: same-TU strong definition of 0x00789A60 (retail TU-mate)
// (defined after FrameUpdate as in Ego_d, lines 751..753; the owner row's text, same 10 bytes `fild [ecx+18h] ; fidiv [ConstantFPS] ; ret`).
// Bracket: `((float)AimingTimer)` -- the no-code PAREN tuple (0x269) takes fild's 3-cycle result edge in c2's list scheduler, so
// fidiv is ready one cycle after fild and is issued with `sub esp,8` (retail 0x0078A868 fidiv, 0x0078A86E sub; measured with
// tools/sched t20: fild+mov / fidiv+sub / test / mov). Without it fidiv waits for cycle 3 and lands after `test eax,eax`
// (DIFFER 1097v1097, that one instruction). The double-local, (float)-on-the-divisor, whole-quotient-bracket and call-site-bracket
// spellings all keep the late fidiv (9 measured).
float CTCHeroRangedAttacks::GetShotBuildupTime() const
{
    return ((float)AimingTimer) / CMainGameComponent::GetConstantFPS();
}