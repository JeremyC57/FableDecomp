// CTCInventoryTrade::AddSellItemToList(long defIndex, long count, NUISystem::CList* list, NUISystem::CViewport* viewport, bool addToTop) -- build the sell-menu entry for one item (a branch entry when the shop holds a default stock item for the def, else a plain entry that is cleared), record it in ItemsToSell, wire its seven actions and add it to the list; returns whether it is a branch entry
// Leaf: ?AddSellItemToList@CTCInventoryTrade@@QAE_NJJPAVCList@NUISystem@@PAVCViewport@3@_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059ED42, 1474 bytes, `ret 14h`, bool in al. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CManager::GetInstance 0x0041E5F2, CTCInventoryBase::GetTradingThing 0x005BC4C4 (catalog
//   "CWorld::DrawGetEnvironment" is propagated), std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (the inline
//   GetInterface, type 0x53 = CTCShop), CTCShop::IsValidStockItem(long) 0x007723D0 (catalog "_atexit" is propagated; it is
//   `FindDefaultStockItem(def) != 0`, 0x00772250), CManager::CreateMenuEntryComponent 0x00428494, CCountedPointer<CComponent>
//   Reset 0x00429C6F / dtor 0x004291DE, CWideString 0x0099B6B0 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   ItemsToSell insert(const value_type&) 0x005B2872 (pair<iterator,bool> by hidden pointer), operator new 0x00BFEA1A,
//   CAction 0x0042BE50, CCountedPointer<CAction> 0x0042AA29 / ~ 0x00429160, param-list push_back 0x0042B646 (same-TU
//   noinline stand-in), CAction::AddParam 0x0053FDE0, CActionParamString 0x0054DAE0.
// Shape: the landed AddWantedItemToList 0x0059CB90 model (same TU): each `new CActionParamLong` is an rvalue bound to the
//   push_back const reference, so VC parks it in a dead frame word (the `this` spill, then the dead viewport / defIndex
//   argument slots); the vtable 0x012366F4 sits in ebx. The bool result is the byte [ebp-1] set before GetInstance.
// Layout: retail CTCInventoryTrade ItemsToSell +0x150 (map<CComponent*, pair<long, long> >, PDB +0x178); engine/CTCInventoryTrade.h
//   is the flattened PDB layout (16-byte containers), so the class is modelled locally (landed ctor 0x0059EC48 offsets).
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (engine/CCountedPointer.h).
// NF51 (round 7): ported onto the shared headers with the same-TU AddWantedItemToList 0x0059CB90 (landed) as the model (were local
// CTCShop / CActionParam / CActionParamLong / CActionParamString / CAction / CComponent / CList / CManager / CTCInventoryTrade /
// map<CComponent*, pair<long,long> > models and a same-TU push_back stand-in body). The parameters are NUISystem::CActionParamSLONG
// (vtable 0x012366F4, ??_7CActionParamSLONG@NUISystem@@6B@; was a local CActionParamLong); 0x005BC4C4 is
// ?GetBuildingThing@CTCInventoryBase@NInventory@@QBEPAVCThing@@XZ (was GetTradingThing); the component slots are the header's: 67
// AddActionOnSelected, 69 AddPreAction, 92 MakeUnavailable, CMenuEntry 151 AddUIActionParam / 152 AddActionOnUnselected / 153
// AddActionOnVisible, CList 152 AddScrollingChildAndSetOffsets / 153 AddScrollingChildDynamicallyAndSetOffsets (were descriptive
// AddSelectedAction / AddSecondaryAction / Clear / SetUserParam / AddFocusAction / AddUnfocusAction / AddEntry / AddEntryAtTop); ItemsToSell
// is the header's map (+0x150).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_WANTEDLIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCSHOP_VALIDSTOCK
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CMENUENTRY_UNSELECTED_VISIBLE
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCShop.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamString.h"
using namespace NUISystem;

bool CTCInventoryTrade::AddSellItemToList(long defIndex, long count, CList* list, CViewport* viewport, bool addToTop)
{
    bool isBranch = true;
    CManager* fe = CManager::GetInstance();
    CCountedPointer<CComponent> entry;
    CTCShop* shop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &shop);
    if (shop->IsValidStockItem(defIndex)) {
        entry.Reset(fe->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",
                                                 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    } else {
        isBranch = false;
        entry.Reset(fe->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",
                                                 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
        entry->MakeUnavailable();
    }
    ItemsToSell.insert(std::map<CComponent*, std::pair<long, long> >::value_type(entry.Get(), std::pair<long, long>(defIndex, count)));
    CMenuEntry* menuEntry = static_cast<CMenuEntry*>(entry.Get());
    CCountedPointer<CAction> addModel(new CAction);
    addModel->ActionType = UI_ACTION_ADD_MESH_CHILD;
    addModel->UIActionParams.push_back(new CActionParamSLONG(defIndex));
    addModel->AddParam(entry.Get(), addModel);
    CWideString model(L"UI_ITEM_MODEL_TEMPLATE_NEW");
    addModel->UIActionParams.push_back(new CActionParamString(model));
    entry->AddActionOnSelected(addModel);
    CCountedPointer<CAction> removeModel(new CAction);
    removeModel->ActionType = UI_ACTION_REMOVE_MESH_CHILD;
    removeModel->AddParam(entry.Get(), removeModel);
    menuEntry->AddActionOnUnselected(removeModel);
    CCountedPointer<CAction> loadDesc(new CAction);
    loadDesc->ActionType = UI_ACTION_LOAD_SELL_ITEM_DESC;
    loadDesc->UIActionParams.push_back(new CActionParamSLONG(defIndex));
    loadDesc->AddParam(viewport, loadDesc);
    loadDesc->AddParam(entry.Get(), loadDesc);
    entry->AddActionOnSelected(loadDesc);
    CCountedPointer<CAction> unloadDesc(new CAction);
    unloadDesc->ActionType = UI_ACTION_UNLOAD_SELL_ITEM_DESC;
    unloadDesc->AddParam(entry.Get(), unloadDesc);
    menuEntry->AddActionOnUnselected(unloadDesc);
    CCountedPointer<CAction> loadMenu(new CAction);
    loadMenu->ActionType = UI_ACTION_LOAD_SELL_ACTION_MENU;
    loadMenu->UIActionParams.push_back(new CActionParamSLONG(defIndex));
    loadMenu->AddParam(entry.Get(), loadMenu);
    entry->AddPreAction(loadMenu);
    CCountedPointer<CAction> unloadMenu(new CAction);
    unloadMenu->ActionType = UI_ACTION_UNLOAD_SELL_ACTION_MENU;
    unloadMenu->AddParam(entry.Get(), unloadMenu);
    menuEntry->AddActionOnUnselected(unloadMenu);
    CCountedPointer<CAction> loadName(new CAction);
    loadName->ActionType = UI_ACTION_LOAD_ITEM_MENU_ENTRY_NAME;
    loadName->UIActionParams.push_back(new CActionParamSLONG(defIndex));
    loadName->AddParam(entry.Get(), loadName);
    loadName->UIActionParams.push_back(new CActionParamSLONG(count));
    menuEntry->AddActionOnVisible(loadName);
    menuEntry->AddUIActionParam(entry.Get());
    if (addToTop)
        list->AddScrollingChildDynamicallyAndSetOffsets(entry);
    else
        list->AddScrollingChildAndSetOffsets(entry);
    return isBranch;
}
