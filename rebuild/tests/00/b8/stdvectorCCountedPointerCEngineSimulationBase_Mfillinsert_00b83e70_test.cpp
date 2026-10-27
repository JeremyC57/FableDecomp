#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour model of STLport vector::_M_fill_insert (in-place arms; the overflow arm is a callee).
struct E { int v; E(int x) : v(x) { } E(const E& o) : v(o.v) { } E& operator=(const E& o) { v = o.v; return *this; } };
struct V { E* s; E* f; E* e; int overflow; };
static void fill_insert(V& vec, E* pos, unsigned int n, const E& x)
{
    if (n == 0) return;
    if ((unsigned int)(vec.e - vec.f) >= n) {
        E x_copy = x;
        unsigned int after = (unsigned int)(vec.f - pos);
        E* old_finish = vec.f;
        if (after > n) {
            E* d = vec.f; for (E* p = vec.f - n; p != vec.f; ++p, ++d) new (d) E(*p);
            vec.f += n;
            E* r = old_finish; for (E* p = old_finish - n; p != pos; ) *--r = *--p;
            for (E* p = pos; p != pos + n; ++p) *p = x_copy;
        } else {
            E* d = vec.f; for (unsigned int k = n - after; k > 0; --k, ++d) new (d) E(x_copy);
            vec.f += n - after;
            d = vec.f; for (E* p = pos; p != old_finish; ++p, ++d) new (d) E(*p);
            vec.f += after;
            for (E* p = pos; p != old_finish; ++p) *p = x_copy;
        }
    } else
        ++vec.overflow;
}
int main()
{
    E* buf = (E*)std::malloc(10 * sizeof(E));
    V vec; vec.s = buf; vec.f = buf + 4; vec.e = buf + 10; vec.overflow = 0;
    for (int i = 0; i < 4; ++i) new (buf + i) E(i + 1);          // 1 2 3 4
    fill_insert(vec, buf + 1, 2, E(9));                           // 1 9 9 2 3 4  (after = 3 > 2)
    static const int a[6] = { 1, 9, 9, 2, 3, 4 };
    for (int i = 0; i < 6; ++i) if (buf[i].v != a[i]) { std::printf("00b83e70_TEST FAIL\n"); return 1; }
    fill_insert(vec, buf + 5, 3, E(7));                           // 1 9 9 2 3 7 7 7 4 (after = 1 <= 3)
    static const int b[9] = { 1, 9, 9, 2, 3, 7, 7, 7, 4 };
    if (vec.f - vec.s != 9) { std::printf("00b83e70_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 9; ++i) if (buf[i].v != b[i]) { std::printf("00b83e70_TEST FAIL\n"); return 1; }
    fill_insert(vec, buf, 0, E(5));                               // n == 0: nothing
    fill_insert(vec, buf, 2, E(5));                               // room for 1 only: overflow arm
    if (vec.overflow != 1 || vec.f - vec.s != 9) { std::printf("00b83e70_TEST FAIL\n"); return 1; }
    std::printf("00b83e70_TEST PASS\n");
    return 0;
}