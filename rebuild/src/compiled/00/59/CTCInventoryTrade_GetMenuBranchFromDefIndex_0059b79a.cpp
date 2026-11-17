// CTCInventoryTrade::GetMenuBranchFromDefIndex(long defIndex, CMenuTree* menu, long* type) -- picks the trade menu branch for an item def by its inventory category and reports the tab type (1 weapons, 0 clothing, 2 other)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059B79A, 897 bytes, size-optimised TU (`leave`, `or esi,-1`, `and [ecx],0`).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CGameDefinitionManager::Get @ 0x0044C6B0,
// CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CCharString::~CCharString @ 0x0099EAE0,
// GetInventoryCategory @ 0x005D97B0, CTCInventoryItem::GetUIInventoryCategory @ 0x005D96F0, and the menu's virtual slot +0x0C
// (branch lookup by name, CCharString by value, the callee destroys it).
// Shape: the same twelve-name lookup and flat else-if chain as GetTypeFromDefIndexPC @ 0x0059C447; every arm fetches
// one UI_TRADING_MENU_*_ENTRY branch and then stores the tab type. The by-value CCharString is built straight in its
// argument slot (`push ecx ; mov ecx,esp`) only because CCharString declares its copy constructor; without it VC builds
// a frame temporary and pushes its word. The 4 and "else" arms share OTHERS, which VC7.1 cross-jumps.
// The PC twin @ 0x0059C0C6 is byte-identical apart from the PC_TRADING_MENU_* literals.
// Name fix (agentNF6 2026-11-10): CTCInventoryItem::GetInventoryCategory 0x005D97B0 is the static ?GetInventoryCategory@CTCInventoryItem@@SIJJ@Z (Ego_r.pdb / FableWin.pdb), was a free GetInventoryCategory.
#pragma optimize("s", on)
// Name fix (agentNF54, e196): the local CCharString / CDefinitionManager / CGameDefinitionManager / CTCInventoryItem models and the
//   free GetInventoryTabState stand-in are gone: fable_string.h, engine/CGameDefinitionManager.h and engine/CTCInventoryItem.h
//   (FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY: the static __fastcall def accessors) supply them. 0x005D96F0 is
//   ?GetUIInventoryCategory@CTCInventoryItem@@SI?AW4EUIInventoryCategory@@J@Z (Ego_r.pdb 0x0055A650 + FableWin.pdb 0x01E02250), its result
//   compared with the EUIInventoryCategory enumerators (both PDBs: TROPHIES 0 .. OTHER 4).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_PASS22_INVENTORY_ITEMS_ORIGINAL_ENUM
#include "fable_string.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCInventoryItem.h"

struct CMenuBranch;
struct CMenuTree {
    virtual void V0();
    virtual void V1();
    virtual void V2();
    virtual CMenuBranch* GetBranch(CCharString name);   // vtable +0x0C
};
class CTCInventoryTrade { public:
    CMenuBranch* GetMenuBranchFromDefIndex(long defIndex, CMenuTree* menu, long* type);
};

CMenuBranch* CTCInventoryTrade::GetMenuBranchFromDefIndex(long defIndex, CMenuTree* menu, long* type)
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

    CMenuBranch* branch;
    long category = CTCInventoryItem::GetInventoryCategory(defIndex);
    if (category == melee) {
        branch = menu->GetBranch(CCharString("UI_TRADING_MENU_MELEE_ENTRY", -1));
        *type = 1;
    } else if (category == ranged) {
        branch = menu->GetBranch(CCharString("UI_TRADING_MENU_RANGED_ENTRY", -1));
        *type = 1;
    } else if (category == headwear) {
        branch = menu->GetBranch(CCharString("UI_TRADING_MENU_HEAD_ENTRY", -1));
        *type = 0;
    } else if (category == torso) {
        branch = menu->GetBranch(CCharString("UI_TRADING_MENU_TORSO_ENTRY", -1));
        *type = 0;
    } else if (category == gloves) {
        branch = menu->GetBranch(CCharString("UI_TRADING_MENU_HANDS_ENTRY", -1));
        *type = 0;
    } else if (category == legwear) {
        branch = menu->GetBranch(CCharString("UI_TRADING_MENU_LEGS_ENTRY", -1));
        *type = 0;
    } else if (category == footwear) {
        branch = menu->GetBranch(CCharString("UI_TRADING_MENU_FEET_ENTRY", -1));
        *type = 0;
    } else if (category == suits) {
        branch = menu->GetBranch(CCharString("UI_TRADING_MENU_SUITS_ENTRY", -1));
        *type = 0;
    } else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == UI_INVENTORY_CATEGORY_TROPHIES) {
        branch = menu->GetBranch(CCharString("UI_TRADING_MENU_TROPHIES_ENTRY", -1));
        *type = 2;
    } else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == UI_INVENTORY_CATEGORY_POTIONS) {
        branch = menu->GetBranch(CCharString("UI_TRADING_MENU_POTIONS_ENTRY", -1));
        *type = 2;
    } else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == UI_INVENTORY_CATEGORY_GIFTS) {
        branch = menu->GetBranch(CCharString("UI_TRADING_MENU_GIFTS_ENTRY", -1));
        *type = 2;
    } else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == UI_INVENTORY_CATEGORY_PRODUCE) {
        branch = menu->GetBranch(CCharString("UI_TRADING_MENU_PRODUCE_ENTRY", -1));
        *type = 2;
    } else if (CTCInventoryItem::GetUIInventoryCategory(defIndex) == UI_INVENTORY_CATEGORY_OTHER) {
        branch = menu->GetBranch(CCharString("UI_TRADING_MENU_OTHERS_ENTRY", -1));
        *type = 2;
    } else {
        branch = menu->GetBranch(CCharString("UI_TRADING_MENU_OTHERS_ENTRY", -1));
        *type = 2;
    }
    return branch;
}