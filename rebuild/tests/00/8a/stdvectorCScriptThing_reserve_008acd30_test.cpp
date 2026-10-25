// Behaviour test for 0x008ACD30 vector<CScriptThing>::reserve(n): only when capacity() < n the block is replaced by n slots holding copies of the elements (the old ones cleared and freed); an empty vector just allocates; size is kept.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_copies = 0, g_cleared = 0;
struct Elem {
    int a, b, c;
    Elem(int v) : a(v), b(v + 1), c(v + 2) { }
    Elem(const Elem& o) : a(o.a), b(o.b), c(o.c) { ++g_copies; }
};
struct Vec {
    Elem* start; Elem* finish; Elem* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    unsigned int capacity() const { return (unsigned int)(eos - start); }
    void clear() { g_cleared += (int)size(); if (start) std::free(start); }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            unsigned int old_size = size();
            Elem* tmp;
            if (start) {
                tmp = n ? (Elem*)std::malloc(n * sizeof(Elem)) : 0;
                Elem* cur = tmp;
                for (Elem* p = start; p != finish; ++p, ++cur) new (cur) Elem(*p);
                clear();
            } else
                tmp = n ? (Elem*)std::malloc(n * sizeof(Elem)) : 0;
            start = tmp; finish = tmp + old_size; eos = tmp + n;
        }
    }
};
static int fail() { std::printf("008acd30_TEST FAIL\n"); return 1; }
int main()
{
    if (sizeof(Elem) != 12) return fail();
    Vec v; v.start = 0; v.finish = 0; v.eos = 0;
    v.reserve(0);                                      // capacity 0 is not < 0: nothing
    if (v.start != 0 || g_copies != 0) return fail();
    v.reserve(2);                                      // empty: allocate only
    if (v.start == 0 || v.size() != 0 || v.capacity() != 2 || g_copies != 0) return fail();
    new (v.finish++) Elem(10); new (v.finish++) Elem(20);
    g_copies = 0;
    v.reserve(2);                                      // enough room: untouched
    if (v.capacity() != 2 || g_copies != 0) return fail();
    v.reserve(5);                                      // grow: copy two, clear two
    if (v.capacity() != 5 || v.size() != 2 || g_copies != 2 || g_cleared != 2 || v.start[1].a != 20 || v.start[1].c != 22) return fail();
    std::free(v.start);
    std::printf("008acd30_TEST PASS\n");
    return 0;
}