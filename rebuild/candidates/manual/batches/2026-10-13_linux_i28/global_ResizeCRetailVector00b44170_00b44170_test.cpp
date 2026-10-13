#include <cstdio>
#include <cstring>
static int g_i = 0; struct V { long* f; long* l; long* e; void IN(long*, unsigned long c, const long&) { g_i += (int)c; }
    static long* CD(long* s, long* se, long* d) { if (s == se) return d; unsigned long b = (unsigned long)((char*)se - (char*)s); return (long*)((char*)std::memmove(d, s, b) + b); }
    void R(unsigned long n, const long& v) { if (n < (unsigned long)(l - f)) l = CD(l, l, f + n); else IN(l, n - (l - f), v); } };
int main()
{
    long buf[4]; V v; v.f = buf; v.l = buf + 3; v.e = buf + 4; long x = 1; v.R(1, x); if (v.l != buf + 1) { std::printf("00b44170_TEST FAIL\n"); return 1; }
    v.R(3, x); if (g_i != 2) { std::printf("00b44170_TEST FAIL\n"); return 1; }

    std::printf("00b44170_TEST PASS\n");
    return 0;
}
