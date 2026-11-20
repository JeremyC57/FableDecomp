// CTCInventoryWeapons::SetupActiveWeaponsOnCarrier() -- dresses the carrier for the active weapons (unless it is the hero with weapon use disallowed, or the inventory is dying): unsheathe the active weapon of the other kind than the one in hand, or sheathe the active melee weapon in its passive slot, put away every other carried non-projectile weapon, and re-apply the multi-strike / multi-arrow effects
// Inline-budget: `#pragma optimize("s", on)` (the TU's size optimisation, as every landed CTCInventoryWeapons TU-mate: removed, DIFFER(1676v1535)); `#pragma auto_inline(off)` round the same-TU IS_THING_ALIVE<CThing*> / IS_THING_DEAD<CThing*> bodies (out of line in retail: 2 calls to 0x0040DCE0, 1 to 0x005CBDBD; pragma removed, DIFFER(1542v1535); bodies declaration-only, DIFFER(1553v1535) / DIFFER(1551v1535)); the visible GetSubDef<CCarryableDef> body (called at 0x005CA918 -> 0x005CC10E; declaration-only, DIFFER(1546v1535); no pragma needed round it, measured); header opt-ins FABLE_RETAIL_CTHING_GETTC_EXPANDED (the inline GetTC / PeekTC expansions round the CVectorMap::LowerBound 0x0040F020 calls: removed, DIFFER(1325v1535)) and FABLE_RETAIL_STLP_BITSET_TEST_DECL (bitset<276>::test called out of line, 0x00410DE0 x11: removed, same bytes but the object binds a different test symbol).
// Leaf: ?SetupActiveWeaponsOnCarrier@CTCInventoryWeapons@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CA68E, 1535 bytes, thiscall `ret`.
// Naming: Ego_r.pdb 0x0062937A / FableWin.pdb tc_inventory_weapons.obj #26 (0x0237C899, 4229 B = Ego_d's size) ?SetupActiveWeaponsOnCarrier@CTCInventoryWeapons@@AAEXXZ.
//   Locals are Ego_d's (ptc_hero, ptc_multistrike, ptc_multiarrow, ptc_carrying, pcarried_thing, ptc_carried_weapon, parent_creature, action_dude,
//   pcarryable_def, active_melee_weapon_sheathe_slot, psheathed_thing, ptc_sheathed_weapon, ptc_sheathed_thing_inventory_item, carried_things,
//   c1, pthing, ptc_weapon, ptc_inventory_item, ptc_cmm); `ptc_carried_weapon = 0` is Ego_d line 674 (a 4-byte `and [ebp-x],0`).
// Shape: Ego_d's own ptc_sheathed_weapon, declared in the sheathed-thing else block (its scope +0x900 opens at line 748 with the combined
//   `PActiveMeleeWeapon != psheathed_thing && (!PeekTC || IsMeleeWeapon)` test), is what keeps the TCI_WEAPON constant 0x48 in ebx across both
//   arms (0x005CA7BE .. 0x005CA97F) with `this` reloaded from [ebp+70h]: reusing ptc_carried_weapon there (the draft) gives DIFFER(1538v1535).
// Callees: IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340 / <CThing*> 0x0040DCE0, IS_THING_DEAD<CThing*> 0x005CBDBD, CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound 0x0040F020, bitset<276>::test 0x00410DE0, CBaseIntelligentPointer::GetPItem 0x00A01B50, RefreshSheathedWeapons 0x005C9962,
//   GetSubDef<CCarryableDef> 0x005CC10E, CTCCarrying GetThingInPrimarySlot / GetThingInCarrySlot / PutThingInCarrySlot / RemoveThing / GetAllCarriedThings /
//   IsThingCarryingWeapon, CTCWeapon IsMeleeWeapon / IsProjectileWeapon / IsProjectile, CCreatureAction_UnsheatheItemFromInventory ctor / PerformAction 0x0069F9E0 /
//   dtor, CThingCreatureBase::SetCurrentAction 0x006644F0, CTCMultiStrike / CTCMultiArrow Remove- / ApplyEffectOnWeapon, CTCCreatureModeManager::RemoveMode,
//   CTCInventoryItem::ReturnToInventory, operator!=(const CBaseObject*, const CIntelligentPointer<CThing>&), free 0x00BFEA14.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CTCINVENTORYBASE_ISDYING
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_ISPROJECTILE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCCARRYING_PUTTHING
#define FABLE_RETAIL_CTCCARRYING_REMOVETHING
#define FABLE_RETAIL_CTCCARRYING_ALLCARRIED
#define FABLE_RETAIL_CTCCARRYING_CARRYSLOT
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CTCHERO_WEAPONUSE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCARRYABLEDEF
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCHero.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCMultiStrike.h"
#include "engine/CTCMultiArrow.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_UnsheatheItemFromInventory.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCarryableDef.h"
#include "engine/CDefinitionManager.h"

template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);          // 0x004CC340 (CIntelligentPointer<CThing>), 0x0040DCE0 (CThing*)
template <class T> bool FABLE_FASTCALL IS_THING_DEAD(const T& thing);           // 0x005CBDBD (CThing*)

// Same-TU body of the def lookup retail's TU holds (out of line in retail: GetSubDef<CCarryableDef> 0x005CC10E).
template <class T>
bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(off)
template <> inline bool FABLE_FASTCALL IS_THING_ALIVE<CThing*>(CThing* const& thing)
{
    if (thing != 0 && thing->IsDead() == 0)
        return true;
    return false;
}
template <> inline bool FABLE_FASTCALL IS_THING_DEAD<CThing*>(CThing* const& thing)
{
    if (thing == 0 || thing->IsDead())
        return true;
    return false;
}
#pragma auto_inline(on)

void CTCInventoryWeapons::SetupActiveWeaponsOnCarrier()
{
    const CTCHero* ptc_hero;
    if (ParentThing->PeekTC(&ptc_hero) && !ptc_hero->IsWeaponUseAllowed())
        return;
    if (IsDying())
        return;
    CTCMultiStrike* ptc_multistrike = 0;
    ParentThing->GetTC(&ptc_multistrike);
    CTCMultiArrow* ptc_multiarrow = 0;
    ParentThing->GetTC(&ptc_multiarrow);
    CTCCarrying* ptc_carrying;
    ParentThing->GetTC(&ptc_carrying);
    CThing* pcarried_thing = ptc_carrying->GetThingInPrimarySlot();
    RefreshSheathedWeapons();
    const CTCWeapon* ptc_carried_weapon = 0;
    if (IS_THING_ALIVE(pcarried_thing) && pcarried_thing->PeekTC(&ptc_carried_weapon)) {
        // a weapon in hand: unsheathe the active one of the other kind
        if (ptc_carried_weapon->IsMeleeWeapon()) {
            if (IS_THING_ALIVE(PActiveMeleeWeapon) && PActiveMeleeWeapon.GetPItem() != pcarried_thing) {
                CThingCreatureBase* parent_creature = static_cast<CThingCreatureBase*>(ParentThing);
                CCreatureAction_UnsheatheItemFromInventory action_dude(*parent_creature, *PActiveMeleeWeapon.Get(), 0, 150);
                if (!parent_creature->SetCurrentAction(action_dude))
                    action_dude.PerformAction();
                if (ptc_multistrike)
                    ptc_multistrike->RemoveEffectOnWeapon(pcarried_thing);
            }
        } else {
            if (IS_THING_ALIVE(PActiveRangedWeapon) && PActiveRangedWeapon.GetPItem() != pcarried_thing) {
                CThingCreatureBase* parent_creature = static_cast<CThingCreatureBase*>(ParentThing);
                CCreatureAction_UnsheatheItemFromInventory action_dude(*parent_creature, *PActiveRangedWeapon.Get(), 0, 150);
                if (!parent_creature->SetCurrentAction(action_dude))
                    action_dude.PerformAction();
                if (ptc_multiarrow)
                    ptc_multiarrow->RemoveEffectOnWeapon(pcarried_thing);
            }
        }
    } else if (IS_THING_ALIVE(PActiveMeleeWeapon)) {
        // nothing in hand: the active melee weapon goes to its passive carry slot
        CDefPointer<const CCarryableDef> pcarryable_def;
        PActiveMeleeWeapon.Get()->GetPBaseDef()->GetSubDef(&pcarryable_def);
        long active_melee_weapon_sheathe_slot = pcarryable_def->PassiveCarrySlot;
        CThing* psheathed_thing = ptc_carrying->GetThingInCarrySlot(active_melee_weapon_sheathe_slot);
        if (IS_THING_ALIVE(psheathed_thing) && psheathed_thing->PeekTC(&ptc_carried_weapon)) {
            if (ptc_carried_weapon->IsMeleeWeapon() && IS_THING_ALIVE(PActiveMeleeWeapon) && PActiveMeleeWeapon.GetPItem() != psheathed_thing) {
                if (ptc_multistrike)
                    ptc_multistrike->RemoveEffectOnWeapon(psheathed_thing);
            } else if (ptc_carried_weapon->IsProjectileWeapon() && IS_THING_ALIVE(PActiveRangedWeapon) && PActiveRangedWeapon.GetPItem() != psheathed_thing) {
                if (ptc_multiarrow)
                    ptc_multiarrow->RemoveEffectOnWeapon(psheathed_thing);
            }
        }
        if (IS_THING_DEAD(psheathed_thing)) {
            ptc_carrying->PutThingInCarrySlot(*PActiveMeleeWeapon.Get(), active_melee_weapon_sheathe_slot, true);
        } else {
            const CTCWeapon* ptc_sheathed_weapon;
            if (PActiveMeleeWeapon.GetPItem() != psheathed_thing && (!psheathed_thing->PeekTC(&ptc_sheathed_weapon) || ptc_sheathed_weapon->IsMeleeWeapon())) {
                ptc_carrying->RemoveThing(*psheathed_thing);
                CTCInventoryItem* ptc_sheathed_thing_inventory_item = 0;
                psheathed_thing->GetTC(&ptc_sheathed_thing_inventory_item);
                ptc_sheathed_thing_inventory_item->ReturnToInventory();
                ptc_carrying->PutThingInCarrySlot(*PActiveMeleeWeapon.Get(), active_melee_weapon_sheathe_slot, true);
            }
        }
    }
    // put away every other carried weapon that is not a projectile
    std::vector<CThing*> carried_things;
    ptc_carrying->GetAllCarriedThings(carried_things);
    for (unsigned long c1 = 0; c1 < carried_things.size(); ++c1) {
        CThing* pthing = carried_things[c1];
        CTCWeapon* ptc_weapon;
        if (pthing->GetTC(&ptc_weapon) && pthing != PActiveMeleeWeapon && pthing != PActiveRangedWeapon && !ptc_weapon->IsProjectile()) {
            ptc_carrying->RemoveThing(*pthing);
            if (!CTCCarrying::IsThingCarryingWeapon(*ParentThing)) {
                CTCCreatureModeManager* ptc_cmm;
                ParentThing->GetTC(&ptc_cmm);
                ptc_cmm->RemoveMode(4);
            }
            CTCInventoryItem* ptc_inventory_item;
            if (pthing->GetTC(&ptc_inventory_item))
                ptc_inventory_item->ReturnToInventory();
        }
    }
    if (ptc_multistrike && IS_THING_ALIVE(PActiveMeleeWeapon) && PActiveMeleeWeapon.GetPItem() != pcarried_thing)
        ptc_multistrike->ApplyEffectOnWeapon(PActiveMeleeWeapon.Get());
    if (ptc_multiarrow && ptc_multiarrow->GetShotsRemaining() > 0 && IS_THING_ALIVE(PActiveRangedWeapon) && PActiveRangedWeapon.GetPItem() != pcarried_thing)
        ptc_multiarrow->ApplyEffectOnWeapon(PActiveRangedWeapon.Get());
}