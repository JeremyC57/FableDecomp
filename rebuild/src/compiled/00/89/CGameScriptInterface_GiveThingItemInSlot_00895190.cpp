// CGameScriptInterface::GiveThingItemInSlot(const CScriptThing& thing, const CCharString& itemDefName, const CCharString& slotName) const -- with an item def name: create that object at the thing, clear the named carry slot (the primary slot through RemoveThingFromPrimarySlot; any other by removing its thing and putting an inventory item no longer carried into limbo) and put the new item there; with an empty name: take the thing out of that slot and kill it
// Leaf: ?GiveThingItemInSlot@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00895190, 525 bytes (+ int3 padding), thiscall `ret 0Ch`. Identity: retail
//   CGameScriptInterface vtable 0x01260F0C (engine/CGameScriptInterface.h); Ego_r.pdb / FableWin.pdb
//   ?GiveThingItemInSlot@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@1@Z.
// Callees (PDB spellings): CScriptThing slot 11 GetPThing, CCharString::operator==(const char*) (inline; CBasicString<char>::operator==
//   0x004115A0), CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) const 0x009AD410 (DefinitionManager +0x10),
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020, CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0,
//   CThing::GetPos() const 0x004C73D0, CThingObject::Create(long, const C3DVector&, long, const CRightHandedSet*, unsigned long,
//   const CCharString&) 0x00703210 (static fastcall; landed rows bind it as a free CreateThingObject: NAME_FIXES), CTCCarrying::
//   IsCarrySlotFree 0x006A1230 / IsSlotPrimarySlot 0x006A1330 / RemoveThingFromPrimarySlot 0x006A1E00 / GetThingInCarrySlot
//   0x006A11F0 / RemoveThing 0x006A19B0 / IsCarryingThing 0x006A1060 / RemoveThingInCarrySlot 0x006A16A0 / PutThingInCarrySlot
//   0x006A1F80, CThing::SetInLimbo 0x004C8CF0 / Kill 0x004C9B80.
// Model: shared headers only (overlay opt-ins CTCCARRYING_SLOTFREE / _SLOTACCESS). The prior local-model draft (2026-10-22 hand287)
//   parked at DIFFER(520v525) on `this` in ebp v a frame word; on the shared CThing GetTC body and the real class models it matches
//   as written (the empty-name branch first in source: VC7.1 lays it after the creation path, 0x008952A0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCCARRYING_PUTTHING
#define FABLE_RETAIL_CTCCARRYING_REMOVETHING
#define FABLE_RETAIL_CTCCARRYING_CARRYSLOT
#define FABLE_RETAIL_CTCCARRYING_SLOTFREE
#define FABLE_RETAIL_CTCCARRYING_SLOTACCESS
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
#include "engine/CThingObject.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCCarrying.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"

void CGameScriptInterface::GiveThingItemInSlot(const CScriptThing& thing, const CCharString& itemDefName, const CCharString& slotName) const
{
    CThing* owner = thing.GetPThing();
    if (!owner)
        return;
    if (itemDefName == "") {
        CTCCarrying* carrying;
        if (!owner->GetTC(TCI_CARRYING, &carrying))
            return;
        long slot = DefinitionManager->GetDefGlobalIndexFromName(slotName);
        if (slot <= 0)
            return;
        CThing* old = carrying->RemoveThingInCarrySlot(slot, false);
        if (old && !old->IsDead())
            old->Kill(false);
        return;
    }
    long defIndex = DefinitionManager->GetDefGlobalIndexFromName(itemDefName);
    if (defIndex <= 0)
        return;
    CTCCarrying* carrying;
    if (!owner->GetTC(TCI_CARRYING, &carrying))
        return;
    CThing* item = CThingObject::Create(defIndex, owner->GetPos(), owner->GetPlayerNumber(), 0, 0, "");
    long slot = DefinitionManager->GetDefGlobalIndexFromName(slotName);
    if (slot <= 0)
        return;
    if (!carrying->IsCarrySlotFree(slot)) {
        if (carrying->IsSlotPrimarySlot(slot))
            carrying->RemoveThingFromPrimarySlot(false);
        else {
            CThing* old = carrying->GetThingInCarrySlot(slot);
            carrying->RemoveThing(*old);
            CTCInventoryItem* oldItem;
            if (!carrying->IsCarryingThing(*old) && old->GetTC(TCI_INVENTORY_ITEM, &oldItem) && oldItem->PInventory)
                old->SetInLimbo(true);
        }
    }
    carrying->PutThingInCarrySlot(*item, slot, true);
}