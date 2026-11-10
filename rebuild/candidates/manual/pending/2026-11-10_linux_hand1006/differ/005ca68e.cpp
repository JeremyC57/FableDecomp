// CTCInventoryWeapons::SetupActiveWeaponsOnCarrier() -- dresses the carrier for the active weapons (unless it is the hero with weapon use disallowed, or the inventory is dying)
// Grade: DIFFER(1538v1535) -- reg-alloc only: c2 colours `this` (priority -72) before the TCI_WEAPON constant 0x48 (-76), so ours keeps `this` in ebx through the in-hand / passive-slot test and rematerialises 0x48 in the else arm (`push 48h ; pop ebx`, +3 bytes); retail keeps 0x48 in ebx across both arms (0x005CA7BE .. 0x005CA97F) with `this` reloaded into edi from [ebp+70h], and its this-spill slot is [ebp+70h] (ours [ebp+6Ch]). Every instruction otherwise matches; c2prio.py shows the same candidate set, the priorities 4 apart. Chunk 1006: 6 more probes (passive-weapon PeekTC local, else-arm CIntelligentPointer reference, hero/weapon locals at function top) leave the priorities unchanged; regraded DIFFER(1538v1535) with the overlay
// Leaf: ?SetupActiveWeaponsOnCarrier@CTCInventoryWeapons@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CA68E, 1535 bytes, thiscall `ret`.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ISDYING
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
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
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
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
#pragma auto_inline(off)
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
    const CTCHero* hero;
    if (ParentThing->PeekTC(&hero) && !hero->IsWeaponUseAllowed())
        return;
    if (IsDying())
        return;
    CTCMultiStrike* multiStrike = 0;
    ParentThing->GetTC(&multiStrike);
    CTCMultiArrow* multiArrow = 0;
    ParentThing->GetTC(&multiArrow);
    CTCCarrying* carrying;
    ParentThing->GetTC(&carrying);
    CThing* inHand = carrying->GetThingInPrimarySlot();
    RefreshSheathedWeapons();
    const CTCWeapon* weapon;
    if (IS_THING_ALIVE(inHand) && inHand->PeekTC(&weapon)) {
        // a weapon in hand: unsheathe the active one of the other kind
        if (weapon->IsMeleeWeapon()) {
            if (IS_THING_ALIVE(PActiveMeleeWeapon) && PActiveMeleeWeapon.GetPItem() != inHand) {
                CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(ParentThing);
                CCreatureAction_UnsheatheItemFromInventory action(*creature, *PActiveMeleeWeapon.Get(), 0, 150);
                if (!creature->SetCurrentAction(action))
                    action.PerformAction();
                if (multiStrike)
                    multiStrike->RemoveEffectOnWeapon(inHand);
            }
        } else {
            if (IS_THING_ALIVE(PActiveRangedWeapon) && PActiveRangedWeapon.GetPItem() != inHand) {
                CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(ParentThing);
                CCreatureAction_UnsheatheItemFromInventory action(*creature, *PActiveRangedWeapon.Get(), 0, 150);
                if (!creature->SetCurrentAction(action))
                    action.PerformAction();
                if (multiArrow)
                    multiArrow->RemoveEffectOnWeapon(inHand);
            }
        }
    } else if (IS_THING_ALIVE(PActiveMeleeWeapon)) {
        // nothing in hand: the active melee weapon goes to its passive carry slot
        CDefPointer<const CCarryableDef> carryableDef;
        PActiveMeleeWeapon.Get()->GetPBaseDef()->GetSubDef(&carryableDef);
        long slot = carryableDef->PassiveCarrySlot;
        CThing* thing = carrying->GetThingInCarrySlot(slot);
        if (IS_THING_ALIVE(thing) && thing->PeekTC(&weapon)) {
            if (weapon->IsMeleeWeapon() && IS_THING_ALIVE(PActiveMeleeWeapon) && PActiveMeleeWeapon.GetPItem() != thing) {
                if (multiStrike)
                    multiStrike->RemoveEffectOnWeapon(thing);
            } else if (weapon->IsProjectileWeapon() && IS_THING_ALIVE(PActiveRangedWeapon) && PActiveRangedWeapon.GetPItem() != thing) {
                if (multiArrow)
                    multiArrow->RemoveEffectOnWeapon(thing);
            }
        }
        if (IS_THING_DEAD(thing)) {
            carrying->PutThingInCarrySlot(*PActiveMeleeWeapon.Get(), slot, true);
        } else if (PActiveMeleeWeapon.GetPItem() != thing) {
            if (!thing->PeekTC(&weapon) || weapon->IsMeleeWeapon()) {
                carrying->RemoveThing(*thing);
                CTCInventoryItem* item = 0;
                thing->GetTC(&item);
                item->ReturnToInventory();
                carrying->PutThingInCarrySlot(*PActiveMeleeWeapon.Get(), slot, true);
            }
        }
    }
    // put away every other carried weapon that is not a projectile
    std::vector<CThing*> carried;
    carrying->GetAllCarriedThings(carried);
    for (unsigned long i = 0; i < carried.size(); ++i) {
        CThing* thing = carried[i];
        CTCWeapon* carriedWeapon;
        if (thing->GetTC(&carriedWeapon) && thing != PActiveMeleeWeapon && thing != PActiveRangedWeapon && !carriedWeapon->IsProjectile()) {
            carrying->RemoveThing(*thing);
            if (!CTCCarrying::IsThingCarryingWeapon(*ParentThing)) {
                CTCCreatureModeManager* modes;
                ParentThing->GetTC(&modes);
                modes->RemoveMode(4);
            }
            CTCInventoryItem* item;
            if (thing->GetTC(&item))
                item->ReturnToInventory();
        }
    }
    if (multiStrike && IS_THING_ALIVE(PActiveMeleeWeapon) && PActiveMeleeWeapon.GetPItem() != inHand)
        multiStrike->ApplyEffectOnWeapon(PActiveMeleeWeapon.Get());
    if (multiArrow && multiArrow->GetShotsRemaining() > 0 && IS_THING_ALIVE(PActiveRangedWeapon) && PActiveRangedWeapon.GetPItem() != inHand)
        multiArrow->ApplyEffectOnWeapon(PActiveRangedWeapon.Get());
}
