// Behaviour test for 0x0059CF49 CTCInventoryTrade::RemoveAllItemsFromCategory
#include <cstdio>
#include <cstdlib>
struct CItemCount { long first; long second; };
struct CItemCountVector {
    CItemCount* _M_start; CItemCount* _M_finish; CItemCount* _M_end_of_storage;
    CItemCountVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CItemCountVector() { if (_M_start) std::free(_M_start); }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CItemCount& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CCategory {
    long n;
    void GetAllItemsInCategory(CItemCountVector& v)
    {
        v._M_start = (CItemCount*)std::malloc(sizeof(CItemCount) * 4);
        for (long i = 0; i < n; ++i) { v._M_start[i].first = 10 + i; v._M_start[i].second = i + 1; }
        v._M_finish = v._M_start + n; v._M_end_of_storage = v._M_start + 4;
    }
};
struct CTrade {
    CCategory cats[2]; long CategoryIdent; long removedTypes, removedCount;
    long GetCategoryIndexForTradeType() { return 1; }
    CCategory* GetCategoryWithIndex(long i) { return &cats[i]; }
    void RemoveItemFromInventory(long t, long c) { removedTypes += t; removedCount += c; }
    void RemoveAllItemsFromCategory(long category)
    {
        CategoryIdent = category;
        CCategory* tradeCategory = GetCategoryWithIndex(GetCategoryIndexForTradeType());
        CItemCountVector items;
        tradeCategory->GetAllItemsInCategory(items);
        long count = (long)items.size();
        for (long i = 0; i < count; ++i)
            RemoveItemFromInventory(items[i].first, items[i].second);
    }
};
int main()
{
    CTrade t; t.cats[0].n = 1; t.cats[1].n = 3; t.CategoryIdent = 0; t.removedTypes = 0; t.removedCount = 0;
    t.RemoveAllItemsFromCategory(5);
    bool ok = t.CategoryIdent == 5 && t.removedTypes == 33 && t.removedCount == 6;
    if (!ok) { std::printf("0059cf49_TEST FAIL\n"); return 1; }
    std::printf("0059cf49_TEST PASS\n");
    return 0;
}
