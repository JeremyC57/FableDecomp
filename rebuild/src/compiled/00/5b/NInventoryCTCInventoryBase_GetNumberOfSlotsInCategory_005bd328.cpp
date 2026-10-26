// NInventory::CTCInventoryBase::GetNumberOfSlotsInCategory(long index) -- the slot count of the index-th inventory category (Categories[index].GetNumberOfSlots())
// Leaf: ?GetNumberOfSlotsInCategory@CTCInventoryBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD328, length 20 bytes, thiscall `ret 4`; size-optimised TU (packed, no
// padding between neighbours), so the result passes straight through eax.
// Callee: NInventory::CCategory::GetNumberOfSlots @ 0x005BD1CF (landed; the catalog label there is the propagated
// "GetCategoryIdentifier").
// Caller: CTCInventoryExperience::ResetStatIndicatorValues @ 0x0056E5AB calls it on `this` with
// GetSelectedCategoryIndex() (0x005BC4B2), so it is a CTCInventoryBase member. The vector index is inlined here
// (`first + index*2Ch` off Categories at +0x20, the landed 0x005BCE3C GetCategoryWithIndex model) rather than called;
// the vector's first pointer is a named local, so it is loaded into eax before the index (`Categories[index]` loads
// the index first and copies `this` to eax).
// The method name is descriptive (no PDB symbol); the catalog's `_Cons_val<allocator<pair<EHeroMorphType,...>>>` label
// is propagated.
#pragma optimize("s", on)
namespace NInventory {
class CCategory { public:
    unsigned char _body[0x2C];
    long GetNumberOfSlots();                             // 0x005BD1CF
};
struct CCategoryVector {
    CCategory* first;
    CCategory* last;
    CCategory* end_of_storage;
    CCategory& operator[](unsigned long n) { return first[n]; }
};
class CTCInventoryBase {
public:
    unsigned char   _pad_0x00[0x20];
    CCategoryVector Categories;                          // +0x20
    long GetNumberOfSlotsInCategory(long index);
};

long CTCInventoryBase::GetNumberOfSlotsInCategory(long index)
{
    CCategory* first = Categories.first;
    return first[index].GetNumberOfSlots();
}
}