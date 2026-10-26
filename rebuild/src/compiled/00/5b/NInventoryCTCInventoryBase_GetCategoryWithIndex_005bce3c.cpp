// NInventory::CTCInventoryBase::GetCategoryWithIndex(long index) -- address of the index-th inventory category (0x2C-byte NInventory::CCategory elements of the Categories vector)
// Leaf: GetCategoryWithIndex@CTCInventoryBase
// Retail: Fable.exe @ 0x005BCE3C, 13 bytes, `ret 4`; size TU (as landed 0x005BDCDC).
// Layout: retail CTCInventoryBase (no engine header; PDB minus 4 per preceding std::vector): Categories +0x20.
#pragma optimize("s", on)
namespace NInventory {
class CCategory { public:
    unsigned char _body[0x2C];
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
    CCategory* GetCategoryWithIndex(long index);
};

CCategory* CTCInventoryBase::GetCategoryWithIndex(long index)
{
    return &Categories[index];
}
}