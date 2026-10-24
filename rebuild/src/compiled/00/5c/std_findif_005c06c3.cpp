// std::__find_if(NInventory::CCategory* first, NInventory::CCategory* last, CIsCategoryWithDefIndex pred, const random_access_iterator_tag&) -- STLport random-access find_if over the inventory categories: the first category whose GlobalDefIndex equals the predicate's def index, else last
// Leaf: ??$__find_if@
// Retail: Fable.exe @ 0x005C06C3, 112 bytes, `ret 8`: __fastcall (/Gr template TU) -- first in ecx, last in edx (kept in ebx),
// the 4-byte predicate BY VALUE and the unused category tag on the stack. Only caller: the find_if forwarder @ 0x005C08D3
// (catalog "_Umove<EWeaponClass*>" propagated; its callers include CTCInventoryBase::AddItem 0x005BF4A3 and five more
// CTCInventoryBase members), called with GetInventoryCategory(itemType) @ 0x005D97B0 over CTCInventoryBase::Categories (+0x20).
// Element: retail NInventory::CCategory is 0x2C bytes (the PDB's 16-byte vector is 12 in retail, so engine/CCategory.h, 0x30,
// sits 4 bytes high): GlobalDefIndex at +0x1C is the compared word (`cmp [ecx+1Ch],esi`); the stride is the `push 2Ch ; pop edi`.
// Body: STLport 4.x _algo.c __find_if (four inlined predicate calls per trip, then the 3/2/1 remainder switch); size TU.
// Twin of 0x005C0653 (byte-identical second instantiation): this one serves the NON-const members (AddItem), the twin the
// const ones (Ego_r: GetBoughtPrice@CTCInventoryBase is `QBE`), so the iterator here is `CCategory*`.
// Retail evidence for the local model: the 0x2C stride; engine/CCategory.h has the PDB (0x30) layout, so it is not included.
#pragma optimize("s", on)
namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
// STLport 4.x _algo.c: the random-access __find_if, four predicate calls per trip, then the 3/2/1 remainder switch.
template <class RandomAccessIter, class Predicate>
RandomAccessIter __fastcall __find_if(RandomAccessIter first, RandomAccessIter last, Predicate pred, const random_access_iterator_tag&)
{
    int trip_count = (last - first) >> 2;
    for ( ; trip_count > 0 ; --trip_count) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}
}
class CTCInventoryBase;
namespace NInventory {
class CCategory {                               // retail layout, 0x2C bytes
public:
    unsigned char PAssociatedThing[0x8];        // +0x00 CIntelligentPointer<CThing>
    unsigned char ItemsInCategory[0xC];         // +0x08 vector<NInventory::CItem> (retail 12 bytes)
    void* PDef_Object;                          // +0x14 CDefPointer<const CInventoryCategoryDef>
    CTCInventoryBase* PInventory;               // +0x18
    long GlobalDefIndex;                        // +0x1C
    long SelectedItemIndex;                     // +0x20
    long HighlightedItemIndex;                  // +0x24
    bool Selected;                              // +0x28
    bool DisplayItems;                          // +0x29
};
}
struct CIsCategoryWithDefIndex {
    long DefIndex;                              // +0x00
    bool operator()(const NInventory::CCategory& category) const { return category.GlobalDefIndex == DefIndex; }
};
template NInventory::CCategory* __fastcall std::__find_if(NInventory::CCategory*, NInventory::CCategory*, CIsCategoryWithDefIndex, const std::random_access_iterator_tag&);