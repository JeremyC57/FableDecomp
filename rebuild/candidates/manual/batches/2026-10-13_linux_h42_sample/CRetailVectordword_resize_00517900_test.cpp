#include <cstdio>
#include <cstring>
static int g_i = 0;
struct V { long* f; long* l; long* e; void I(long*, unsigned long c, const long&) { g_i += (int)c; }
  static long* C(long* a, long* b, long* r) { if (a != b) { unsigned long n = (char*)b - (char*)a; r = (long*)((char*)std::memmove(r, a, n) + n); } return r; }
  long* E(long* a, long* b) { long* s = C(b, l, a); l = s; return a; }
  void R(unsigned long n, const long& v) { if (n < (unsigned long)(l - f)) E(f + n, l); else I(l, n - (l - f), v); } };
int main()
{
    long buf[4]; V v; v.f = buf; v.l = buf + 4; v.e = buf + 4; long x = 0; v.R(2, x); if (v.l != buf + 2) { std::printf("00517900_TEST FAIL\n"); return 1; }
    v.R(5, x); if (g_i != 3) { std::printf("00517900_TEST FAIL\n"); return 1; }

    std::printf("00517900_TEST PASS\n");
    return 0;
}
