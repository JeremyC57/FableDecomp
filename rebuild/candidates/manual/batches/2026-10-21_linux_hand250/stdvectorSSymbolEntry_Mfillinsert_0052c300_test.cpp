// Behaviour test for vector<SSymbolEntry>::_M_fill_insert (0x0052C300): both in-place arms and the overflow hand-off.
#include <cstdio>
struct E { long v; };
struct Vec {
    E* s; E* f; E* e; int overflowCalls;
    void _M_insert_overflow(E*, const E&, unsigned) { ++overflowCalls; }
    void ins(E* pos, unsigned n, const E& x)
    {
        if (n != 0) {
            if (unsigned(e - f) >= n) {
                E xc = x;
                const unsigned after = f - pos;
                E* old = f;
                if (after > n) {
                    for (E *a = f - n, *r = f; a != f; ++a, ++r) *r = *a;
                    f += n;
                    for (E *l = old - n, *r = old; l != pos; ) *--r = *--l;
                    for (E* p = pos; p != pos + n; ++p) *p = xc;
                } else {
                    E* p = f; for (unsigned k = n - after; k > 0; --k, ++p) *p = xc;
                    f += n - after;
                    for (E *a = pos, *r = f; a != old; ++a, ++r) *r = *a;
                    f += after;
                    for (E* q = pos; q != old; ++q) *q = xc;
                }
            } else
                _M_insert_overflow(pos, x, n);
        }
    }
};
static bool check(const Vec& v, const long* want, int n)
{
    if (v.f - v.s != n) return false;
    for (int i = 0; i < n; ++i) if (v.s[i].v != want[i]) return false;
    return true;
}
int main()
{
    E buf[10]; for (int i = 0; i < 10; ++i) buf[i].v = i + 1;
    Vec a = { buf, buf + 5, buf + 10, 0 }; E x = { 99 };
    a.ins(buf + 1, 2, x);                                       // 1 99 99 2 3 4 5 (tail longer than n)
    long w1[] = { 1, 99, 99, 2, 3, 4, 5 };
    E b2[10]; for (int i = 0; i < 10; ++i) b2[i].v = i + 1;
    Vec b = { b2, b2 + 3, b2 + 10, 0 };
    b.ins(b2 + 2, 4, x);                                        // 1 2 99 99 99 99 3 (fill past the end first)
    long w2[] = { 1, 2, 99, 99, 99, 99, 3 };
    Vec c = { b2, b2 + 7, b2 + 8, 0 };
    c.ins(b2, 3, x);                                            // no room: overflow
    Vec d = { b2, b2 + 7, b2 + 8, 0 };
    d.ins(b2, 0, x);
    if (!check(a, w1, 7) || !check(b, w2, 7) || c.overflowCalls != 1 || d.overflowCalls != 0) { std::printf("0052c300_TEST FAIL\n"); return 1; }
    std::printf("0052c300_TEST PASS\n");
    return 0;
}
