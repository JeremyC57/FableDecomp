// Grade: DIFFER(298v301) -- global esi/edi swap: retail keeps `this` in esi (spilled to [ebp-8]) and the zero / hero in edi, ours the reverse, so the last lookup reuses the dead hero register instead of reloading `this`; structure (sunk `push ebx`, converted CThing* temporary for IS_THING_ALIVE, ternary entry pick) is retail's. Tried: if-form pick (304v301), named self pointer.
// NInventory::CTCInventoryBase::SetNumberOfSelectedItemOwned(long itemDefIndex) -- for the highlighted item def, stores the selected category's per-slot maximum and how many the living hero owns across its three inventory components
// Retail 0x005BE284, 301 bytes, `ret 4`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: GetSelectedCategory() 0x005BCACE, the category's per-slot maximum 0x005BD993 (manifest "CRenderTarget::
// DetachTarget" is propagated; name descriptive), CTCBase::GetPlayerManager() const 0x00686D50, CPlayerManager::
// GetMainPlayer() const 0x00449960, CPlayer::GetControlledCreature 0x00487DD0, IS_THING_ALIVE<T>(const T&) 0x0043DBF0
// (__fastcall, the pointer's address in ecx), std::bitset::test 0x00410DE0 + the const CVectorMap::LowerBound 0x004365B0
// (three inline GetInterface lookups: inventory components 0x11, 0x13, 0x12), GetNumberOfItemsOfTypeInInventory(long)
// 0x005BDF08 (on each component).
// Shape: both counters are reset first (max 1, owned 0); a positive def index takes the category maximum when a category
// is selected, then sums the hero's counts; the creature converts to a CThing* temporary for IS_THING_ALIVE<CThing*>
// (its frame word is reused by the lookup keys while the creature stays in edi); `this` lives in a frame word across the lookups.
// Layout (retail; Ego_r.pdb names): HighlightedItemNumOwned +0x2C, HighlightedItemMaxOwned +0x30.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing {};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
struct CPlayer {
    CThingCreatureBase* GetControlledCreature();     // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                  // 0x00449960
};
namespace NInventory {
struct CCategory {
    long GetMaxItemsPerSlot();                       // 0x005BD993
};
struct CTCInventoryBase {
    void* __vftable;
    unsigned char _pad_0x004[0x2c - 0x04];
    long HighlightedItemNumOwned;                    // +0x2c
    long HighlightedItemMaxOwned;                    // +0x30
    CCategory* GetSelectedCategory();                // 0x005BCACE
    const CPlayerManager* GetPlayerManager() const;  // 0x00686D50 (CTCBase)
    long GetNumberOfItemsOfTypeInInventory(long itemDefIndex);   // 0x005BDF08
    void SetNumberOfSelectedItemOwned(long itemDefIndex);
};

void CTCInventoryBase::SetNumberOfSelectedItemOwned(long itemDefIndex)
{
    HighlightedItemMaxOwned = 1;
    HighlightedItemNumOwned = 0;
    if (itemDefIndex > 0) {
        CCategory* category = GetSelectedCategory();
        if (category)
            HighlightedItemMaxOwned = category->GetMaxItemsPerSlot();
        long owned = 0;
        CThingCreatureBase* hero = GetPlayerManager()->GetMainPlayer()->GetControlledCreature();
        if (IS_THING_ALIVE<CThing*>(hero)) {
            CTCInventoryBase* inventory;
            if (hero->GetInterface(&inventory, 0x11))
                owned = inventory->GetNumberOfItemsOfTypeInInventory(itemDefIndex);
            if (hero->GetInterface(&inventory, 0x13))
                owned += inventory->GetNumberOfItemsOfTypeInInventory(itemDefIndex);
            if (hero->GetInterface(&inventory, 0x12))
                owned += inventory->GetNumberOfItemsOfTypeInInventory(itemDefIndex);
        }
        HighlightedItemNumOwned = owned;
    }
}
}
