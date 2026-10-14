#include <cstdio>

struct Out { long n; unsigned long v[4]; Out() : n(0) {} void WriteULong(unsigned long x) { v[n++] = x; } };
struct P { unsigned long a, b; void Base(Out& o) const { o.v[o.n++] = 300; } void WriteToFile(Out& o) const { o.WriteULong(a); o.WriteULong(b); Base(o); } };
int main()
{
    P p = { 4, 2 }; Out o; p.WriteToFile(o); if (o.n != 3 || o.v[0] != 4 || o.v[1] != 2 || o.v[2] != 300) { std::printf("00aa4ca0_TEST FAIL\n"); return 1; }
    std::printf("00aa4ca0_TEST PASS\n");
    return 0;
}
