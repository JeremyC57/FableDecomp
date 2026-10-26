// NInventory::CTCInventoryBase::GetSelectedCategory() -- the selected inventory category, or null when there are no categories
// Leaf: GetSelectedCategory@CTCInventoryBase
// Retail: Fable.exe @ 0x005BCACE, 21 bytes; Ego_r.pdb ?GetSelectedCategory@CTCInventoryBase@NInventory@@QAEPAVCCategory@2@XZ.
// Layout: retail CTCInventoryBase (no engine header): Categories +0x20 (as landed 0x005BCE3C), SelectedCategoryIndex +0x70
// (Ego_r.pdb +0x78; retail STLport containers are smaller). NInventory::CCategory is 0x2C bytes.
// Shape: size TU, non-empty test first (retail falls through to the indexed return).
#pragma optimize("s", on)
namespace NInventory {
class CCategory { public:
    unsigned char _body[0x2C];
};
struct CCategoryVector {
    CCategory* first;
    CCategory* last;
    CCategory* end_of_storage;
    bool empty() const { return first == last; }
    CCategory& operator[](unsigned long n) { return first[n]; }
};
class CTCInventoryBase {
public:
    unsigned char   _pad_0x00[0x20];
    CCategoryVector Categories;                          // +0x20
    unsigned char   _pad_0x2c[0x70 - 0x2c];
    long            SelectedCategoryIndex;               // +0x70
    CCategory* GetSelectedCategory();
};

CCategory* CTCInventoryBase::GetSelectedCategory()
{
    if (!Categories.empty())
        return &Categories[SelectedCategoryIndex];
    return 0;
}
}