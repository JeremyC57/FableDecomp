#include <cstdio>
// Behaviour test for CTCInventory::Copy (retail 0x00583A6D).
struct CLongVector {
    long data[4]; unsigned long count;
    CLongVector& operator=(const CLongVector& other)
    {
        count = other.count;
        for (unsigned long i = 0; i < other.count; ++i) data[i] = other.data[i];
        return *this;
    }
};
struct CTCBase { long m_baseState; };
struct CTCInventoryBase : CTCBase {
    void Copy(const CTCBase* other) { m_baseState = other->m_baseState; }
};
struct CTCInventory : CTCInventoryBase {
    CLongVector QuickAccessItems;
    void Copy(const CTCBase* other);
};
void CTCInventory::Copy(const CTCBase* other)
{
    CTCInventoryBase::Copy(other);
    QuickAccessItems = static_cast<const CTCInventory*>(other)->QuickAccessItems;
}
int main()
{
    CTCInventory a, b;
    a.m_baseState = 5; a.QuickAccessItems.count = 3; a.QuickAccessItems.data[0] = 10; a.QuickAccessItems.data[1] = 0; a.QuickAccessItems.data[2] = 30;
    b.m_baseState = 0; b.QuickAccessItems.count = 0;
    b.Copy(&a);
    if (b.m_baseState != 5 || b.QuickAccessItems.count != 3 || b.QuickAccessItems.data[0] != 10 || b.QuickAccessItems.data[2] != 30) { std::printf("00583a6d_TEST FAIL\n"); return 1; }
    std::printf("00583a6d_TEST PASS\n");
    return 0;
}