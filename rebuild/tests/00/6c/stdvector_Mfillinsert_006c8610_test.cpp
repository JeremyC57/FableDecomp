// behaviour test for 0x006C8610 std::vector<pair<long, COpinionDeed*> >::_M_fill_insert: in-place insert with a long tail (elems_after > n), with a short tail (fill past the end, then move the tail), n == 0 no-op, and the growth path through _M_insert_overflow
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <stddef.h>
#ifndef _MSC_VER
#define __cdecl
#endif
namespace tp { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
class COpinionDeed;
using tp::pair;
typedef pair<long, COpinionDeed*> T;
static T mk(unsigned int v) { T e; std::memset(&e, 0, sizeof(T)); std::memcpy(&e, &v, 4); return e; }
static unsigned int key(const T& e) { unsigned int w; std::memcpy(&w, &e, 4); return w; }
static int gOverflows = 0;
struct Vec {
    T* s; T* f; T* e;
    void ovf(T* pos, const T& x, unsigned int fill_len)
    {
        ++gOverflows;
        unsigned int old = (unsigned int)(f - s);
        unsigned int len = old + (old < fill_len ? fill_len : old);
        T* ns = (T*)std::malloc(len * sizeof(T));
        T* nf = ns;
        for (T* p = s; p != pos; ++p, ++nf) new (nf) T(*p);
        for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) T(x);
        for (T* p = pos; p != f; ++p, ++nf) new (nf) T(*p);
        std::free(s);
        s = ns; f = nf; e = ns + len;
    }
    void fill_insert(T* pos, unsigned int n, const T& x)
    {
        if (n == 0) return;
        if ((unsigned int)(e - f) >= n) {
            T xc = x;
            unsigned int after = (unsigned int)(f - pos);
            T* old = f;
            if (after > n) {
                for (T* p = f - n, *d = f; p != f; ++p, ++d) new (d) T(*p);
                f += n;
                for (T* p = old - n, *d = old; p != pos; ) *--d = *--p;
                for (T* p = pos; p != pos + n; ++p) *p = xc;
            } else {
                for (unsigned int k = n - after; k > 0; --k, ++f) new (f) T(xc);
                for (T* p = pos; p != old; ++p, ++f) new (f) T(*p);
                for (T* p = pos; p != old; ++p) *p = xc;
            }
        } else
            ovf(pos, x, n);
    }
};
static int Same(const Vec& v, const unsigned int* want, unsigned int n)
{
    if ((unsigned int)(v.f - v.s) != n) return 0;
    for (unsigned int i = 0; i < n; ++i) if (key(v.s[i]) != want[i]) return 0;
    return 1;
}
int main()
{
    int fails = 0;
    Vec v; v.s = (T*)std::malloc(10 * sizeof(T)); v.f = v.s; v.e = v.s + 10;
    for (unsigned int i = 1; i <= 5; ++i) { new (v.f) T(mk(i)); ++v.f; }
    v.fill_insert(v.s + 1, 2, mk(9));                      // long tail: 1 9 9 2 3 4 5
    const unsigned int w1[] = { 1, 9, 9, 2, 3, 4, 5 };
    if (!Same(v, w1, 7) || gOverflows != 0) fails |= 1;
    v.fill_insert(v.s + 6, 2, mk(8));                      // short tail: 1 9 9 2 3 4 8 8 5
    const unsigned int w2[] = { 1, 9, 9, 2, 3, 4, 8, 8, 5 };
    if (!Same(v, w2, 9) || gOverflows != 0) fails |= 2;
    v.fill_insert(v.s, 0, mk(7));
    if (!Same(v, w2, 9)) fails |= 4;
    v.fill_insert(v.s, 3, mk(6));                          // growth: 6 6 6 1 9 9 2 3 4 8 8 5
    const unsigned int w3[] = { 6, 6, 6, 1, 9, 9, 2, 3, 4, 8, 8, 5 };
    if (!Same(v, w3, 12) || gOverflows != 1 || v.e - v.s != 18) fails |= 8;
    std::free(v.s);
    if (fails) { std::printf("006c8610_TEST FAIL %d\n", fails); return 1; }
    std::printf("006c8610_TEST PASS\n");
    return 0;
}