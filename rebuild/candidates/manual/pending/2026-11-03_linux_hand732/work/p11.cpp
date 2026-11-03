// CThingPlayerCreature::SetPlayerCreatureActionForMelee(ECombatZone zone, const C3DVector& direction, CThing* target, const CCreatureAbilityDef& ability) -- hero melee action
// Leaf: ?SetPlayerCreatureActionForMelee@CThingPlayerCreature@@IAEXW4ECombatZone@@ABVC3DVector@@PAVCThing@@ABVCCreatureAbilityDef@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CKICKABLEDEF
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_NCREATUREMODE
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTION_STABTHING
#define FABLE_RETAIL_CCOMBATACTION_KICKTHINGONGROUND
#define FABLE_RETAIL_CCREATUREACTION_INTERRUPTABLEMIDATTACKAUTOTURN
#include "fable_string.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCCombat.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCTargeted.h"
#include "engine/CKickableDef.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCStealth.h"
#include "engine/NCreatureMode.h"
#include "engine/NPlayerTargeting.h"
#include "engine/CCombatManager.h"
#include "engine/CCreatureAbilityDef.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatAction_StabThing.h"
#include "engine/CCombatAction_KickThingOnGround.h"
#include "engine/CCreatureAction_InterruptableMidAttackAutoTurn.h"

template<class T> inline bool __fastcall IS_THING_ALIVE(const T& thing) { return thing && !thing->IsDead(); }   // FableWin ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z

void CThingPlayerCreature::SetPlayerCreatureActionForMelee(ECombatZone zone, const C3DVector& direction, CThing* target,
                                                          const CCreatureAbilityDef& ability)
{
    CTCCombat* combat;
    GetTC(&combat);
    CDefPointer<const CMeleeCombatAbilityDef> melee_ability;
    if (ability.Type == (ECreatureAbility)1 && combat->IsMeleeFlourishValid())
        melee_ability = combat->GetPMeleeFlourishAbility();
    else if (ability.Type == (ECreatureAbility)7 && combat->IsMeleeBlockCounterValid())
        melee_ability = combat->GetPMeleeBlockCounterAbility();
    const CTCCreatureModeManager* modes;
    if (PeekTC(&modes) && modes->GetCurrentMode().GetType() == NCreatureMode::MODE_CARRYING_GENERIC_BOX) {
        CTCCarrying* carrying;
        GetTC(&carrying);
        carrying->RemoveThingFromPrimarySlot(false);
    }
    bool ranged = false;
    CThing* weapon = 0;
    const CTCCarrying* carrying;
    if (PeekTC(&carrying)) {
        weapon = carrying->GetThingInPrimarySlot();
        const CTCWeapon* weapon_tc;
        if (IS_THING_ALIVE(weapon) && weapon->PeekTC(&weapon_tc) && !weapon_tc->IsMeleeWeapon() && weapon_tc->IsProjectileWeapon())
            ranged = true;
    }
    CCountedPointer<CCreatureActionBase> action;
    CTCTargeted* targeted;
    if (IS_THING_ALIVE(target) && target->GetTC(&targeted) && targeted->IsZTargetable()
        && NPlayerTargeting::IsThingInTargetingRangeForTargetType(*this, *target, 4)) {
        {
            bool ignore_navigation = false;
            CDefPointer<const CKickableDef> kickable;
            if (target->GetPBaseDef()->GetSubDef(&kickable))
                ignore_navigation = kickable->IgnoreNavigationTest;
            CTCCreatureNavigation* navigation;
            GetTC(&navigation);
            if (ignore_navigation
                || navigation->IsAbleToNavigateFromPosToPos(GetPos(), target->GetPos(), GetCombatManager().GetProximityForNavigation())) {
                CTCStealth* stealth;
                if ((NCreatureMode::IsStabbableByHero(*target) || target->GetType() == 13) && GetTC(&stealth)
                    && stealth->IsAvailableToPerformStealthAttack(*this))
                    action.Reset(new CCombatAction_StabThing(*this, *target));
                else
                    action.Reset(new CCombatAction_KickThingOnGround(*this, *target));
            }
        }
    } else if (!ranged) {
        if (IS_THING_ALIVE(target) && NCreatureMode::IsAbleToBeAttacked(*target) == true)
            action.Reset(new CCreatureAction_InterruptableMidAttackAutoTurn(*this, *target, weapon, direction, melee_ability));
        else
            action.Reset(new CCreatureAction_InterruptableMidAttackAutoTurn(*this, weapon, direction, melee_ability));
    }
    if (action.Get())
        SetCurrentAction(*action);
}
