// Leaf: ?PutThingInCarrySlot@CTCCarrying@@QAEXAAVCThing@@J_N@Z
// Flags: (none)

#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST_REMOVE_DECL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCCARRYING_PUTTHING
#define FABLE_RETAIL_CTCCARRYING_SLOTFREE
#define FABLE_RETAIL_CTCCARRYING_SLOTACCESS
#define FABLE_RETAIL_CTCCARRYING_MEMBERS
#define FABLE_RETAIL_CTCCARRYABLE_SETASCARRIEDBY
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_ISPROJECTILE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CMODEBASE_ORIGINAL_GETTYPE_PASS26
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CVectorMap.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NCreatureMode.h"
#include "engine/CTCCreatureModeManager.h"
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include "engine/CDefPointer.h"
#include "engine/CCarrySlotDef.h"
#include "engine/CGameDefinitionManager.h"

bool CTCCarrying::IsCarrySlotFree(long slot) const
{
    CVectorMap<long, CIntelligentPointer<CThing> >::const_iterator slot_it = CarrySlots.Find(slot);
    if (slot_it == CarrySlots.end())
        return false;
    return slot_it->second.Get() == 0;
}

bool CTCCarrying::IsSlotPrimarySlot(long slot) const
{
    CDefPointer<const CCarrySlotDef> pdef(NULL_DEF);
    if (!CGameDefinitionManager::Get().GetDef(slot, &pdef))
        FABLE_RETAIL_ASSERT_ARM(false);
    return pdef->PrimarySlot;
}

void CTCCarrying::PutThingInCarrySlot(CThing& thing, long slot, bool set_up_weapon_mode)
{
    if (!IsCarrySlotFree(slot))
        RemoveThingInCarrySlot(slot, false);

    CVectorMap<long, CIntelligentPointer<CThing> >::iterator slot_it = CarrySlots.Find(slot);
    if (slot_it != CarrySlots.end()) {
        slot_it->second = CIntelligentPointer<CThing>(&thing);
        CTCCarryable* ptc_carryable;
        thing.GetTC(&ptc_carryable);
        ptc_carryable->SetAsCarriedBy(this);
        thing.GetPhysicsTC()->ResetInterpolation();
    }

    ThingsInPrimarySlots.remove(CIntelligentPointer<CThing>(0));
    thing.SetInLimbo(false);
    if (IsSlotPrimarySlot(slot)) {
        ThingsInPrimarySlots.push_back(CIntelligentPointer<CThing>(&thing));
        const CTCWeapon* ptc_weapon;
        if (set_up_weapon_mode && thing.PeekTC(&ptc_weapon) && !ptc_weapon->IsProjectile()) {
            CTCCreatureModeManager* ptc_modes;
            if (GetParentThing().GetTC(&ptc_modes)) {
                NCreatureMode::EMode old_mode_type = ptc_modes->GetCurrentMode().GetType();
                if (old_mode_type != NCreatureMode::MODE_CARRYING_WEAPON_STAND)
                    ptc_modes->AddMode(NCreatureMode::MODE_CARRYING_WEAPON_STAND);
                if (old_mode_type == NCreatureMode::MODE_STRAFE) {
                    ptc_modes->RemoveMode(old_mode_type);
                    ptc_modes->AddMode(old_mode_type);
                    return;
                }
                if (old_mode_type == NCreatureMode::MODE_BERSERK) {
                    ptc_modes->RemoveMode(old_mode_type);
                    ptc_modes->AddMode(old_mode_type);
                    return;
                }
                if (old_mode_type == NCreatureMode::MODE_BLOCK) {
                    ptc_modes->RemoveMode(old_mode_type);
                    ptc_modes->AddMode(old_mode_type);
                }
            }
        }
    }
}