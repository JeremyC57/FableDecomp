// behaviour test for 0x005BCACE
#include <stdio.h>
namespace NInventory {
struct CCategory {
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
using namespace NInventory;
int main()
{
    CCategory cats[3]; CTCInventoryBase b; int ok = 1;
    b.Categories.first = b.Categories.last = cats; b.SelectedCategoryIndex = 1;
    ok &= b.GetSelectedCategory() == 0;
    b.Categories.last = cats + 3;
    ok &= b.GetSelectedCategory() == &cats[1];
    b.SelectedCategoryIndex = 2; ok &= b.GetSelectedCategory() == &cats[2];
    printf(ok ? "005bcace_TEST PASS\n" : "005bcace_TEST FAIL\n");
    return ok ? 0 : 1;
}