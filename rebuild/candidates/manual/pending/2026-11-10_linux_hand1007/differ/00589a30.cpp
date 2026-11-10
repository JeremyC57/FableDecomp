// CTCInventory::OpenPCMapMenu() -- switch the PC inventory to the map screen: drop the old menu components, mark the map menu open, reuse an open PC_MAP_LIST component or build one through the hero's CTCInventoryMap (initialised and kept as a menu component), then hook its scroll buttons and its back / close clickables to the UI manager's mouse input and put it in state 5
// Grade: DIFFER(549v549) -- one swapped pair: after the inline `repe cmpsb` retail reloads esi (this, [esp+0x18]) before edi (i, [esp+0x14]); ours edi first. c2prio: i (candidate 5) is split piece #17 and this (candidate 10) #18 and the reloads follow piece order; inert: renames, loop index declared at the top, entry / count / index variants (probe/9a30_*.cpp)
// Leaf: ?OpenPCMapMenu@CTCInventory@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00589A30, 549 bytes, thiscall `ret`; size TU without a frame pointer (`optimize("s", on)`,
//   `sub esp,10h`). Identity: Ego_r.pdb / FableWin.pdb ?OpenPCMapMenu@CTCInventory@@QAEXXZ; the OpenInventoryShortcut 0x0058E99F
//   jump table sends screen 4 here; the "PC_MAP_LIST" literal 0x01250DC0.
// Callees: CTCInventory::DeleteMenuComponents 0x00587A41; NUISystem::CManager::GetInstance 0x0041E5F2 (the first result unused);
//   CComponent::GetName (slot 84) with the inline CCharString::operator==(const char*) (null arm `repe cmpsb` with "", else
//   CBasicString<char>::operator== 0x004115A0) and ~CCharString 0x0099EAE0; CThing::GetTC (bitset test 0x00410DE0, CVectorMap
//   LowerBound 0x0040F020) for TCI_INVENTORY_MAP 27; CTCInventoryMap::ConstructPCMapList(bool, bool*) 0x0060EEB0; CComponent::Initialise
//   (slot 43); vector<CComponent*>::push_back 0x00486024; CComponent::FindChild(CCharString) (slot 3) through CCharString(const char*,
//   long) 0x0099EBF0; CObserver::ObserveEvent (+4 base, slot 3) with UI_EVENT_MOUSE_MOVEMENT; CObservable::AddObserver (the manager's
//   slot 2); CComponent::ChangeState(unsigned long) (slot 48).
// Shape: the OpenPCOptionsMenu 0x0058AF10 spellings (landed): the find loop with `if (list) break;` first, the shared
//   FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ operator==, implicit CCharString conversions at FindChild.
// Inline-budget: FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (shared fable_string.h opt-in, as the landed 0x0058AF10) -- retail expands CCharString::operator==(const char*) in place at 0x00589A80..0x00589AB0
// Header: overlay FABLE_HDR_OVERLAY (FABLE_RETAIL_CTCINVENTORYMAP_PCMAPLIST).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_OPTIONSMENU
#define FABLE_RETAIL_CTCINVENTORYMAP_PCMAPLIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/CTCInventoryMap.h"

using NUISystem::CComponent;

void CTCInventory::OpenPCMapMenu()
{
    DeleteMenuComponents();
    CurrentOpenMenu = (CURRENT_OPEN_MENU)6;

    // reuse the map list if it is already one of the open menu components
    CComponent* list = 0;
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    for (unsigned long i = 0; i < Components.size(); ++i) {
        if (list)
            break;
        if (Components[i]->GetName() == "PC_MAP_LIST")
            list = Components[i];
    }

    if (!list) {
        CTCInventoryMap* map = 0;
        ParentThing->GetTC(TCI_INVENTORY_MAP, &map);
        list = map->ConstructPCMapList(false, 0);
        if (!list)
            return;
        list->Initialise();
        Components.push_back(list);
    }

    CComponent* scroll = list->FindChild("SCROLL_LIST_UP");
    if (scroll)
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    scroll = list->FindChild("SCROLL_DESCRIPTION_UP");
    if (scroll)
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    scroll = list->FindChild("SCROLL_LIST_DOWN");
    if (scroll)
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    scroll = list->FindChild("SCROLL_DESCRIPTION_DOWN");
    if (scroll)
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    CComponent* clickable = list->FindChild("PC_BACK_CLICKABLE");
    if (clickable) {
        NUISystem::CManager::GetInstance()->AddObserver(clickable);
        clickable->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }
    clickable = list->FindChild("PC_CLOSE_CLICKABLE");
    if (clickable) {
        NUISystem::CManager::GetInstance()->AddObserver(clickable);
        clickable->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }
    list->ChangeState(5);
}
