// Behaviour test for NInventory::CTCInventoryBase::GetCategoryCount @ 0x005BCE2F: the method comes from the source object.
#include <cstdio>
#include <stddef.h>
namespace NInventory {
class CCategory { unsigned char _body[0x2c]; };
class CTCInventoryBase {
public:
    virtual ~CTCInventoryBase() {}
    unsigned char _pad_0x04[0x18];
    long          InventorySelectionState;
    CCategory*    CategoriesFirst;
    CCategory*    CategoriesLast;
    CCategory*    CategoriesEnd;
    long GetCategoryCount() const;
};
}
int main()
{
    using NInventory::CCategory;
    static CCategory buf[7];
    NInventory::CTCInventoryBase b;
    b.CategoriesFirst = buf; b.CategoriesLast = buf + 5; b.CategoriesEnd = buf + 7;
    int ok = b.GetCategoryCount() == 5;
    b.CategoriesLast = buf; ok &= b.GetCategoryCount() == 0;
    ok &= sizeof(CCategory) == 0x2c && offsetof(NInventory::CTCInventoryBase, CategoriesFirst) == 0x20;
    if (!ok) { std::printf("005bce2f_TEST FAIL\n"); return 1; }
    std::printf("005bce2f_TEST PASS\n");
    return 0;
}
