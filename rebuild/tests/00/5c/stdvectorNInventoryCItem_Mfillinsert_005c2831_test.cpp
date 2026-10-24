// behaviour test for 005c2831: STLport vector<NInventory::CItem>::_M_fill_insert (both in-place arms and the overflow path)
#include <cstdio>
#include <new>
static int g_live = 0;
static int g_overflow = 0;
struct CItem {
    int ItemType; int NumberOfItems; int PPerItemData[2]; int AverageBoughtPrice;
    CItem() : ItemType(0), NumberOfItems(0), AverageBoughtPrice(0) { PPerItemData[0] = PPerItemData[1] = 0; ++g_live; }
    CItem(const CItem& o) : ItemType(o.ItemType), NumberOfItems(o.NumberOfItems), AverageBoughtPrice(o.AverageBoughtPrice) { PPerItemData[0] = o.PPerItemData[0]; PPerItemData[1] = o.PPerItemData[1]; ++g_live; }
    CItem& operator=(const CItem& o) { ItemType = o.ItemType; NumberOfItems = o.NumberOfItems; AverageBoughtPrice = o.AverageBoughtPrice; return *this; }
    ~CItem() { --g_live; }
};
struct CItemVector {
    CItem* _M_start; CItem* _M_finish; CItem* _M_end_of_storage;
    void _M_insert_overflow(CItem*, const CItem&, unsigned, bool) { ++g_overflow; }
    void _M_fill_insert(CItem* position, unsigned n, const CItem& x)
    {
        if (n != 0) {
            if (unsigned(_M_end_of_storage - _M_finish) >= n) {
                CItem x_copy = x;
                const unsigned elems_after = unsigned(_M_finish - position);
                CItem* old_finish = _M_finish;
                if (elems_after > n) {
                    CItem* d = _M_finish;
                    for (CItem* s = _M_finish - n; s != _M_finish; ++s, ++d) new (d) CItem(*s);
                    _M_finish += n;
                    CItem* last = old_finish - n; CItem* result = old_finish;
                    for (int k = int(last - position); k > 0; --k) *--result = *--last;
                    for (CItem* p = position; p != position + n; ++p) *p = x_copy;
                } else {
                    CItem* f = _M_finish;
                    for (unsigned k = n - elems_after; k > 0; --k, ++f) new (f) CItem(x_copy);
                    _M_finish += n - elems_after;
                    CItem* d = _M_finish;
                    for (CItem* s = position; s != old_finish; ++s, ++d) new (d) CItem(*s);
                    _M_finish += elems_after;
                    for (CItem* p = position; p != old_finish; ++p) *p = x_copy;
                }
            } else
                _M_insert_overflow(position, x, n, false);
        }
    }
};
static unsigned char g_raw[16 * sizeof(CItem)];
static void Reset(CItemVector& v, int count)
{
    CItem* s = (CItem*)g_raw;
    v._M_start = v._M_finish = s; v._M_end_of_storage = s + 10;
    for (int i = 0; i < count; ++i) { new (v._M_finish) CItem(); v._M_finish->ItemType = i + 1; ++v._M_finish; }
}
static bool Check(const CItemVector& v, const int* want, int n)
{
    if (v._M_finish - v._M_start != n) return false;
    for (int i = 0; i < n; ++i) if (v._M_start[i].ItemType != want[i]) return false;
    return true;
}
static void Clear(CItemVector& v) { for (CItem* p = v._M_start; p != v._M_finish; ++p) p->~CItem(); }
int main()
{
    int bad = 0;
    CItemVector v; CItem x; x.ItemType = 9;
    int base = g_live;
    Reset(v, 5);                                         /* 1 2 3 4 5, insert 2 nines at 1: elems_after 4 > 2 */
    v._M_fill_insert(v._M_start + 1, 2, x);
    { int w[7] = { 1, 9, 9, 2, 3, 4, 5 }; if (!Check(v, w, 7)) ++bad; }
    if (g_live != base + 7) ++bad;
    Clear(v);
    Reset(v, 4);                                         /* 1 2 3 4, insert 3 nines at 2: elems_after 2 <= 3 */
    v._M_fill_insert(v._M_start + 2, 3, x);
    { int w[7] = { 1, 2, 9, 9, 9, 3, 4 }; if (!Check(v, w, 7)) ++bad; }
    if (g_live != base + 7) ++bad;
    v._M_fill_insert(v._M_start, 0, x);                  /* n == 0: nothing */
    v._M_fill_insert(v._M_finish, 4, x);                 /* 3 spare < 4: overflow path */
    if (g_overflow != 1 || v._M_finish - v._M_start != 7) ++bad;
    Clear(v);
    if (g_live != base) ++bad;
    if (bad) { std::printf("005c2831_TEST FAIL\n"); return 1; }
    std::printf("005c2831_TEST PASS\n");
    return 0;
}