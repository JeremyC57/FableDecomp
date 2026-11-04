// NInventory::CTCInventoryBase::OnItemReturnedToInventory(CThing& item) -- after refreshing the item's category stats, a trophy whose minigame is running is put in limbo and its minigame ended; any other returned item thing is killed
// Leaf: OnItemReturnedToInventory
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
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

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    void SetInLimbo(bool inLimbo);                   // 0x004C8CF0
    void Kill(bool b);                               // 0x004C9B80
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || wanted < it->first)
                it = map->end();
            *out = (T*)((void*)it->second);
            return true;
        }
        return false;
    }
};
class CTCTrophy { public:                                   // interface type 0x39
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