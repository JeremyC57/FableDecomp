// Leaf: ?Dispatch@CActionEventShot@NCreatureAction@@UAEXAAVCCreatureActionBase@@@Z
// Flags: /O2 /Oy /W3
#define FABLE_RETAIL_ACTION_EVENT_SHOT_PASS30
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CBASECLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_GFDEBUGDYNAMICCAST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETSHOTACCURACYPERCENTAGE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCCARRYING_REMOVETHING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CWEAPONDEF_SIZE_BODY
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_ASSERT_DEAD_ARM

#include "fable_assert.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/ECombatAbilityDataType.h"
#include "engine/CTCCombat.h"
#include "engine/CTCCarrying.h"
#include "engine/CCombatActionBase.h"
#include "engine/CWeaponDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NThingShotHelpers.h"
#include "engine/NCreatureActionEvents.h"
#include "engine/ThingAlive.h"

void NCreatureAction::CActionEventShot::Dispatch(CCreatureActionBase& action)
{
    FABLE_RETAIL_ASSERT(action.IsStrikeAction());
    if (action.IsStrikeAction()) {
        CCombatActionBase& combat_action = *GFDebugDynamicCast(&action, static_cast<CCombatActionBase*>(0));
        const CThing* pTarget = combat_action.GetTarget();
        CDefPointer<const CWeaponDef> pweapon_def;
        if (IS_THING_ALIVE(pTarget) && GetThing().PeekBaseDef().GetSubDef(&pweapon_def)) {
            GetThing().MakeNoise("SIMPLE_NOISE");
            const CTCCombat* ptc_combat;
            long index;
            if (GetThing().PeekTC(&ptc_combat) && ptc_combat->GetShotTypeOverride() > 0)
                index = ptc_combat->GetShotTypeOverride();
            else
                index = pweapon_def->Shot;
            if (index > 0) {
                long acc = GetThing().GetShotAccuracyPercentage();
                CThing* pthing_carried = 0;
                CTCCarrying* ptc_carrying;
                if (GetThing().GetTC(&ptc_carrying) &&
                    (pthing_carried = ptc_carrying->GetThingInPrimarySlot()) != 0) {
                    CDefPointer<const CWeaponDef> pcarried_thing_weapon_def;
                    if (pthing_carried->GetPBaseDef()->GetSubDef(&pcarried_thing_weapon_def) &&
                        pcarried_thing_weapon_def->Class == WC_PROJECTILE) {
                        CRightHandedSet rhs(pthing_carried->GetPhysicsTC()->GetRHSet());
                        ptc_carrying->RemoveThing(*pthing_carried);
                        pthing_carried->GetPhysicsTC()->SetRHSet(rhs);
                        NThingShot::CreateShot(*pthing_carried, &GetThing(), &GetThing(), pTarget, static_cast<float>(acc),
                            1.0f, 0.0f, CDefPointer<const CMeleeCombatAbilityDef>(), index);
                    } else {
                        pthing_carried = 0;
                    }
                }
                if (!pthing_carried) {
                    C3DVector pos(GetThing().GetPhysicsTC()->GetPosition() + C3DVector(0.0f, 0.0f, 1.0f));
                    CRightHandedSet rhs(GetThing().GetPhysicsTC()->GetRHSet());
                    CThing* pweapon = combat_action.GetWeaponUsedInStrikeAction();
                    CCharString weapon_attachment_dummy("WEAPON_FOCUS_01");
                    CCharString strike_event_helper_name = combat_action.GetStrikeEventHelperName();
                    if ((!IS_THING_ALIVE(pweapon) ||
                         !pweapon->GetTCGraphicAppearance()->GetDummyObject(weapon_attachment_dummy, 0, 0, pos, rhs, 1.0f)) &&
                        !GetThing().GetTCGraphicAppearance()->GetHelperPoint(strike_event_helper_name, 0, 0, pos, 1.0f)) {
                        FABLE_RETAIL_ASSERT(false);
                    }
                    CThing* pShot = NThingShot::CreateShot(index, &GetThing(), 0, pTarget, static_cast<float>(acc),
                        1.0f, 0.0f, CDefPointer<const CMeleeCombatAbilityDef>(), &pos, &rhs);
                    static_cast<void>(pShot);
                }
            }
        }
    }
}