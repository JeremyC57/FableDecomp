#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour of vector<NInventory::CItem>::resize(n) (0x005C2AA9): grows with zeroed items, shrinks by erasing the tail.
struct SCountBlock { long m_count; };
static long g_releases = 0;
template <class T> struct CCountedPointer {
    T* m_pObject; SCountBlock* m_pBlock;
    CCountedPointer() : m_pObject(0), m_pBlock(0) {}
    CCountedPointer(const CCountedPointer& o) : m_pObject(o.m_pObject), m_pBlock(o.m_pBlock) { if (m_pBlock) ++m_pBlock->m_count; }
    ~CCountedPointer() { if (m_pBlock) { ++g_releases; --m_pBlock->m_count; } m_pObject = 0; m_pBlock = 0; }
};
struct CPerItemData { long m_value; };
struct CItem {
    long ItemType; long NumberOfItems; CCountedPointer<CPerItemData> PPerItemData; long AverageBoughtPrice;
    CItem() : ItemType(0), NumberOfItems(0), AverageBoughtPrice(0) {}
};
struct CItemVector {
    CItem* _First; CItem* _Last; CItem* _End;
    void resize(unsigned int newSize, const CItem& value)
    {
        unsigned int size = (unsigned int)(_Last - _First);
        if (newSize < size) { for (CItem* p = _First + newSize; p != _Last; ++p) p->~CItem(); _Last = _First + newSize; }
        else for (; size < newSize; ++size) { new (_Last) CItem(value); ++_Last; }
    }
    void resize(unsigned int newSize) { resize(newSize, CItem()); }
};
int main()
{
    CItem* storage = (CItem*)std::malloc(sizeof(CItem) * 8);
    CItemVector v; v._First = storage; v._Last = storage; v._End = storage + 8;
    v.resize(3);
    int bad = (v._Last - v._First) != 3;
    for (int i = 0; i < 3; ++i) bad |= (storage[i].ItemType != 0 || storage[i].NumberOfItems != 0 || storage[i].AverageBoughtPrice != 0 || storage[i].PPerItemData.m_pBlock != 0);
    SCountBlock block; block.m_count = 1; CPerItemData data; data.m_value = 4;
    storage[2].PPerItemData.m_pObject = &data; storage[2].PPerItemData.m_pBlock = &block;
    v.resize(1);
    bad |= ((v._Last - v._First) != 1 || g_releases != 1 || block.m_count != 0);
    if (bad) { std::printf("005c2aa9_TEST FAIL\n"); return 1; }
    std::printf("005c2aa9_TEST PASS\n");
    return 0;
}
