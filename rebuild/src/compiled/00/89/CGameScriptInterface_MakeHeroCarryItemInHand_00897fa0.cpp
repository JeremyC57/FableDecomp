// CGameScriptInterface::MakeHeroCarryItemInHand(const CScriptThing& thing, bool makeActiveWeapon, bool e) const -- put a valid inventory item (inventory-item TC, carryable TC) into the live hero's hand: add it to the hero's inventory of its type when it has no inventory yet (else it must already be the hero's), optionally make it the active weapon, take the inventory's instance when it was just added, clear the target carry slot, take it out of any other slot and put it into the slot, then bring it out of limbo
// Leaf: ?MakeHeroCarryItemInHand@CGameScriptInterface@@UBEXABVCScriptThing@@_N1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00897FA0, 541 bytes (+ int3 padding), thiscall `ret 0Ch`. Identity: retail
//   CGameScriptInterface vtable 0x01260F0C (engine/CGameScriptInterface.h); Ego_r.pdb / FableWin.pdb
//   ?MakeHeroCarryItemInHand@CGameScriptInterface@@UBEXABVCScriptThing@@_N1@Z.
// Callees (PDB spellings): CPlayerManager::GetMainPlayer() 0x00449970 (non-const, through the PlayerManager +0x14 pointer),
//   CPlayer::GetPControlledCreature() const 0x00487DC0, CScriptThing slots 75 IsAlive / 11 GetPThing, CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound 0x0040F020 (GetTC) / const 0x004365B0 (PeekTC), CTCInventoryItem::GetInventoryType() const 0x005D9030 /
//   AddToInventory 0x005D8D50 / GetInventoryOwner() const 0x005D8E90, bitset<274>::test's range check (puts 0x00BFEAF2 + abort
//   0x00BFEAEC, "bitset" 0x0122E068), CTCInventoryWeapons::SetThingAsActiveWeapon 0x005CB2B0, NInventory::CTCInventoryBase slot 47
//   GetItemOfTypeInInventory (+0xBC), CTCCarryable::GetCarrySlotActive() const 0x00705CC0, CTCCarrying::IsCarrySlotFree(long) const
//   0x006A1230 / RemoveThingFromPrimarySlot 0x006A1E00 / IsCarryingThing(const CThing&) const 0x006A1060 / RemoveThing 0x006A19B0 /
//   PutThingInCarrySlot 0x006A1F80, CThing::SetInLimbo 0x004C8CF0.
// Model: shared headers only (overlay opt-ins CTCINVENTORYITEM_INVENTORYACCESS, CTCCARRYING_SLOTFREE; hdr/README.md). The prior
//   local-model draft (2026-10-22 hand287) parked at DIFFER(541v541) on an object/item ebx-ebp swap; on the shared CThing GetTC /
//   PeekTC bodies (GETTC_EXPANDED) and the real class models it matches as written: the swap was a local-model artefact.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_ADDTOINVENTORY
#define FABLE_RETAIL_CTCINVENTORYITEM_INVENTORYACCESS
#define FABLE_RETAIL_CTCCARRYING_PUTTHING
#define FABLE_RETAIL_CTCCARRYING_REMOVETHING
#define FABLE_RETAIL_CTCCARRYING_SLOTFREE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCCarrying.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"

void CGameScriptInterface::MakeHeroCarryItemInHand(const CScriptThing& thing, bool makeActiveWeapon, bool e) const
{
    CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (!hero || hero->IsDead())
        return;
    if (!thing.IsAlive())
        return;
    CThing* object = thing.GetPThing();
    bool added = false;
    CTCInventoryItem* item;
    if (!object->GetTC(TCI_INVENTORY_ITEM, &item))
        return;
    const CTCCarryable* carryable;
    if (!object->PeekTC(TCI_CARRYABLE, &carryable))
        return;
    if (item->PInventory == 0) {
        NInventory::CTCInventoryBase* inventory;
        if (!hero->GetTC(item->GetInventoryType(), &inventory))
            return;
        item->AddToInventory(*inventory, false, false, 0, true, e);
        added = true;
    }
    else if (item->GetInventoryOwner() != hero)
        return;
    if (item->PInventory == 0)
        return;
    if (makeActiveWeapon && object->HasTC(TCI_WEAPON))
        static_cast<CTCInventoryWeapons*>(item->PInventory)->SetThingAsActiveWeapon(object->GetDefGlobalIndex(), 0);
    if (added)
        object = item->PInventory->GetItemOfTypeInInventory(object->GetDefGlobalIndex());
    long slot = carryable->GetCarrySlotActive();
    CTCCarrying* carrying = 0;
    hero->GetTC(TCI_CARRYING, &carrying);
    if (!carrying->IsCarrySlotFree(slot))
        carrying->RemoveThingFromPrimarySlot(false);
    if (carrying->IsCarryingThing(*object))
        carrying->RemoveThing(*object);
    carrying->PutThingInCarrySlot(*object, slot, true);
    object->SetInLimbo(false);
}