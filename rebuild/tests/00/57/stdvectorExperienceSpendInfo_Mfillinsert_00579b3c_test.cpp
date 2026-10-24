// Behaviour test for 0x00579B3C vector<ExperienceSpendInfo>::_M_fill_insert: in-place insert of n copies (both the
// elems_after > n and the elems_after <= n arms, x aliasing an element), the overflow path, and n == 0.
#include <cstdio>
#include <cstdlib>
#include <new>

struct E { int currentSpend; int initialSpend; int levelUpSpend; };
static E Make(int k) { E e; e.currentSpend = k; e.initialSpend = k * 10; e.levelUpSpend = -k; return e; }
static bool Is(const E& e, int k) { return e.currentSpend == k && e.initialSpend == k * 10 && e.levelUpSpend == -k; }
static int g_overflows = 0;

struct Vec {
    E* start; E* finish; E* eos;
    void InsertOverflow(E* pos, const E& x, unsigned int fill_len)
    {
        ++g_overflows;
        unsigned int old_size = (unsigned int)(finish - start);
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        E* ns = (E*)std::malloc(len * sizeof(E)); E* nf = ns;
        for (E* p = start; p != pos; ++p, ++nf) new (nf) E(*p);
        for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) E(x);
        for (E* p = pos; p != finish; ++p, ++nf) new (nf) E(*p);
        std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
    void FillInsert(E* pos, unsigned int n, const E& x)
    {
        if (n != 0) {
            if ((unsigned int)(eos - finish) >= n) {
                E x_copy = x;
                const unsigned int elems_after = (unsigned int)(finish - pos);
                E* old_finish = finish;
                if (elems_after > n) {
                    E* r = finish; for (E* p = finish - n; p != finish; ++p, ++r) new (r) E(*p);
                    finish += n;
                    E* d = old_finish; for (E* s = old_finish - n; s != pos; ) *--d = *--s;
                    for (E* p = pos; p != pos + n; ++p) *p = x_copy;
                } else {
                    E* r = finish; for (unsigned int k = n - elems_after; k > 0; --k, ++r) new (r) E(x_copy);
                    finish += n - elems_after;
                    r = finish; for (E* p = pos; p != old_finish; ++p, ++r) new (r) E(*p);
                    finish += elems_after;
                    for (E* p = pos; p != old_finish; ++p) *p = x_copy;
                }
            } else
                InsertOverflow(pos, x, n);
        }
    }
};

int main()
{
    Vec v;
    v.start = (E*)std::malloc(10 * sizeof(E));
    for (int i = 0; i < 4; ++i) v.start[i] = Make(i + 1);     // 1 2 3 4, capacity 10
    v.finish = v.start + 4; v.eos = v.start + 10;
    v.FillInsert(v.start + 1, 2, v.start[3]);                  // elems_after 3 > 2: 1 4 4 2 3 4 (x aliases)
    bool ok = v.finish - v.start == 6 && Is(v.start[0], 1) && Is(v.start[1], 4) && Is(v.start[2], 4) &&
              Is(v.start[3], 2) && Is(v.start[4], 3) && Is(v.start[5], 4) && g_overflows == 0;
    v.FillInsert(v.start + 5, 3, Make(8));                     // elems_after 1 <= 3: 1 4 4 2 3 8 8 8 4
    ok = ok && v.finish - v.start == 9 && Is(v.start[5], 8) && Is(v.start[7], 8) && Is(v.start[8], 4) && Is(v.start[4], 3);
    v.FillInsert(v.start, 0, Make(5));                         // no-op
    ok = ok && v.finish - v.start == 9;
    v.FillInsert(v.finish, 2, Make(6));                        // capacity 10 < 11: overflow
    ok = ok && g_overflows == 1 && v.finish - v.start == 11 && Is(v.start[9], 6) && Is(v.start[10], 6) && Is(v.start[0], 1);
    std::free(v.start);
    std::printf(ok ? "00579b3c_TEST PASS\n" : "00579b3c_TEST FAIL\n");
    return ok ? 0 : 1;
}