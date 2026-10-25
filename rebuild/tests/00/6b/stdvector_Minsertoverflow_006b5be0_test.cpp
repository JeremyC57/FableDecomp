// behaviour test for 0x006B5BE0 std::vector<pair<long, float> >::_M_insert_overflow: growth at the end (fill_len 1, atend), growth in the middle with fill_len 3 (head, fill, tail order), capacity size + max(size, fill_len)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <stddef.h>
#ifndef _MSC_VER
#define __cdecl
#endif
namespace tp { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }

using tp::pair;
typedef pair<long, float> T;
static T mk(unsigned int v) { T e; std::memset(&e, 0, sizeof(T)); std::memcpy(&e, &v, 4); return e; }
static unsigned int key(const T& e) { unsigned int w; std::memcpy(&w, &e, 4); return w; }
struct Vec {
    T* s; T* f; T* e;
    void ovf(T* pos, const T& x, unsigned int fill_len, bool atend)
    {
        unsigned int old = (unsigned int)(f - s);
        unsigned int len = old + (old < fill_len ? fill_len : old);
        T* ns = len ? (T*)std::malloc(len * sizeof(T)) : 0;
        T* nf = ns;
        for (T* p = s; p != pos; ++p, ++nf) new (nf) T(*p);
        if (fill_len == 1) { new (nf) T(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) T(x);
        if (!atend) for (T* p = pos; p != f; ++p, ++nf) new (nf) T(*p);
        if (s) std::free(s);
        s = ns; f = nf; e = ns + len;
    }
};
int main()
{
    int fails = 0;
    if ((unsigned int)sizeof(T) < 4u) fails |= 1;
    Vec v = { 0, 0, 0 };
    v.ovf(v.f, mk(10), 1, true);
    v.ovf(v.f, mk(20), 1, true);
    if (v.f - v.s != 2 || v.e - v.s != 2 || key(v.s[0]) != 10 || key(v.s[1]) != 20) fails |= 2;
    v.ovf(v.s + 1, mk(7), 3, false);                 // 10, 7, 7, 7, 20 in 2 + max(2, 3) = 5 slots
    const unsigned int want[5] = { 10, 7, 7, 7, 20 };
    if (v.f - v.s != 5 || v.e - v.s != 5) fails |= 4;
    for (int i = 0; i < 5 && !(fails & 4); ++i) if (key(v.s[i]) != want[i]) fails |= 8;
    v.ovf(v.f, mk(30), 1, true);                     // 5 + 5 = 10 slots
    if (v.f - v.s != 6 || v.e - v.s != 10 || key(v.s[5]) != 30) fails |= 16;
    std::free(v.s);
    if (fails) { std::printf("006b5be0_TEST FAIL %d\n", fails); return 1; }
    std::printf("006b5be0_TEST PASS\n");
    return 0;
}