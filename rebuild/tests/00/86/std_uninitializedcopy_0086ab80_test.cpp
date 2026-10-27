#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
#include <cstdio>
#include <new>
// Behaviour model: copy three 12-byte pairs into raw storage and return the end.
struct P { unsigned long k; long a, b; P(const P& o) : k(o.k), a(o.a), b(o.b) { } P(unsigned long x) : k(x), a(x * 2), b(x * 3) { } };
static P* ucopy(P* f, P* l, P* r) { for (; f != l; ++f, ++r) new ((void*)r) P(*f); return r; }
int main()
{
    P s[3] = { P(1), P(2), P(3) };
    static unsigned long raw[9];
    P* d = (P*)raw;
    if (ucopy(s, s + 3, d) != d + 3 || d[2].k != 3 || d[1].b != 6 || ucopy(s, s, d) != d) { std::printf("0086ab80_TEST FAIL\n"); return 1; }
    std::printf("0086ab80_TEST PASS\n");
    return 0;
}