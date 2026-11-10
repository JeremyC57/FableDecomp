// CTCInventory::AssignThingPC(long thingID, unsigned long slot, NUISystem::CComponent* container) -- put a thing into a PC quick-access slot: record it, mark the slot's old contents for deletion, create a fresh assignment icon whose sprite shows the item's graphic, and give it the drop-nowhere actions (unassign the slot, then remove / clear-ability-or-item), the observe-mouse event and the drop action (use the ability or the item on this slot), then hand the icon to the container
// Grade: DIFFER(854v854) -- 34 difflines, swap-action arm layout only: retail emits the ability arm (SET_SPELL_TO_ASSIGN_IN_ITEMS_SWAPPING_PC 0x149, SLONG(ability->Ability)) after the slot push_back's init jmp and jumps back (deferred cold arm, cross-jump survivors = the slot push_back init + the ability arm's null path); ours lays it inline. Ability-arm-first (probe 4b86_sh4) gets the survivors but lays B first (52 lines). Micro-TU if/else, switch, &&, inversion, ternary and named-pointer spellings all lay it inline.
// Leaf: ?AssignThingPC@CTCInventory@@QAEXJKPAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00584B86, 854 bytes, thiscall `leave ; ret 0Ch`; frame-pointer size TU (optimize("y",off) +
//   ("s",on)), frame 0x34. Identity: Ego_r.pdb / FableWin.pdb ?AssignThingPC@CTCInventory@@QAEXJKPAVCComponent@NUISystem@@@Z; callers
//   AssignThingSwappingPC 0x00584EDC (landed).
// Callees: CTCInventory::SetThingAsQuickAccessItem 0x00584157; NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent
//   0x0041DB1D; CCharString 0x0099EBF0 / ~ 0x0099EAE0; CComponent FindChild (slot 3), AddChildDynamically (60), SetDeletion (62),
//   AddActionOnDropped (74), AddActionOnDroppedNowhere (75), CObserver::ObserveEvent (+4 base, slot 3); NUISystem::CDeletion 0x0042BED4 /
//   copy 0x0042CD84 / list dtor 0x0042ABCA; CCountedPointer<CComponent>(T*) 0x00429C15 / ReleaseData 0x004291DE; CTCInventoryItem::
//   GetEngineGraphic(long) 0x005D96A0 + CSprite::SetGraphicIndex (slot 145); operator new 0x00BFEA1A, CAction() 0x0042BE50,
//   CCountedPointer<CAction>(T*) 0x0042AA29 / Reset 0x005791E5 / ReleaseData 0x00429160; CAction::AddParam(CActionParam*,
//   CCountedPointer<CAction>&) 0x0053FDE0 / AddParam(CActionParam*) 0x0053FDB0; list<CActionParam*>::push_back 0x0042B646;
//   CActionParamULONG (vtable 0x0122F844) / CActionParamSLONG (0x012366F4); CTCBase::PeekWorld 0x00686A80; CDefinitionManager::
//   GetDef<CThingObjectDef> 0x0042B040; CDefClassBase::GetSubDef<CAbilityDef> 0x005936C2.
// Header: overlay FABLE_HDR_OVERLAY: engine/CTCInventory.h FABLE_RETAIL_CTCINVENTORY_ASSIGNPC (the two methods), new engine/CAbilityDef.h.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORY_ASSIGNPC
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_STATICENGINEGRAPHIC
#define FABLE_RETAIL_CENGINEGRAPHIC_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CDELETION_BODIES
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CWorld.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefPointer.h"
#include "engine/CThingObjectDef.h"
#include "engine/CAbilityDef.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamULONG.h"

using NUISystem::CComponent;
using NUISystem::CAction;
using NUISystem::CActionParamSLONG;
using NUISystem::CActionParamULONG;

// Same-TU bodies of the def lookups retail calls out of line (GetDef<CThingObjectDef> 0x0042B040, GetSubDef<CAbilityDef> 0x005936C2;
// the landed ConstructScreenPC 0x005B8743 spelling): VC7.1 then knows neither keeps the CDefPointer locals' addresses.
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
template <class T>
bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (!def.IsNull()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}
#pragma auto_inline(on)

void CTCInventory::AssignThingPC(long thingID, unsigned long slot, CComponent* container)
{
    long previous = QuickAccessItems[slot];
    SetThingAsQuickAccessItem(thingID, slot, true);
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    // the slot's old contents (an assigned icon, or the empty item) are deleted
    CComponent* old;
    if (previous == 0)
        old = container->FindChild("PC_EXPRESSION_ITEM_INSIDE_CONTAINER");
    else
        old = container->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER");
    if (old) {
        NUISystem::CDeletion deletion;
        deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
        old->SetDeletion(deletion);
    }
    CCountedPointer<CComponent> icon(manager->CreateComponent("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER", false));
    NUISystem::CSprite* sprite = static_cast<NUISystem::CSprite*>(icon->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE"));
    if (sprite)
        sprite->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(thingID).BankIndex);
    // dropped nowhere: unassign the slot and clear the spell / item to assign
    CCountedPointer<CAction> dropped(new CAction);
    dropped->ActionType = NUISystem::UI_ACTION_TYPE_UNASSIGN_EXPRESSION_ITEM_PC;
    dropped->AddParam(container, dropped);
    dropped->UIActionParams.push_back(new CActionParamULONG(slot));
    icon->AddActionOnDroppedNowhere(dropped);
    bool isAbility = false;
    CDefPointer<const CThingObjectDef> def;
    const CWorld& world = PeekWorld();
    world.DefinitionManager->GetDef(thingID, &def);
    CDefPointer<const CAbilityDef> ability;
    if (!def.IsNull() && def->GetSubDef(&ability)) {
        isAbility = true;
        dropped.Reset(new CAction);
        dropped->ActionType = NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC;
        dropped->UIActionParams.push_back(new CActionParamSLONG(0));
    } else {
        dropped.Reset(new CAction);
        dropped->ActionType = NUISystem::UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_PC;
        dropped->UIActionParams.push_back(new CActionParamSLONG(0));
    }
    icon->AddActionOnDroppedNowhere(dropped);
    icon->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    // dropped on another slot: swap this spell / item into it
    CCountedPointer<CAction> swap(new CAction);
    if (!isAbility) {
        swap->ActionType = NUISystem::UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC;
        swap->UIActionParams.push_back(new CActionParamSLONG(thingID));
    } else {
        swap->ActionType = NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_IN_ITEMS_SWAPPING_PC;
        swap->UIActionParams.push_back(new CActionParamSLONG(ability->Ability));
    }
    swap->UIActionParams.push_back(new CActionParamSLONG(slot));
    swap->AddParam(container);
    icon->AddActionOnDropped(swap);
    container->AddChildDynamically(icon);
}
