// behaviour test for 0x004124C0 vector<CDisplayModeDesc >::_M_insert_overflow: the new block holds size + max(size, fill_len) 16-byte slots, the head [start, position) is copied, x lands once (or fill_len times), the tail [position, finish) follows unless atend, and the vector adopts the new block
#include <cstdio>
#include <cstdlib>
struct Elem16 { int w[4]; };                                      // the 16-byte element's four words
struct Vec { Elem16* start; Elem16* finish; Elem16* eos; };
static Elem16 Make(int k) { Elem16 e; for (int i = 0; i < 4; ++i) e.w[i] = k * 10 + i; return e; }
static bool Same(const Elem16& a, const Elem16& b) { for (int i = 0; i < 4; ++i) if (a.w[i] != b.w[i]) return false; return true; }
static void InsertOverflow(Vec& v, Elem16* position, const Elem16& x, unsigned int fill_len, bool atend)   // the function under test
{
    unsigned int old_size = (unsigned int)(v.finish - v.start);
    unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
    Elem16* ns = len ? (Elem16*)std::malloc(len * sizeof(Elem16)) : 0;
    Elem16* nf = ns;
    for (Elem16* p = v.start; p != position; ++p, ++nf) *nf = *p;      // __uninitialized_copy (head)
    if (fill_len == 1) { *nf = x; ++nf; }
    else for (unsigned int n = fill_len; n > 0; --n, ++nf) *nf = x;    // __uninitialized_fill_n
    if (!atend) for (Elem16* p = position; p != v.finish; ++p, ++nf) *nf = *p;
    if (v.start) std::free(v.start);
    v.start = ns; v.finish = nf; v.eos = ns + len;
}
int main()
{
    int fail = 0;
    if (sizeof(Elem16) != (unsigned int)16) fail |= 8;
    Vec v; v.start = (Elem16*)std::malloc(3 * sizeof(Elem16)); v.finish = v.start + 3; v.eos = v.finish;
    for (int i = 0; i < 3; ++i) v.start[i] = Make(i + 1);
    Elem16 x = Make(9);
    InsertOverflow(v, v.start + 2, x, 4, false);                      // insert four copies before the last element
    if (v.finish - v.start != 7 || v.eos - v.start != 7) fail |= 1;   // 3 + max(3, 4)
    for (int i = 0; i < 7; ++i) {
        Elem16 want = i < 2 ? Make(i + 1) : i < 6 ? Make(9) : Make(3);
        if (!Same(v.start[i], want)) fail |= 2;
    }
    Elem16 y = Make(5);
    InsertOverflow(v, v.finish, y, 1, true);                           // push_back growth
    if (v.finish - v.start != 8 || v.eos - v.start != 14 || !Same(v.start[7], y) || !Same(v.start[6], Make(3))) fail |= 4;
    std::free(v.start);
    if (fail) { std::printf("004124c0_TEST FAIL %d\n", fail); return 1; }
    std::printf("004124c0_TEST PASS\n");
    return 0;
}