#include <stdio.h>
namespace NInventory {
class CCategory { public: unsigned char _body[0x2C]; };
struct CCategoryVector {
    CCategory* first; CCategory* last; CCategory* end_of_storage;
    CCategory& operator[](unsigned long n) { return first[n]; }
};
class CTCInventoryBase {
public:
    unsigned char   _pad_0x00[0x20];
    CCategoryVector Categories;
    CCategory* GetCategoryWithIndex(long index);
};

CCategory* CTCInventoryBase::GetCategoryWithIndex(long index)
{
    return &Categories[index];
}
}
int main()
{
    static NInventory::CCategory cats[4]; static NInventory::CTCInventoryBase inv;
    inv.Categories.first = cats; inv.Categories.last = cats + 4;
    bool ok = inv.GetCategoryWithIndex(0) == &cats[0] && inv.GetCategoryWithIndex(3) == &cats[3]
           && (char*)inv.GetCategoryWithIndex(2) - (char*)cats == 0x58;
    printf(ok ? "005bce3c_TEST PASS\n" : "005bce3c_TEST FAIL\n");
    return ok ? 0 : 1;
}