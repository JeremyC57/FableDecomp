// CTCInventoryTrade::GetTypeFromDefIndexPC(long defIndex) -- maps an item def's inventory category to the PC trade tab: 1 weapons, 0 clothing, 2 everything else
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059C447, 617 bytes, size-optimised TU (`push 2 ; pop eax`, `leave`, `or esi,-1`).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CGameDefinitionManager::Get @ 0x0044C6B0 (static __fastcall),
// CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410, CCharString::~CCharString @ 0x0099EAE0,
// GetInventoryCategory(long) @ 0x005D97B0 and the inventory tab-state query @ 0x005D96F0 (both __fastcall, the def index in ecx).
// Shape: twelve INVENTORY_CATEGORY_* def indices are looked up (FOOD, GIFTS, TROPHIES, POTIONS unused); each name is a named
// CCharString in its own block (a temporary would pass the ctor's eax instead of retail's re-`lea`). `this` is never read.
// The decision is one flat else-if chain with a separate `return` per value, which VC7.1 merges into retail's groups
// (`F==0 || F==1` -> push 2/pop eax; `F==2`, `F==3`, `F==4`, else -> the 2 promoted to edi). The sibling
// GetMenuBranchFromDefIndex @ 0x0059B79A has the same chain with a distinct label per case.
#pragma optimize("s", on)
class CCharString { public:
    void* PImp;
    CCharString(const char* text, long length);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();          // 0x0044C6B0
};
long __fastcall GetInventoryCategory(long defIndex);          // 0x005D97B0
long __fastcall GetInventoryTabState(long defIndex);          // 0x005D96F0

// CTCInventoryTrade: no member is read; a method-only model (the engine header is not needed here).
class CTCInventoryTrade { public:
    long GetTypeFromDefIndexPC(long defIndex);
};

long CTCInventoryTrade::GetTypeFromDefIndexPC(long defIndex)
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

    long category = GetInventoryCategory(defIndex);
    if (category == melee)
        return 1;
    else if (category == ranged)
        return 1;
    else if (category == headwear)
        return 0;
    else if (category == torso)
        return 0;
    else if (category == gloves)
        return 0;
    else if (category == legwear)
        return 0;
    else if (category == footwear)
        return 0;
    else if (category == suits)
        return 0;
    else if (GetInventoryTabState(defIndex) == 0)
        return 2;
    else if (GetInventoryTabState(defIndex) == 1)
        return 2;
    else if (GetInventoryTabState(defIndex) == 2)
        return 2;
    else if (GetInventoryTabState(defIndex) == 3)
        return 2;
    else if (GetInventoryTabState(defIndex) == 4)
        return 2;
    else
        return 2;
}