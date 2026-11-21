// CTCInventoryTrade::GetMenuBranchFromDefIndexPC(long defIndex, NUISystem::CComponent* menu, EItem_Menu_Type& type) -- picks the trade menu branch for an item def by its inventory category and reports the tab type (MENU_TYPE_WEAPONS 1, MENU_TYPE_CLOTHING 0, MENU_TYPE_ITEMS 2)
// Leaf: ?GetMenuBranchFromDefIndexPC@CTCInventoryTrade@@AAEPAVCComponent@NUISystem@@JPAV23@AAW4EItem_Menu_Type@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059C0C6, 897 bytes, size-optimised TU (`leave`, `or esi,-1`, `and [ecx],0`).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CGameDefinitionManager::Get @ 0x0044C6B0,
// CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CCharString::~CCharString @ 0x0099EAE0,
// GetInventoryCategory @ 0x005D97B0, CTCInventoryItem::GetUIInventoryCategory @ 0x005D96F0, and the menu's virtual slot +0x0C
// (branch lookup by name, CCharString by value, the callee destroys it).
// Shape: the same twelve-name lookup and flat else-if chain as GetTypeFromDefIndexPC @ 0x0059C447; every arm fetches
// one PC_TRADING_MENU_*_ENTRY branch and then stores the tab type. The by-value CCharString is built straight in its
// argument slot (`push ecx ; mov ecx,esp`) only because CCharString declares its copy constructor; without it VC builds
// a frame temporary and pushes its word. The 4 and "else" arms share OTHERS, which VC7.1 cross-jumps.
// The console twin @ 0x0059B79A is byte-identical apart from the UI_TRADING_MENU_* literals; the catalog label (no PC
// suffix) is propagated -- this copy reads the PC_TRADING_MENU_* names, so it is GetMenuBranchFromDefIndexPC.
// Name fix (agentNF6 2026-11-10): CTCInventoryItem::GetInventoryCategory 0x005D97B0 is the static ?GetInventoryCategory@CTCInventoryItem@@SIJJ@Z (Ego_r.pdb / FableWin.pdb), was a free GetInventoryCategory.
#pragma optimize("s", on)
// Name fix (agentNF54, e196): the local CCharString / CDefinitionManager / CGameDefinitionManager / CTCInventoryItem models and the
//   free GetInventoryTabState stand-in are gone: fable_string.h, engine/CGameDefinitionManager.h and engine/CTCInventoryItem.h
//   (FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY: the static __fastcall def accessors) supply them. 0x005D96F0 is
//   ?GetUIInventoryCategory@CTCInventoryItem@@SI?AW4EUIInventoryCategory@@J@Z (Ego_r.pdb 0x0055A650 + FableWin.pdb 0x01E02250), its result
//   compared with the EUIInventoryCategory enumerators (both PDBs: TROPHIES 0 .. OTHER 4).
// Re-port (p25P, 2026-11-21): the local CMenuTree / CMenuBranch / CTCInventoryTrade models are gone. engine/CTCInventoryTrade.h declares
//   the PDB signature ?GetMenuBranchFromDefIndexPC@CTCInventoryTrade@@AAEPAVCComponent@NUISystem@@JPAV23@AAW4EItem_Menu_Type@1@@Z
//   (private; NUISystem::CComponent* menu, EItem_Menu_Type& type), the branch lookup is CComponent::FindChild (vtable slot 3, +0x0C,
//   CCharString by value) and the tab type is stored as the PDB enumerators (FABLE_RETAIL_CTCINVENTORYTRADE_SELLITEM: MENU_TYPE_CLOTHING 0,
//   MENU_TYPE_WEAPONS 1, MENU_TYPE_ITEMS 2). Body otherwise unchanged; the TU-mate row 0x005B01FE carries it verbatim as its same-TU copy.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_PASS22_INVENTORY_ITEMS_ORIGINAL_ENUM
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_SELLITEM
#define FABLE_RETAIL_STLP_MAP
#include "fable_string.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/NUISystem_CComponent.h"
using NUISystem::CComponent;

CComponent* CTCInventoryTrade::GetMenuBranchFromDefIndexPC(long defIndex, CComponent* menu, EItem_Menu_Type& type)
{
    long melee, ranged, headwear, torso, gloves, legwear, footwear, suits;
    { CCharString name("INVENTORY_CATEGORY_FOOD", -1); CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_MELEE_WEAPONS", -1); melee = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_RANGED_WEAPONS", -1); ranged = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_HEADWEAR", -1); headwear = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_TORSO_WEAR", -1); torso = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_GLOVES", -1); gloves = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_LEGWEAR", -1); legwear = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_FOOTWEAR", -1); footwear = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_SUITS", -1); suits = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_GIFTS", -1); CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_TROPHIES", -1); CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_POTIONS", -1); CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }

    CComponent* branch;
    long category = CTCInventoryItem::GetInventoryCategory(defIndex);
    if (category == melee) {
        branch = menu->FindChild(CCharString("PC_TRADING_MENU_MELEE_ENTRY", -1));
        type = MENU_TYPE_WEAPONS;
    } else if (category == ranged) {
        branch = menu->FindChild(CCharString("PC_TRADING_MENU_RANGED_ENTRY", -1));
        type = MENU_TYPE_WEAPONS;
    } else if (category == headwear) {
        branch = menu->FindChild(CCharString("PC_TRADING_MENU_HEAD_ENTRY", -1));
        type = MENU_TYPE_CLOTHING;
    } else if (category == torso) {
        branch = menu->FindChild(CCharString("PC_TRADING_MENU_TORSO_ENTRY", -1));
        type = MENU_TYPE_CLOTHING;
    } else if (category == gloves) {
        branch = menu->FindChild(CCharString("PC_TRADING_MENU_HANDS_ENTRY", -1));
        type = MENU_TYPE_CLOTHING;
    } else if (category == legwear) {
        branch = menu->FindChild(CCharString("PC_TRADING_MENU_LEGS_ENTRY", -1));
        type = MENU_TYPE_CLOTHING;
    } else if (category == footwear) {
        branch = menu->FindChild(CCharString("PC_TRADING_MENU_FEET_ENTRY", -1));
        type = MENU_TYPE_CLOTHING;
    } else if (category == suits) {
        branch = menu->FindChild(CCharString("PC_TRADING_MENU_SUITS_ENTRY", -1));
        type = MENU_TYPE_CLOTHING;
    } else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == UI_INVENTORY_CATEGORY_TROPHIES) {
        branch = menu->FindChild(CCharString("PC_TRADING_MENU_TROPHIES_ENTRY", -1));
        type = MENU_TYPE_ITEMS;
    } else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == UI_INVENTORY_CATEGORY_POTIONS) {
        branch = menu->FindChild(CCharString("PC_TRADING_MENU_POTIONS_ENTRY", -1));
        type = MENU_TYPE_ITEMS;
    } else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == UI_INVENTORY_CATEGORY_GIFTS) {
        branch = menu->FindChild(CCharString("PC_TRADING_MENU_GIFTS_ENTRY", -1));
        type = MENU_TYPE_ITEMS;
    } else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == UI_INVENTORY_CATEGORY_PRODUCE) {
        branch = menu->FindChild(CCharString("PC_TRADING_MENU_PRODUCE_ENTRY", -1));
        type = MENU_TYPE_ITEMS;
    } else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == UI_INVENTORY_CATEGORY_OTHER) {
        branch = menu->FindChild(CCharString("PC_TRADING_MENU_OTHERS_ENTRY", -1));
        type = MENU_TYPE_ITEMS;
    } else {
        branch = menu->FindChild(CCharString("PC_TRADING_MENU_OTHERS_ENTRY", -1));
        type = MENU_TYPE_ITEMS;
    }
    return branch;
}
