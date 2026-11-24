// CTCInventory::AssignThingPC(long, unsigned long, NUISystem::CComponent*) -- puts a thing into a PC quick-access slot: records it (SetThingAsQuickAccessItem), marks the container's old contents (the empty item, or the previous assignment icon) for deletion, creates a fresh assignment icon showing the thing's graphic, and gives it its actions: dropped nowhere = unassign this slot; dropped nowhere again = re-arm the spell / expression-item assignment; dropped on another slot = assign swapping (thing or spell's ability, and this slot); the icon watches mouse movement and becomes the container's contents
// Leaf: ?AssignThingPC@CTCInventory@@QAEXJKPAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00584B86, 854 bytes, thiscall `leave ; ret 0Ch`; frame-pointer size TU (optimize("y",off) +
//   ("s",on)), frame 0x34. Identity: Ego_r.pdb / FableWin.pdb ?AssignThingPC@CTCInventory@@QAEXJKPAVCComponent@NUISystem@@@Z; the
//   landed AssignThingSwappingPC 0x00584EDC calls it twice.
// Callees: CTCInventory::SetThingAsQuickAccessItem 0x00584157; NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D;
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0; CComponent FindChild (slot 3), AddChildDynamically (60), SetDeletion (62), AddActionOnDropped
//   (74), AddActionOnDroppedNowhere (75), CObserver::ObserveEvent (slot 3 of the +4 base); CDeletion 0x0042BED4 / copy 0x0042CD84 /
//   list dtor 0x0042ABCA; CTCInventoryItem::GetEngineGraphic(long) 0x005D96A0 (static __fastcall, by value); CSprite::SetGraphicIndex
//   (slot 145); operator new 0x00BFEA1A, CAction() 0x0042BE50, CCountedPointer<CAction>(T*) 0x0042AA29 / Reset 0x005791E5 / ReleaseData
//   0x00429160, CCountedPointer<CComponent>(T*) 0x00429C15 / ReleaseData 0x004291DE; CAction::AddParam 0x0053FDE0 (with the action) /
//   0x0053FDB0; list<CActionParam*>::push_back 0x0042B646; inline CActionParamULONG (vtable 0x0122F844) / CActionParamSLONG (0x012366F4);
//   CTCBase::PeekWorld 0x00686A80, CWorld::PeekDefinitionManager (inline, +0x10); CDefinitionManager::GetDef<CThingObjectDef> 0x0042B040
//   and CDefClassBase::GetSubDef<CAbilityDef> 0x005936C2 (bodies visible, noinline: header opt-ins *_BODY).
// Shape notes: the old contents' lookup is an if/else of two FindChild calls -- VC7.1 cross-jumps the arms into one call with the
//   name pushed in the arms (a `?:` on the two literals is a register select). The visible GetDef / GetSubDef bodies keep the two def
//   handles' addresses from escaping: their dead `Object = 0` stores vanish and `this` shares its spill slot with the ability handle
//   (frame 0x34, as retail). The swap action's slot parameter is pushed in both arms; the spell arm's tail then cross-jumps into the
//   thing arm (retail's block order), where one shared push after the if/else lays the arms out consecutively.
// Header: overlay FABLE_RETAIL_CTCINVENTORY_ASSIGNTHINGPC (hdr/engine/CTCInventory.h: the two declarations). CAbilityDef has no shared
//   header (only _quarantine/): a local class on the shared CSubDefClassBase with the Ego_r.pdb member Ability at +0x28 and the inline
//   StatGetTypeName of the landed 0x005936C2.
// Locals: previous, manager, old, deletion, icon, sprite, action, isSpell, def, ability, swap are descriptive names.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORY_ASSIGNTHINGPC
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_STATICENGINEGRAPHIC
#define FABLE_RETAIL_CENGINEGRAPHIC_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CDELETION_BODIES
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CSubDefClassBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryItem.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamULONG.h"
#include "engine/CWorld.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingObjectDef.h"
#include "engine/EHeroAbility.h"

using NUISystem::CComponent;
using NUISystem::CAction;
using NUISystem::CActionParamSLONG;
using NUISystem::CActionParamULONG;

// CAbilityDef has no shared header: RTTI .?AVCAbilityDef@@ : CSubDefClassBase (sizeof 0x25), Ego_r.pdb member Ability +0x28.
class CAbilityDef : public CSubDefClassBase {
public:
    static CCharString FABLE_FASTCALL StatGetTypeName() { return CCharString("CAbilityDef", -1); }   // ?StatGetTypeName@CAbilityDef@@SI?AVCCharString@@XZ (inline)
    EHeroAbility Ability;                                            // +0x28
};

void CTCInventory::AssignThingPC(long thingIndex, unsigned long slot, CComponent* container)
{
    long previous = QuickAccessItems[slot];
    SetThingAsQuickAccessItem(thingIndex, slot, true);
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
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
        sprite->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(thingIndex).BankIndex);
    CCountedPointer<CAction> action(new CAction);
    action->ActionType = NUISystem::UI_ACTION_TYPE_UNASSIGN_EXPRESSION_ITEM_PC;
    action->AddParam(container, action);
    action->UIActionParams.push_back(new CActionParamULONG(slot));
    icon->AddActionOnDroppedNowhere(action);
    bool isSpell = false;
    CDefPointer<const CThingObjectDef> def;
    PeekWorld().PeekDefinitionManager().GetDef(thingIndex, &def);
    CDefPointer<const CAbilityDef> ability;
    if (!def.IsNull() && def->GetSubDef(&ability)) {
        isSpell = true;
        action.Reset(new CAction);
        action->ActionType = NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC;
        action->UIActionParams.push_back(new CActionParamSLONG(0));
    } else {
        action.Reset(new CAction);
        action->ActionType = NUISystem::UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_PC;
        action->UIActionParams.push_back(new CActionParamSLONG(0));
    }
    icon->AddActionOnDroppedNowhere(action);
    icon->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    CCountedPointer<CAction> swap(new CAction);
    if (!isSpell) {
        swap->ActionType = NUISystem::UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC;
        swap->UIActionParams.push_back(new CActionParamSLONG(thingIndex));
        swap->UIActionParams.push_back(new CActionParamSLONG(slot));
    } else {
        swap->ActionType = NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_IN_ITEMS_SWAPPING_PC;
        swap->UIActionParams.push_back(new CActionParamSLONG(ability->Ability));
        swap->UIActionParams.push_back(new CActionParamSLONG(slot));
    }
    swap->AddParam(container);
    icon->AddActionOnDropped(swap);
    container->AddChildDynamically(icon);
}