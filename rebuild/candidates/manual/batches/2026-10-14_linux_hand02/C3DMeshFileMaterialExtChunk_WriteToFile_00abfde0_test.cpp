#include <cstdio>

struct Out { long n; unsigned long v[4]; Out() : n(0) {} void WriteULong(unsigned long x) { v[n++] = x; } };
struct Ext { unsigned char pad[0x10]; unsigned long SelfIllumination; void WriteToFile(Out& o) const { o.WriteULong(1); o.WriteULong(SelfIllumination); } };
int main()
{
    Ext e; e.SelfIllumination = 77; Out o; e.WriteToFile(o);
    if (o.n != 2 || o.v[0] != 1 || o.v[1] != 77) { std::printf("00abfde0_TEST FAIL\n"); return 1; }
    std::printf("00abfde0_TEST PASS\n");
    return 0;
}
