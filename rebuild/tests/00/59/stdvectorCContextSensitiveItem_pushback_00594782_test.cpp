// Behaviour test for 0x00594782 std::vector<CContextSensitiveItem>::push_back (model copy)
#include <cstdio>
#include <cstdlib>
#include <new>
#include <stddef.h>
struct Item { int ThingDefIndex; int ButtonIndex; int Priority; };
struct Vec {
    Item* start; Item* finish; Item* eos;
    int overflows;
    void insert_overflow(Item* pos, const Item& x, size_t fill_len, bool atend)
    {
        ++overflows;
        size_t old = size_t(finish - start);
        size_t len = old + (old < fill_len ? fill_len : old);
        Item* ns = (Item*)malloc(len * sizeof(Item));
        Item* nf = ns;
        for (Item* p = start; p != pos; ++p, ++nf) new (nf) Item(*p);
        for (size_t i = 0; i < fill_len; ++i, ++nf) new (nf) Item(x);
        if (!atend) for (Item* p = pos; p != finish; ++p, ++nf) new (nf) Item(*p);
        if (start) free(start);
        start = ns; finish = nf; eos = ns + len;
    }
    void push_back(const Item& x)
    {
        if (finish != eos) { new (finish) Item(x); ++finish; }
        else insert_overflow(finish, x, 1, true);
    }
};
int main()
{
    Vec v; v.start = v.finish = v.eos = 0; v.overflows = 0;
    for (int i = 0; i < 5; ++i) { Item it; it.ThingDefIndex = i; it.ButtonIndex = 2 * i; it.Priority = 3 * i; v.push_back(it); }
    if (v.finish - v.start != 5 || v.overflows != 4 || v.eos - v.start != 8) { std::printf("00594782_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i) if (v.start[i].ThingDefIndex != i || v.start[i].Priority != 3 * i) { std::printf("00594782_TEST FAIL\n"); return 1; }
    free(v.start);
    std::printf("00594782_TEST PASS\n");
    return 0;
}