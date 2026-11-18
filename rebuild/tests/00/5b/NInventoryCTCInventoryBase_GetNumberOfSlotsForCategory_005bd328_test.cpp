#include <cstdio>
namespace NInventory {
class CCategory { public:
    long m_slots;
    unsigned char _body[0x2C - 4];
    long GetNumberOfSlots() { return m_slots; }
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
    CCategoryVector Categories;
    long GetNumberOfSlotsInCategory(long index);
};
long CTCInventoryBase::GetNumberOfSlotsInCategory(long index)
{
    CCategory* first = Categories.first;
    return first[index].GetNumberOfSlots();
}
}
int main()
{
    NInventory::CCategory cats[3];
    cats[0].m_slots = 4; cats[1].m_slots = 9; cats[2].m_slots = 16;
    NInventory::CTCInventoryBase inv;
    inv.Categories.first = cats; inv.Categories.last = cats + 3; inv.Categories.end_of_storage = cats + 3;
    if (inv.GetNumberOfSlotsInCategory(0) != 4 || inv.GetNumberOfSlotsInCategory(1) != 9 || inv.GetNumberOfSlotsInCategory(2) != 16) { std::printf("005bd328_TEST FAIL\n"); return 1; }
    std::printf("005bd328_TEST PASS\n");
    return 0;
}