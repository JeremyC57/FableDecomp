// CTCQuickAccessMenu::ContructItemLists(std::map<long,long>& items, NUISystem::CList* list, NUISystem::CComponent* owner, CCountedPointer<NUISystem::CComponent>& actions_list) -- the quick-access items list: for every (def index, count) of the merged item map a menu entry (the plain or branch template by the item's use-button action) registered in the inventory's ItemsMenuEntries, the assigned quick-access icon (left / right / up / down, recorded in AssignedIcons) when the item sits in a slot, a LOAD_ITEM_MENU_ENTRY_NAME action (def index, entry, count) shown when visible, the def index as the entry's thing parameter, the entry added to the list, and the shared actions list as a child of usable items
// Leaf: ?ContructItemLists@CTCQuickAccessMenu@@AAEXAAV?$map@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@PAVCList@NUISystem@@PAVCComponent@5@AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Flags: /d2inlT59
// Inline-budget: /d2inlT59 -- the four quick-access icon arms (left / right / up / down) of the speed TU: retail expands the
//   CCountedPointer<CComponent>(T*) constructor only in the LEFT arm and calls 0x00429C15 in the other three, and calls the destructor
//   0x00429C53 only in the DOWN arm. c2 inline trace (c2.dll 0x1078D1AA, tools/inl): ctor c1 70 at f 2.5 / 1.25 / 0.625 / 0.3125 ->
//   scores 56 / 65 / 76 / 92, dtor c1 47 -> 37 / 43 / 51 / 61; retail needs 65 and 61 called with 56 and the bitset test (58, f 1.0)
//   expanded: T in 59..61 under the default 74. Loop shapes tried first: a `while` loop (body 9x: everything expanded), a guarded
//   do-while (same as `for`); a folded `&& true` in the loop condition (body 2.5x) gives the same split and also matches.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00626110, 1596 bytes, thiscall `ret 10h` at 0x00626749 (then int3 padding to 0x00626750), speed TU
//   (`sub esp,68h`, no frame pointer, 16-aligned with int3 padding before).
// Identity (two sources): Ego_r.pdb 0x0066A9F0 / FableWin.pdb 0x0211A640 ?ContructItemLists@CTCQuickAccessMenu@@AAEXAAV?$map@JJ...@Z
//   (private, sic "Contruct"; the map overload, laid before the vector one in both PDBs as in retail); its only caller 0x006272FD is
//   the landed ConstructItemCategoryList 0x00626E40, which passes its std::map<long,long> thingmap. The same body as the landed
//   CTCInventory::ConstructItemEntry 0x0058A0C7 minus the mesh / description actions (here inlined into the map walk).
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 / CreateMenuEntryComponent 0x00428494 / CreateComponent 0x0041DB1D;
//   CThing::GetTC<CTCInventory> (inline: the interface bitset bit 17 at +0x20, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//   0x0040F020); map<long, CComponent*>::operator[] 0x005949C3 (CTCInventory::ItemsMenuEntries +0x1FC / AssignedIcons +0x208);
//   CCountedPointer<CComponent> (T*) 0x00429C15 (expanded at the LEFT site) / Reset 0x00429C6F / ~ 0x00429C53 (the DOWN icon) and the
//   expanded release; CCountedPointer<CAction> expanded (deleter 0x00429F43), CAction() expanded (malloc 0x00BFEA0E for the two
//   STLport list heads), CActionParamSLONG expanded (vtable 0x012366F4), list<CActionParam*>::push_back expanded,
//   CAction::AddParam(CActionParam*, CCountedPointer<CAction>&) 0x0053FDE0, operator new 0x00BFEA1A / delete 0x00BFE9BC;
//   CWideString(const unsigned short*) 0x0099B6B0 / ~ 0x0099B510, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0;
//   CComponent slot 59 AddChild (+0xEC), CMenuEntry slots 150 AddThingActionParam (+0x258) / 153 AddActionOnVisible (+0x264),
//   CList slot 152 AddScrollingChildAndSetOffsets (+0x260).
// Shape (chunk 2013): shared headers only (engine/CTCInventory.h QUICKACCESS members, CMenuEntry ACTIONLISTS, CActionParamSLONG); the
//   hdr/ overlay opt-in FABLE_RETAIL_CACTION_INLINE_CTOR makes the CAction constructor visible so the speed TU expands it as retail.
//   `CTCInventory* inventory = 0; ParentThing->GetTC(&inventory);` before the loop (FABLE_RETAIL_CTHING_GETTC_EXPANDED: the slot
//   stays 0 when the bit is clear).
//   The map walk caches end() (retail [esp+44h]; the items parameter slot is reused for the def index) and
//   expands _Rb_global::_M_increment; the entry is default-constructed and Reset (called) in either template arm.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_USEBUTTONACTION
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CMENUENTRY_ACTIONLISTS
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTION_INLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/ETCInterfaceType.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryItem.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/CTCQuickAccessMenu.h"

void CTCQuickAccessMenu::ContructItemLists(std::map<long, long>& items, NUISystem::CList* list, NUISystem::CComponent* owner,
                                           CCountedPointer<NUISystem::CComponent>& actions_list)
{
    using namespace NUISystem;
    CManager* manager = CManager::GetInstance();
    CTCInventory* inventory = 0;
    ParentThing->GetTC(&inventory);
    for (std::map<long, long>::iterator it = items.begin(), end = items.end(); it != end; ++it) {
        long def_index = (*it).first;
        long count = (*it).second;
        CCountedPointer<CComponent> entry;
        if (CTCInventoryItem::GetUseButtonActionType(def_index) == 0)
            entry.Reset(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE", 0.0f, "", 0.0f,
                                                          "PC_BUTTON_BASTARD"));
        else
            entry.Reset(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_FOR_QUICK_ACCESS",
                                                          0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        inventory->ItemsMenuEntries[def_index] = entry.Get();
        if (inventory->QuickAccessItems[2] == def_index) {
            CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_LEFT_ITEM_ICON", false));
            entry->AddChild(icon);
            inventory->AssignedIcons[2] = icon.Get();
        } else if (inventory->QuickAccessItems[3] == def_index) {
            CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_RIGHT_ITEM_ICON", false));
            entry->AddChild(icon);
            inventory->AssignedIcons[3] = icon.Get();
        } else if (inventory->QuickAccessItems[0] == def_index) {
            CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_UP_ITEM_ICON", false));
            entry->AddChild(icon);
            inventory->AssignedIcons[0] = icon.Get();
        } else if (inventory->QuickAccessItems[1] == def_index) {
            CCountedPointer<CComponent> icon(manager->CreateComponent("UI_ASSIGNED_DOWN_ITEM_ICON", false));
            entry->AddChild(icon);
            inventory->AssignedIcons[1] = icon.Get();
        }
        CMenuEntry* menu_entry = static_cast<CMenuEntry*>(entry.Get());
        CCountedPointer<CAction> load_name(new CAction);
        load_name->ActionType = UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME;
        load_name->UIActionParams.push_back(new CActionParamSLONG(def_index));
        load_name->AddParam(entry.Get(), load_name);
        load_name->UIActionParams.push_back(new CActionParamSLONG(count));
        menu_entry->AddActionOnVisible(load_name);
        menu_entry->AddThingActionParam(def_index);
        list->AddScrollingChildAndSetOffsets(entry);
        if (CTCInventoryItem::GetUseButtonActionType(def_index))
            entry->AddChild(actions_list);
    }
}
