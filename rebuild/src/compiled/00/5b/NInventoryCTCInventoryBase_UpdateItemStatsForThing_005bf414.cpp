// NInventory::CTCInventoryBase::UpdateItemStatsForThing(const CThing& thing) -- refreshes the item stats of the inventory category the thing's inventory-item component belongs to
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

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x004365B0
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline T* GetTC(long wanted)        // the landed 0x00584157 lookup: the entry pick as one ternary, the component read once
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long wanted) { return m_present.test(wanted); }
};
struct CTCInventoryItem {                             // interface type 0x1A
    long GetCategoryIdentifier() const;              // 0x005D91F0
};
namespace NInventory {
struct CCategory {
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