// NInventory::CTCInventoryBase::UpdateItemStatsForThing(const CThing& thing) -- refreshes the item stats of the inventory category the thing's inventory-item component belongs to
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed; map pointer made const (the row grades the const LowerBound).
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- HasTC + the ternary GetTC model into a zero-initialised local: both arms load the component straight into ecx
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF414, length 110 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Ego_r.pdb: ?UpdateItemStatsForThing@CTCInventoryBase@NInventory@@IAEXABVCThing@@@Z (catalog label "CTCInventoryStats::
// BuildRenownString" is propagated; the body reads CTCInventoryBase's Categories at +0x20).
// Callees: std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x004365B0 (the inline CThing::GetInterface for interface
// 0x1A, the inventory-item component), CTCInventoryItem::GetCategoryIdentifier() @ 0x005D91F0 (manifest "CCategory::"),
// find(first, last, ident) over the categories @ 0x005C08D3 (LTCG __fastcall: ecx = first, edx = last, ident pushed),
// NInventory::CCategory::UpdateItemStatsForThing(const CThing&) @ 0x005BF16E.
// Shape: the component local is zeroed first (`and [ebp-8],0`) and its address taken by GetInterface, so the miss path reads
// it back from the frame; the GetInterface key sits in the dead `thing` slot; `this` is spilled to [ebp-4] around the lookup.
// Layout: retail CTCInventoryBase Categories {first,last,end} +0x20 (0x2C-byte CCategory); CThing bitset +0x20, map +0x44.
#pragma optimize("y", off)
#pragma optimize("s", on)

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline T* GetTC(long wanted)        // the landed 0x00584157 lookup: the entry pick as one ternary, the component read once
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((void*)((it == map->end() || wanted < it->first) ? map->end() : it)->second);
    }
    unsigned long HasTC(long wanted) { return m_present.test(wanted); }
};
class CTCInventoryItem { public:                             // interface type 0x1A
    long GetCategoryIdentifier() const;              // 0x005D91F0
};
namespace NInventory {
class CCategory { public:
    unsigned char _body[0x2C];
    void UpdateItemStatsForThing(const CThing& thing);   // 0x005BF16E
};
}
NInventory::CCategory* __fastcall FindCategory(NInventory::CCategory* first, NInventory::CCategory* last, long ident);   // 0x005C08D3

struct CTCInventoryBase_Retail {
    unsigned char _pad_0x00[0x20];
    NInventory::CCategory* CategoriesFirst;                    // +0x20
    NInventory::CCategory* CategoriesLast;                     // +0x24
    NInventory::CCategory* CategoriesEnd;                      // +0x28
    void UpdateItemStatsForThing(const CThing& thing);
};

void CTCInventoryBase_Retail::UpdateItemStatsForThing(const CThing& thing)
{
    CTCInventoryItem* item = 0;
    if (const_cast<CThing&>(thing).HasTC(0x1A))
        item = const_cast<CThing&>(thing).GetTC<CTCInventoryItem>(0x1A);
    long ident = item->GetCategoryIdentifier();
    FindCategory(CategoriesFirst, CategoriesLast, ident)->UpdateItemStatsForThing(thing);
}