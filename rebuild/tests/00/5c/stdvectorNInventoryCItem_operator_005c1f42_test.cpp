// behaviour test for 005c1f42: STLport vector<NInventory::CItem>::operator= (grow / shrink / partial overlap arms)
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_live = 0;
struct CItem {
    int ItemType; int NumberOfItems; int PPerItemData[2]; int AverageBoughtPrice;
    CItem(int t) : ItemType(t), NumberOfItems(1), AverageBoughtPrice(0) { PPerItemData[0] = PPerItemData[1] = 0; ++g_live; }
    CItem(const CItem& o) : ItemType(o.ItemType), NumberOfItems(o.NumberOfItems), AverageBoughtPrice(o.AverageBoughtPrice) { PPerItemData[0] = o.PPerItemData[0]; PPerItemData[1] = o.PPerItemData[1]; ++g_live; }
    CItem& operator=(const CItem& o) { ItemType = o.ItemType; NumberOfItems = o.NumberOfItems; AverageBoughtPrice = o.AverageBoughtPrice; return *this; }
    ~CItem() { --g_live; }
};
struct CItemVector {
    CItem* _M_start; CItem* _M_finish; CItem* _M_end_of_storage;
    unsigned size() const { return unsigned(_M_finish - _M_start); }
    unsigned capacity() const { return unsigned(_M_end_of_storage - _M_start); }
    CItem* _M_allocate_and_copy(unsigned n, const CItem* first, const CItem* last)
    {
        CItem* r = n ? (CItem*)std::malloc(n * sizeof(CItem)) : 0;
        CItem* d = r;
        for (; first != last; ++first, ++d) new (d) CItem(*first);
        return r;
    }
    void _M_clear() { for (CItem* p = _M_start; p != _M_finish; ++p) p->~CItem(); if (_M_start) std::free(_M_start); }
    CItemVector& operator=(const CItemVector& x)
    {
        if (&x != this) {
            const unsigned xlen = x.size();
            if (xlen > capacity()) {
                CItem* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
                _M_clear();
                _M_start = tmp;
                _M_end_of_storage = _M_start + xlen;
            } else if (size() >= xlen) {
                CItem* i = _M_start;
                for (const CItem* s = x._M_start; s != x._M_finish; ++s, ++i) *i = *s;
                for (CItem* p = i; p != _M_finish; ++p) p->~CItem();
            } else {
                const CItem* s = x._M_start; CItem* d = _M_start;
                for (unsigned n = size(); n > 0; --n, ++s, ++d) *d = *s;
                CItem* f = _M_finish;
                for (; s != x._M_finish; ++s, ++f) new (f) CItem(*s);
            }
            _M_finish = _M_start + xlen;
        }
        return *this;
    }
};
static void Fill(CItemVector& v, unsigned cap, unsigned n, int base)
{
    v._M_start = (CItem*)std::malloc(cap * sizeof(CItem)); v._M_finish = v._M_start; v._M_end_of_storage = v._M_start + cap;
    for (unsigned i = 0; i < n; ++i) { new (v._M_finish) CItem(base + (int)i); ++v._M_finish; }
}
static bool Same(const CItemVector& a, const CItemVector& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned i = 0; i < a.size(); ++i) if (a._M_start[i].ItemType != b._M_start[i].ItemType) return false;
    return true;
}
int main()
{
    int bad = 0;
    CItemVector src, a, b, c;
    Fill(src, 6, 5, 100);
    Fill(a, 2, 2, 1);        /* grow arm: 5 > capacity 2 */
    Fill(b, 8, 7, 10);       /* shrink arm: size 7 >= 5, two destroyed */
    Fill(c, 6, 3, 20);       /* overlap arm: 3 assigned, 2 constructed */
    int live = g_live;       /* 5 + 2 + 7 + 3 = 17 */
    a = src; b = src; c = src;
    if (!Same(a, src) || !Same(b, src) || !Same(c, src)) ++bad;
    if (a.capacity() != 5 || b.capacity() != 8 || c.capacity() != 6) ++bad;
    if (g_live != live + 3 - 2 + 2) ++bad;   /* a: +5 -2, b: -2, c: +2 */
    CItemVector* self = &a; a = *self;       /* self-assignment: untouched */
    if (!Same(a, src)) ++bad;
    if (bad) { std::printf("005c1f42_TEST FAIL\n"); return 1; }
    std::printf("005c1f42_TEST PASS\n");
    return 0;
}