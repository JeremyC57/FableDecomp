// Behaviour test for 0x00593F39 std::vector<CContextSensitiveItem>::_M_insert_overflow (model copy)
#include <cstdio>
#include <cstdlib>
#include <new>
#include <stddef.h>
struct Item { int ThingDefIndex; int ButtonIndex; int Priority; };
static Item* ucopy(const Item* f, const Item* l, Item* r) { Item* c = r; for (; f != l; ++f, ++c) new (c) Item(*f); return c; }
static Item* ufill_n(Item* f, size_t n, const Item& x) { Item* c = f; for (; n > 0; --n, ++c) new (c) Item(x); return c; }
template <class T> const T& tmax(const T& a, const T& b) { return a < b ? b : a; }
static Item* allocate(size_t n) { return n != 0 ? (Item*)malloc(n * sizeof(Item)) : 0; }
struct Vec {
    Item* start; Item* finish; Item* eos;
    size_t size() const { return size_t(finish - start); }
    void insert_overflow(Item* pos, const Item& x, size_t fill_len, bool atend)
    {
        const size_t old = size();
        const size_t len = old + tmax(old, fill_len);
        Item* ns = allocate(len);
        Item* nf = ucopy(start, pos, ns);
        if (fill_len == 1) { new (nf) Item(x); ++nf; } else nf = ufill_n(nf, fill_len, x);
        if (!atend) nf = ucopy(pos, finish, nf);
        if (start) free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
static Item I(int k) { Item r; r.ThingDefIndex = k; r.ButtonIndex = k + 100; r.Priority = -k; return r; }
int main()
{
    Vec v; v.start = v.finish = v.eos = 0;
    v.insert_overflow(v.finish, I(1), 1, true);
    if (v.size() != 1 || v.eos - v.start != 1 || v.start[0].ThingDefIndex != 1) { std::printf("00593f39_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.finish, I(2), 1, true);
    v.insert_overflow(v.start + 1, I(7), 2, false);        // 1 7 7 2, capacity 2 + 2
    int want[4] = { 1, 7, 7, 2 };
    if (v.size() != 4 || v.eos - v.start != 4) { std::printf("00593f39_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (v.start[i].ThingDefIndex != want[i] || v.start[i].ButtonIndex != want[i] + 100 || v.start[i].Priority != -want[i]) { std::printf("00593f39_TEST FAIL\n"); return 1; }
    free(v.start);
    std::printf("00593f39_TEST PASS\n");
    return 0;
}