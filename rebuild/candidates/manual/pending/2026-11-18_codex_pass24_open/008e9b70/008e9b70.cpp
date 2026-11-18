// NCreatureMode::CModeStrafe::AnimUpdate refreshes main, upper-body and ranged-weapon animation layers.
// Grade: DIFFER(1736v1705). The shared model keeps the animation decisions and targets. The prologue, construction order and frame stores still differ from retail.
// Leaf: ?AnimUpdate@CModeStrafe@NCreatureMode@@UAEXAAVCAnimateThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E9B70, 1705 bytes, CModeStrafe vtable 0x01268588 slot 2 (virtual, `ret 4`), frame 0x44, no EH frame.

#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_INTERRUPTION
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_LOADING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_NCREATUREMODE_CMODESTRAFE_PASS24
#define FABLE_RETAIL_CTCSPECIALABILITIES
#define FABLE_RETAIL_CTCSPECIALABILITIES_STRAFE_CALLS_PASS24
#define FABLE_RETAIL_CTCANIMATINGOBJECT_PLAYANIMATION
#define FABLE_RETAIL_CTCANIMATINGOBJECT_MANUAL_FRAME_PASS24
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#define FABLE_RETAIL_CANIMATETHING_MAINANIM
#define FABLE_RETAIL_CANIMATETHING_UPDATEANIMATION
#define FABLE_RETAIL_CANIMATETHING_REMOVE_COMBINATION_CALL_PASS24
#define FABLE_RETAIL_CANIMSET_TYPE
#define FABLE_RETAIL_CANIMSET_STRAFE_ACCESSORS_PASS24
#define FABLE_RETAIL_CANIMATIONENTRY_ACCESSORS
#define FABLE_RETAIL_CANIMATIONENTRY_BLENDTIMES
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCCreatureModeManager.h"

extern const float DELTA_NOTIONAL_ZERO;   // 0x0129BA3C

#include "engine/NCreatureMode_CModeStrafe.h"
#include "engine/CAnimateThing.h"
#include "engine/CAnimationEntry.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCAnimatingObject.h"

namespace NCreatureMode {
void CModeStrafe::AnimUpdate(CAnimateThing& anim)
{
    const CThingCreatureBase& creature = static_cast<const CThingCreatureBase&>(ParentThing);
    CCharString anim_name = GetStrafingAnimationName(creature, (EMovementType)0);
    if (anim_name != StrafeAnimName || anim.PeekMainAnim().IsFinished() || anim.GetMainAnimType() != 0x1b) {
        StrafeAnimName = anim_name;
        GetAnimDistanceAndIndexFromAnimName(creature, anim_name, AnimIndex, AnimMovementDistance);
        const CAnimationEntry& anim_entry = creature.GetAnimFromName(anim_name, 0);
        anim.SetMainAnim(CAnimInit(0x1b, 0, AnimIndex, ANIM_UPDATE_FROM_SPEED, true, anim_entry.GetAnimateThingFlags(), 0x5a),
            ForceAnimNoBlendIn ? 0 : anim_entry.GetTimeToBlendIn(ParentThing), ForceAnimNoBlendIn ? 0 : anim_entry.GetDelayTime());
    }
    CThing* pcarried = 0;
    const CTCWeapon* ptc_weapon = 0;
    bool add_bow_anims = false;
    const CTCCarrying* ptc_carrying;
    if (creature.PeekTC(TCI_CARRYING, &ptc_carrying)) {
        pcarried = ptc_carrying->GetThingInPrimarySlot();
        if (pcarried && pcarried->PeekTC(TCI_WEAPON, &ptc_weapon) && ptc_weapon->IsProjectileWeapon())
            add_bow_anims = true;
    }
    bool locked = false;
    const CTCSpecialAbilities* ptc_spec_abils;
    if (creature.PeekTC(TCI_SPECIAL_ABILITIES, &ptc_spec_abils) && ptc_spec_abils->GetAbilityMutexLock())
        locked = true;
    if (locked || !add_bow_anims) {
        anim.RemoveCombinationAnimsOfType(0x24);
        anim.RemoveCombinationAnimsOfType(0x25);
        anim.RemoveCombinationAnimsOfType(0x1f);
        CAnimSet* panim_set = anim.GetFirstCombinationAnimFromType(0x23);
        if (locked && ptc_spec_abils->GetCurrentAbilityStrafeAnim() != "") {
            CCharString spec_abil_animname = ptc_spec_abils->GetCurrentAbilityStrafeAnim();
            const CAnimationEntry& anim_entry = creature.GetAnimFromName(spec_abil_animname, 0);
            if (!panim_set || panim_set->GetAnimIndex() != anim_entry.BankIndex) {
                anim.RemoveCombinationAnimsOfType(0x23);
                CAnimInit anim_init(0x23, 0, anim_entry.BankIndex, ANIM_UPDATE_FROM_SPEED, true, anim_entry.GetAnimateThingFlags(), 0x5f);
                anim.AddCombinationAnim(anim_init, ForceAnimNoBlendIn ? 0 : anim_entry.GetTimeToBlendIn(creature), 0, false, false);
            }
        } else if (const NAnimationEntryBodge::CAnimEntryLite* lite = PAnimStrafeUpperBody.Data) {
            if (!panim_set || panim_set->GetAnimIndex() != lite->BankIndex) {
                anim.RemoveCombinationAnimsOfType(0x23);
                CAnimInit anim_init(0x23, 0, lite->BankIndex, ANIM_UPDATE_FROM_SPEED, true, lite->AnimFlags, 0x5f);
                anim.AddCombinationAnim(anim_init, ForceAnimNoBlendIn ? 0 : lite->GetTimeToBlendIn(creature), 0, false, false);
            }
        } else
            anim.RemoveCombinationAnimsOfType(0x23);
    } else {
        anim.RemoveCombinationAnimsOfType(0x23);
        if (PAnimProjectileAim.Data) {
            float aiming_frame;
            if (!GetBestFrameInAimingAnim(aiming_frame))
                aiming_frame = -1.0f;
            CAnimSet* panim_set = anim.GetFirstCombinationAnimFromType(0x24);
            if (!panim_set) {
                do {
                const NAnimationEntryBodge::CAnimEntryLite* lite = PAnimProjectileAim.Data;
                CAnimInit anim_init(0x24, 0, lite->BankIndex, ANIM_UPDATE_NONE, true, lite->AnimFlags, 0x5f);
                anim.AddCombinationAnim(anim_init, ForceAnimNoBlendIn ? 0 : lite->GetTimeToBlendIn(ParentThing), 0, false, false);
                } while (0);
                panim_set = anim.GetFirstCombinationAnimFromType(0x24);
                if (panim_set)
                    panim_set->SetFrame(CAnimFrameInfo(aiming_frame > -DELTA_NOTIONAL_ZERO ? aiming_frame : 0.5f, aiming_frame > -DELTA_NOTIONAL_ZERO ? aiming_frame : 0.5f));
            }
            if (panim_set) {
                float last_frame = panim_set->GetPercentageComplete();
                float cur_frame = last_frame;
                if (aiming_frame > -DELTA_NOTIONAL_ZERO)
                    cur_frame = aiming_frame;
                panim_set->SetFrame(CAnimFrameInfo(cur_frame, last_frame));
                panim_set->SetAsUpdated(true);
            }
        }
        if (PAnimProjectileBuildUp.Data) {
            CAnimSet* panim_set = anim.GetFirstCombinationAnimFromType(0x25);
            if (!panim_set || panim_set->IsFinished()) {
                if (panim_set)
                    anim.RemoveCombinationAnim(panim_set);
                const NAnimationEntryBodge::CAnimEntryLite* lite = PAnimProjectileBuildUp.Data;
                CAnimInit anim_init(0x25, 0, lite->BankIndex, ANIM_UPDATE_FROM_SPEED, true, lite->AnimFlags, 0x69);
                anim.AddCombinationAnim(anim_init, ForceAnimNoBlendIn ? 0 : lite->GetTimeToBlendIn(ParentThing), 0, false, false);
                panim_set = anim.GetFirstCombinationAnimFromType(0x25);
                if (panim_set) {
                    panim_set->SetAlpha(RumbleParam);
                    panim_set->SetChangeAlphaBy(0.0f);
                }
            }
            panim_set->SetAlpha(RumbleParam);
            panim_set->SetInternalAlpha(0, RumbleParam, false);
        }
        if (PAnimProjectilePullBack.Data && pcarried && !pcarried->IsDead() && ptc_weapon && ptc_weapon->IsAnalogueLoading()) {
            CAnimSet* panim_load = anim.GetFirstCombinationAnimFromType(0x1f);
            const CCreatureActionBase* paction = creature.PeekPCurrentAction();
            if (paction && !paction->IsFinished() && paction->GetInterruptionGroupID() == 100 && paction->GetInterruptionPriority() > 150) {
                if (panim_load)
                    anim.RemoveCombinationAnim(panim_load);
            } else if (!ptc_weapon->IsProjectileWeaponLoaded()) {
                if (panim_load)
                    anim.RemoveCombinationAnim(panim_load);
            } else {
                CTCAnimatingObject* ptc_anim_obj;
                if (!pcarried->GetTC(TCI_ANIMATING_OBJECT, &ptc_anim_obj))
                    ptc_anim_obj = 0;
                if (!panim_load) {
                    const NAnimationEntryBodge::CAnimEntryLite* lite = PAnimProjectilePullBack.Data;
                    CAnimInit anim_init(0x1f, 0, lite->BankIndex, ANIM_UPDATE_NONE, false, lite->AnimFlags, 0x6e);
                    anim.AddCombinationAnim(anim_init, 0, 0, false, false);
                    panim_load = anim.GetFirstCombinationAnimFromType(0x1f);
                    if (panim_load)
                        panim_load->SetFrame(CAnimFrameInfo(PullBackAmount, PullBackAmount));
                    if (ptc_anim_obj)
                        ptc_anim_obj->SetCurrentAnimAsManualUpdate();
                }
                panim_load->SetFrame(CAnimFrameInfo(PullBackAmount, panim_load->GetPercentageComplete()));
                panim_load->SetAsUpdated(true);
                if (ptc_anim_obj)
                    ptc_anim_obj->SetFractionComplete(PullBackAmount);
            }
        }
    }
    ForceAnimNoBlendIn = false;
}

}
