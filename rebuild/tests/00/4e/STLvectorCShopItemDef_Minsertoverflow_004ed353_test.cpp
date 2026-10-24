#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour copy of STLport vector<CShopItemDef>::_M_insert_overflow: new capacity size + max(size, n), head copy, n copies
// of x, tail copy unless atend, old block released.
static int g_live = 0;
struct SItem {
    long Type;
    explicit SItem(long t) : Type(t) { ++g_live; }
    SItem(const SItem& o) : Type(o.Type) { ++g_live; }
    ~SItem() { --g_live; }
};
struct SVec {
    SItem* start; SItem* finish; SItem* eos;
    unsigned size() const { return unsigned(finish - start); }
    void clear() { for (SItem* p = start; p != finish; ++p) p->~SItem(); std::free(start); }
    void insert_overflow(SItem* position, const SItem& x, unsigned fill_len, bool atend)
    {
        const unsigned old_size = size();
        const unsigned len = old_size + (old_size < fill_len ? fill_len : old_size);
        SItem* new_start = len ? (SItem*)std::malloc(len * sizeof(SItem)) : 0;
        SItem* new_finish = new_start;
        for (SItem* f = start; f != position; ++f, ++new_finish) new (new_finish) SItem(*f);
        if (fill_len == 1) { new (new_finish) SItem(x); ++new_finish; }
        else for (unsigned n = fill_len; n > 0; --n, ++new_finish) new (new_finish) SItem(x);
        if (!atend) for (SItem* f = position; f != finish; ++f, ++new_finish) new (new_finish) SItem(*f);
        clear();
        start = new_start; finish = new_finish; eos = new_start + len;
    }
};
int main()
{
    bool ok = true;
    SVec v; v.start = (SItem*)std::malloc(3 * sizeof(SItem)); v.finish = v.start; v.eos = v.start + 3;
    for (int i = 0; i < 3; ++i) { new (v.finish) SItem(i); ++v.finish; }
    v.insert_overflow(v.finish, SItem(9), 1, true);                 // push_back overflow: capacity 6
    if (v.size() != 4 || unsigned(v.eos - v.start) != 6 || v.start[3].Type != 9 || v.start[0].Type != 0 || g_live != 4) ok = false;
    v.insert_overflow(v.start + 1, SItem(5), 7, false);             // insert 7 in the middle: capacity 4 + 7 = 11
    if (v.size() != 11 || unsigned(v.eos - v.start) != 11 || g_live != 11) ok = false;
    const long expect[11] = { 0, 5, 5, 5, 5, 5, 5, 5, 1, 2, 9 };
    for (int i = 0; ok && i < 11; ++i) if (v.start[i].Type != expect[i]) ok = false;
    v.clear();
    if (g_live != 0) ok = false;
    if (!ok) { std::printf("004ed353_TEST FAIL\n"); return 1; }
    std::printf("004ed353_TEST PASS\n");
    return 0;
}