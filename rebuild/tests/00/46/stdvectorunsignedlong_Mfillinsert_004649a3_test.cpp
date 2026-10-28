// behaviour test for 0x004649A3 std::vector<unsigned long>::_M_fill_insert (STLport 4.6: both in-capacity arms + overflow)
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct VecUL {
    unsigned long* Start; unsigned long* Finish; unsigned long* End;
    void init(unsigned n, unsigned cap) { Start = (unsigned long*)std::malloc(cap * sizeof(unsigned long)); for (unsigned i = 0; i < n; ++i) Start[i] = i + 1; Finish = Start + n; End = Start + cap; }
    void overflow(unsigned long* pos, const unsigned long& x, unsigned n)
    {
        unsigned old = (unsigned)(Finish - Start), len = old + (old > n ? old : n);
        unsigned long* ns = (unsigned long*)std::malloc(len * sizeof(unsigned long));
        unsigned long* nf = ns + (pos - Start);
        std::memmove(ns, Start, (pos - Start) * sizeof(unsigned long));
        for (unsigned i = 0; i < n; ++i) *nf++ = x;
        std::memmove(nf, pos, (Finish - pos) * sizeof(unsigned long)); nf += Finish - pos;
        std::free(Start); Start = ns; Finish = nf; End = ns + len;
    }
    void fill_insert(unsigned long* pos, unsigned n, const unsigned long& x)
    {
        if (n == 0) return;
        if ((unsigned)(End - Finish) >= n) {
            unsigned long xc = x;
            unsigned after = (unsigned)(Finish - pos);
            unsigned long* oldf = Finish;
            if (after > n) {
                std::memmove(Finish, Finish - n, n * sizeof(unsigned long)); Finish += n;
                std::memmove(pos + n, pos, (oldf - n - pos) * sizeof(unsigned long));
                for (unsigned long* p = pos; p != pos + n; ++p) *p = xc;
            } else {
                for (unsigned i = 0; i < n - after; ++i) Finish[i] = xc;
                Finish += n - after;
                std::memmove(Finish, pos, (oldf - pos) * sizeof(unsigned long)); Finish += after;
                for (unsigned long* p = pos; p != oldf; ++p) *p = xc;
            }
        } else overflow(pos, x, n);
    }
};
static bool eq(const VecUL& v, const unsigned long* e, unsigned n)
{
    if ((unsigned)(v.Finish - v.Start) != n) return false;
    for (unsigned i = 0; i < n; ++i) if (v.Start[i] != e[i]) return false;
    return true;
}
int main()
{
    VecUL a; a.init(4, 10); a.fill_insert(a.Start + 1, 2, 9);           // elems_after (3) > n (2)
    const unsigned long ea[] = { 1, 9, 9, 2, 3, 4 };
    VecUL b; b.init(4, 10); b.fill_insert(b.Start + 3, 3, 7);           // elems_after (1) <= n (3)
    const unsigned long eb[] = { 1, 2, 3, 7, 7, 7, 4 };
    VecUL c; c.init(3, 3); c.fill_insert(c.Start + 1, 2, 5);            // overflow: capacity 3 + max(3, 2)
    const unsigned long ec[] = { 1, 5, 5, 2, 3 };
    bool ok = eq(a, ea, 6) && eq(b, eb, 7) && eq(c, ec, 5) && (c.End - c.Start) == 6;
    std::free(a.Start); std::free(b.Start); std::free(c.Start);
    std::printf(ok ? "004649a3_TEST PASS\n" : "004649a3_TEST FAIL\n");
    return ok ? 0 : 1;
}