// NInventory::CTCInventoryBase::OnItemReturnedToInventory(CThing& item) -- after refreshing the item's category stats, a trophy whose minigame is running is put in limbo and its minigame ended; any other returned item thing is killed
// Leaf: OnItemReturnedToInventory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF4ED, length 105 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Vtable slot 83 of the inventory screens (CTCInventoryClothing vtable 0x0124356C slot 83); Ego_r.pdb
// ?OnItemReturnedToInventory@CTCInventoryBase@NInventory@@MAEXAAVCThing@@@Z (catalog label "CTCInventoryStats::
// HandleMinigameCompletion" is propagated).
// Callees: CTCInventoryBase::UpdateItemStatsForThing(const CThing&) @ 0x005BF414, std::bitset::test @ 0x00410DE0,
// CVectorMap::LowerBound @ 0x0040F020 (the inline CThing::GetInterface for interface 0x39, the trophy), CThing::SetInLimbo(bool)
// @ 0x004C8CF0, CTCTrophy::EndMinigame() @ 0x006DDA50, CThing::Kill(bool) @ 0x004C9B80.
// Shape: `this` is only needed for the first call (ecx passes straight through); the trophy is a named local kept in esi; the
// key sits in the dead `item` slot; the miss and the not-in-minigame paths share the Kill(false) arm.
// Layout: CTCTrophy MinigameInProgress +0x14 (Ego_r.pdb); CThing bitset +0x20, interface map +0x44.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
struct CThing {
    void SetInLimbo(bool inLimbo);                   // 0x004C8CF0
    void Kill(bool b);                               // 0x004C9B80
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCTrophy {                                   // interface type 0x39
    unsigned char _pad_0x00[0x14];
    bool MinigameInProgress;                         // +0x14
    void EndMinigame();                              // 0x006DDA50
};

struct CTCInventoryBase_Retail {
    virtual void OnItemReturnedToInventory(CThing& item);
    void UpdateItemStatsForThing(const CThing& item);    // 0x005BF414
};

void CTCInventoryBase_Retail::OnItemReturnedToInventory(CThing& item)
{
    UpdateItemStatsForThing(item);
    CTCTrophy* trophy;
    if (item.GetInterface(&trophy, 0x39) && trophy->MinigameInProgress) {
        item.SetInLimbo(true);
        trophy->EndMinigame();
    } else
        item.Kill(false);
}