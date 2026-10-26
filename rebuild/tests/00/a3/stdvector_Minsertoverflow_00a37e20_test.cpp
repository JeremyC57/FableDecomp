// behaviour test for 0x00A37E20 std::vector<CClassTypeInfo >::_M_insert_overflow: the grown block holds the head, x (once or fill_len
// times) and, unless atend, the tail; capacity is size + max(size, fill_len); the old elements are destroyed and the old block freed
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_live = 0, g_copies = 0;
struct Elem {                                              // 20-byte element with an out-of-line style copy
    int v; int pad[4];
    Elem(int x) : v(x) { ++g_live; }
    Elem(const Elem& o) : v(o.v) { ++g_live; ++g_copies; }
    ~Elem() { --g_live; }
};
struct Vec { Elem* start; Elem* finish; Elem* eos; };
static void insert_overflow(Vec& vec, Elem* pos, const Elem& x, unsigned int fill_len, bool atend)
{
    unsigned int old_size = (unsigned int)(vec.finish - vec.start);
    unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
    Elem* ns = len != 0 ? (Elem*)std::malloc(len * sizeof(Elem)) : 0;
    Elem* nf = ns;
    for (Elem* p = vec.start; p != pos; ++p, ++nf) new (nf) Elem(*p);
    if (fill_len == 1) { new (nf) Elem(x); ++nf; }
    else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) Elem(x);
    if (!atend) for (Elem* p = pos; p != vec.finish; ++p, ++nf) new (nf) Elem(*p);
    for (Elem* p = vec.start; p != vec.finish; ++p) p->~Elem();
    if (vec.start) std::free(vec.start);
    vec.start = ns; vec.finish = nf; vec.eos = ns + len;
}
static void make(Vec& vec, int n, int cap)
{
    vec.start = (Elem*)std::malloc(cap * sizeof(Elem));
    for (int i = 0; i < n; ++i) new (vec.start + i) Elem(10 + i);
    vec.finish = vec.start + n; vec.eos = vec.start + cap;
}
static void kill(Vec& vec) { for (Elem* p = vec.start; p != vec.finish; ++p) p->~Elem(); std::free(vec.start); }
int main()
{
    int fail = 0;
    if (sizeof(Elem) != 20u) fail |= 1;
    Vec a; make(a, 3, 3);
    Elem x(99);
    insert_overflow(a, a.start + 1, x, 1, false);          // push_back-style single insert in the middle
    if (a.finish - a.start != 4 || a.eos - a.start != 6) fail |= 2;
    if (a.start[0].v != 10 || a.start[1].v != 99 || a.start[2].v != 11 || a.start[3].v != 12) fail |= 4;
    insert_overflow(a, a.finish, x, 7, true);              // fill at the end: len = 4 + max(4, 7)
    if (a.finish - a.start != 11 || a.eos - a.start != 11) fail |= 8;
    for (int i = 4; i < 11; ++i) if (a.start[i].v != 99) fail |= 16;
    if (g_live != 12) fail |= 32;                          // eleven in the block plus x
    kill(a);
    Vec e; e.start = e.finish = e.eos = 0;
    insert_overflow(e, e.finish, x, 1, true);              // first push_back into an empty vector
    if (e.finish - e.start != 1 || e.eos - e.start != 1 || e.start[0].v != 99) fail |= 64;
    kill(e);
    if (g_live != 1) fail |= 128;
    if (fail) { std::printf("00a37e20_TEST FAIL %d\n", fail); return 1; }
    std::printf("00a37e20_TEST PASS\n");
    return 0;
}