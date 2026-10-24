// Behaviour test for 0x00580928 std::vector<CHeroMarriageInfo>::_M_fill_insert (model copy)
#include <cstdio>
#include <new>
struct Rec { unsigned int v[6]; };
static Rec* ucopy(Rec* f, Rec* l, Rec* r) { for (; f != l; ++f, ++r) new (r) Rec(*f); return r; }
static Rec* ufill_n(Rec* f, unsigned int n, const Rec& x) { for (; n > 0; --n, ++f) new (f) Rec(x); return f; }
static Rec* copy_backward(Rec* f, Rec* l, Rec* r) { for (int n = int(l - f); n > 0; --n) *--r = *--l; return r; }
static void fill(Rec* f, Rec* l, const Rec& x) { for (; f != l; ++f) *f = x; }
struct Vec {
    Rec* start; Rec* finish; Rec* eos; int overflowCalls; unsigned int overflowN;
    void insert_overflow(Rec*, const Rec&, unsigned int n) { ++overflowCalls; overflowN = n; }
    void fill_insert(Rec* pos, unsigned int n, const Rec& x)
    {
        if (n != 0) {
            if ((unsigned int)(eos - finish) >= n) {
                Rec xc = x;
                const unsigned int after = finish - pos;
                Rec* old = finish;
                if (after > n) {
                    ucopy(finish - n, finish, finish); finish += n;
                    copy_backward(pos, old - n, old); fill(pos, pos + n, xc);
                } else {
                    ufill_n(finish, n - after, xc); finish += n - after;
                    ucopy(pos, old, finish); finish += after; fill(pos, old, xc);
                }
            } else insert_overflow(pos, x, n);
        }
    }
};
static Rec R(unsigned int k) { Rec r; for (int i = 0; i < 6; ++i) r.v[i] = k; return r; }
int main()
{
    Rec buf[10];
    Vec v; v.start = buf; v.finish = buf; v.eos = buf + 10; v.overflowCalls = 0;
    for (unsigned int i = 0; i < 5; ++i) *v.finish++ = R(i);       // 0 1 2 3 4
    v.fill_insert(buf + 1, 2, R(9));                                // 0 9 9 1 2 3 4  (after=4 > n)
    unsigned int want1[7] = { 0, 9, 9, 1, 2, 3, 4 };
    for (int i = 0; i < 7; ++i) if (buf[i].v[5] != want1[i]) { std::printf("00580928_TEST FAIL\n"); return 1; }
    if (v.finish != buf + 7) { std::printf("00580928_TEST FAIL\n"); return 1; }
    v.fill_insert(buf + 6, 2, R(7));                                // 0 9 9 1 2 3 7 7 4  (after=1 <= n)
    unsigned int want2[9] = { 0, 9, 9, 1, 2, 3, 7, 7, 4 };
    for (int i = 0; i < 9; ++i) if (buf[i].v[0] != want2[i]) { std::printf("00580928_TEST FAIL\n"); return 1; }
    v.fill_insert(buf, 0, R(1));
    v.fill_insert(buf, 3, R(1));                                    // only 1 slot left -> overflow
    if (v.finish != buf + 9 || v.overflowCalls != 1 || v.overflowN != 3) { std::printf("00580928_TEST FAIL\n"); return 1; }
    std::printf("00580928_TEST PASS\n");
    return 0;
}