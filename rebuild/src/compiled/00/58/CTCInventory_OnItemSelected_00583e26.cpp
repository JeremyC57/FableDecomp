// CTCInventory::OnItemSelected(long itemDefIndex, bool& handled) -- the hero picked an inventory item: create its object out of limbo and, by the item's use-button action, use it through its carried action-use component (action 1: only when the hero is idle or the component may be used mid-action, then `handled`; else it goes back to the inventory) or (action 2) use it the same way and re-highlight the current item when the use killed the object, `handled` false
// Leaf: ?OnItemSelected@CTCInventory@@UAEXJAA_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00583E26, 484 bytes, thiscall `leave ; ret 8`; frame-pointer size TU (optimize("y",off) + ("s",on)),
//   two frame words (ParentThing [ebp-4], `this` [ebp-8]). Identity: CTCInventory vtable 0x01250DE4 slot 33 (the CTCInventoryBase pure
//   virtual), Ego_r.pdb / FableWin.pdb ?OnItemSelected@CTCInventory@@UAEXJAA_N@Z; the landed UseQuickAccessItem 0x005840A6 calls it.
// Callees: CTCInventoryItem::GetUseButtonActionType 0x005D9710 (FableWin.pdb static fastcall, donor order between GetUIInventoryCategory
//   0x005D96F0 and GetStartingRightHandedSet 0x005D9730; reads CInventoryItemDef::UseButtonAction +0x4C), CTCInventoryBase::CreateInventoryObject
//   0x005BE8F3, IS_THING_ALIVE<CThing*> 0x0040DCE0, CThing::SetInLimbo 0x004C8CF0, bitset<274>::test 0x00410DE0, CVectorMap LowerBound
//   0x0040F020, CThingCreatureBase::IsPerformingAnyAction 0x006622A0, CTCInventoryItem::ReturnToInventory 0x005D8E50,
//   CTCInventoryBase::GetHighlightedItem 0x005BCAE3 (protected const); virtuals: the carried action-use component's slot 28 OnActionUse
//   (+0x70) and slot 30 IsUsableWhileThingPerformingAction (+0x78), this class's slot 36 OnItemHighlighted (+0x90).
// Shape: the visible IS_THING_ALIVE<CThing*> body (landed 0x005CB2B0 device) homes the created thing's copy and every interface key in the
//   dead `itemDefIndex` slot [ebp+8].
// Header: overlay FABLE_HDR_OVERLAY (FABLE_RETAIL_CTCINVENTORY_ITEMSELECTED, FABLE_RETAIL_CTCINVENTORYBASE_HIGHLIGHTEDITEM,
//   FABLE_RETAIL_CTCINVENTORYITEM_USEBUTTONACTION).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_CREATEOBJECT
#define FABLE_RETAIL_CTCINVENTORYBASE_HIGHLIGHTEDITEM
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_ITEMSELECTED
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_USEBUTTONACTION
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryItem.h"

// The carried action-use component (interface 0x0E, retail vtable 0x0123F894) as this row calls it: slot 28 OnActionUse(CThing&)
// (the CTCActionUseBase virtual; `ret 4` in the base, 0x007EAB90) and slot 30 IsUsableWhileThingPerformingAction (Ego_r.pdb
// ?IsUsableWhileThingPerformingAction@CTCActionUseBase@@UBE_NXZ), as the landed OnItemSelectedInHand 0x0058400A models it.
class CTCCarriedActionUseBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual void OnActionUse(CThing& user);                    // slot 28 (+0x70)
    virtual void v29();
    virtual bool IsUsableWhileThingPerformingAction() const;   // slot 30 (+0x78)
};

template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);   // 0x0040DCE0 (CThing*)
// Same-TU body of IS_THING_ALIVE<CThing*> (out of line in retail: 0x0040DCE0): VC7.1 sees that it does not keep the reference, so the
// created thing stays in edi and is passed through a copy in the dead argument slot (`mov [ebp+8],edi ; lea ecx,[ebp+8]`).
#pragma auto_inline(off)
template <> inline bool FABLE_FASTCALL IS_THING_ALIVE<CThing*>(CThing* const& thing)
{
    if (thing != 0 && thing->IsDead() == 0)
        return true;
    return false;
}
#pragma auto_inline(on)

void CTCInventory::OnItemSelected(long itemDefIndex, bool& handled)
{
    CThingCreatureBase* hero = static_cast<CThingCreatureBase*>(ParentThing);
    switch (CTCInventoryItem::GetUseButtonActionType(itemDefIndex)) {
    case 1: {
        CThing* object = CreateInventoryObject(itemDefIndex);
        if (IS_THING_ALIVE(object)) {
            object->SetInLimbo(true);
            CTCCarriedActionUseBase* use;
            if (object->GetTC(TCI_CARRIED_ON_ACTION_USE, &use)) {
                if (!hero->IsPerformingAnyAction() || use->IsUsableWhileThingPerformingAction()) {
                    handled = true;
                    use->OnActionUse(*ParentThing);
                } else {
                    CTCInventoryItem* item = 0;
                    object->GetTC(TCI_INVENTORY_ITEM, &item);
                    item->ReturnToInventory();
                }
            } else {
                CTCInventoryItem* item = 0;
                object->GetTC(TCI_INVENTORY_ITEM, &item);
                item->ReturnToInventory();
            }
        }
        break;
    }
    case 2: {
        CThing* object = CreateInventoryObject(itemDefIndex);
        if (IS_THING_ALIVE(object)) {
            object->SetInLimbo(true);
            CTCCarriedActionUseBase* use = 0;
            object->GetTC(TCI_CARRIED_ON_ACTION_USE, &use);
            if (!hero->IsPerformingAnyAction() || use->IsUsableWhileThingPerformingAction()) {
                use->OnActionUse(*ParentThing);
                if (object->IsDead())
                    OnItemHighlighted(GetHighlightedItem());
            } else {
                CTCInventoryItem* item = 0;
                object->GetTC(TCI_INVENTORY_ITEM, &item);
                item->ReturnToInventory();
            }
        }
        handled = false;
        break;
    }
    }
}