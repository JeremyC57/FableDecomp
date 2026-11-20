// CTCInventory::OpenPCMapMenu() -- switch the PC inventory to the map screen: drop the old menu components, mark the map menu open, reuse an open PC_MAP_LIST component or build one through the hero's CTCInventoryMap (initialised and kept as a menu component), then hook its scroll buttons and its back / close clickables to the UI manager's mouse input and put it in state 5
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
// Re-author (followup, 2026-11-20): the landed text went DIFFER(549v549) on ce3b25d5d (GFDebugPDynamicCast moved from CThing.h to its
//   own header; the change renumbers c2's symbols, and the reload order after the inline `repe cmpsb` flipped: edi before esi at
//   0x00589A97). Rewritten in Ego_d.pdb's text (lib: lines 7024..7092 of the CTCInventory TU; locals pmanager, pmap_list, the for-scope
//   `unsigned long count`, the block-scoped ptc_map and pscroll_up / pscroll_down / pback / pclose), which matches on today's headers:
//   - the search loop's condition carries `&& !pmap_list` (line 7032, 48 bytes; no separate break line before 7034);
//   - `if (pmap_list) { Initialise; push_back; }` inside the build arm (7045 6 bytes, 7047 14, 7048 15) and the whole FindChild part
//     in a second `if (pmap_list) { ... }` (7052, 10 bytes = cmp + rel32 jump; the four pointers are that block's locals in Ego_d);
//     c2 threads the null case of the first test straight to the epilogue (retail 0x00589B29 `je 0x00589C4D`);
//   - two scroll locals, each reused for the LIST and the DESCRIPTION button (Ego_d has four child locals for six FindChild calls:
//     7054 / 7059 store pscroll_up, 7065 / 7070 pscroll_down).
//   The `if (list) break;` loop form also matches with these locals (measured); Ego_d's line table decides for the condition form.
// Inline-budget: FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (shared fable_string.h opt-in, as the landed 0x0058AF10) -- retail expands
//   CCharString::operator==(const char*) in place at 0x00589A80..0x00589AB0
// Header: shared rebuild/include only (FABLE_RETAIL_CTCINVENTORYMAP_PCMAPLIST, committed by chunk 1007).
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
    CComponent* pmap_list = 0;
    NUISystem::CManager* pmanager = NUISystem::CManager::GetInstance();
    for (unsigned long count = 0; count < Components.size() && !pmap_list; ++count)
    {
        if (Components[count]->GetName() == "PC_MAP_LIST")
            pmap_list = Components[count];
    }
    if (!pmap_list)
    {
        CTCInventoryMap* ptc_map = 0;
        ParentThing->GetTC(TCI_INVENTORY_MAP, &ptc_map);
        pmap_list = ptc_map->ConstructPCMapList(false, 0);
        if (pmap_list)
        {
            pmap_list->Initialise();
            Components.push_back(pmap_list);
        }
    }
    if (pmap_list)
    {
        CComponent* pscroll_up = pmap_list->FindChild("SCROLL_LIST_UP");
        if (pscroll_up)
            pscroll_up->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        pscroll_up = pmap_list->FindChild("SCROLL_DESCRIPTION_UP");
        if (pscroll_up)
            pscroll_up->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        CComponent* pscroll_down = pmap_list->FindChild("SCROLL_LIST_DOWN");
        if (pscroll_down)
            pscroll_down->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        pscroll_down = pmap_list->FindChild("SCROLL_DESCRIPTION_DOWN");
        if (pscroll_down)
            pscroll_down->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        CComponent* pback = pmap_list->FindChild("PC_BACK_CLICKABLE");
        if (pback)
        {
            NUISystem::CManager::GetInstance()->AddObserver(pback);
            pback->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        }
        CComponent* pclose = pmap_list->FindChild("PC_CLOSE_CLICKABLE");
        if (pclose)
        {
            NUISystem::CManager::GetInstance()->AddObserver(pclose);
            pclose->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
        }
        pmap_list->ChangeState(5);
    }
}