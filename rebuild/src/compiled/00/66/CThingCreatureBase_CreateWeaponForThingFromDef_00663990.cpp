// CThingCreatureBase::CreateWeaponForThingFromDef(CThing& thing, long defIndex) -- create a weapon object at the thing's position for its player; a thing with a weapons inventory gets it as a default inventory item made the active weapon, any other thing has it put in its carrying component's active carry slot (the secondary one when a living thing already holds the primary), after clearing the passive slot
// Leaf: ?CreateWeaponForThingFromDef@CThingCreatureBase@@SIXAAVCThing@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00663990, 381 bytes (+ int3 padding), static __fastcall (ecx = thing, edx = def index), plain `ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?CreateWeaponForThingFromDef@CThingCreatureBase@@SIXAAVCThing@@J@Z; the landed
//   CCreatureAction_BanditKingChangeWeapons::PerformAction 0x008C8960 calls 0x00663990 with (creature, def).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CThing::GetPos 0x004C73D0,
//   CThingObject::Create 0x00703210, CVectorMap::LowerBound 0x0040F020 (inline GetTC: 0x13 weapons inventory, 0x1A inventory
//   item, 0x46 carrying), CTCInventoryItem::AddToInventory 0x005D8D50, CTCInventoryWeapons::SetThingAsActiveWeapon 0x005CB2B0,
//   CDefClassBase::GetSubDef<CCarryableDef> 0x005CC10E (the shared template body, out of line), CTCCarrying::RemoveThingInCarrySlot
//   0x006A16A0 / GetThingInCarrySlot 0x006A11F0 / PutThingInCarrySlot 0x006A1F80.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_CREATEDBYOWNER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_ADDTOINVENTORY
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATEWEAPON
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCARRYABLEDEF
#define FABLE_RETAIL_CTCCARRYING_PUTTHING
#define FABLE_RETAIL_CTCCARRYING_SLOTACCESS
#define FABLE_RETAIL_CTCCARRYING_CARRYSLOT
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCCarrying.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCarryableDef.h"
#include "engine/CDefinitionManager.h"

#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
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
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif

void CThingCreatureBase::CreateWeaponForThingFromDef(CThing& thing, long defIndex)
{
    if (defIndex == 0)
        return;
    CThingObject* object = CThingObject::Create(defIndex, thing.GetPos(), thing.GetPlayerNumber(), 0, 0, "");
    CTCInventoryWeapons* weapons;
    if (thing.GetTC(TCI_INVENTORY_WEAPONS, &weapons)) {
        CTCInventoryItem* item;
        if (object->GetTC(&item)) {
            item->DefaultItem = true;
            item->AddToInventory(*weapons, false, true, 0, false, false);
            weapons->SetThingAsActiveWeapon(object->GetDefGlobalIndex(), 0);
        }
    } else {
        CTCCarrying* carrying = 0;
        thing.GetTC(&carrying);
        CDefPointer<const CCarryableDef> def;
        object->PeekBaseDef().GetSubDef(&def);
        carrying->RemoveThingInCarrySlot(def->PassiveCarrySlot, false);
        long slot = def->ActiveCarrySlot;
        CThing* held = carrying->GetThingInCarrySlot(slot);
        if (held && !held->IsDead())
            slot = def->SecondaryActiveCarrySlot;
        carrying->PutThingInCarrySlot(*object, slot, false);
        object->SetAsCreatedByOwnerThingCreation(true);
    }
}