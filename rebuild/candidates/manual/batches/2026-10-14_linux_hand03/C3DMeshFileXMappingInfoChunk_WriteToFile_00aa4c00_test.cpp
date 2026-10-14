#include <cstdio>

struct Out { long n; float v[4]; Out() : n(0) {} void WriteULong(unsigned long x) { v[n++] = (float)x; } void WriteFloat(float x) { v[n++] = x; } };
struct M { float stretch; void WriteToFile(Out& o) const { o.WriteULong(1); o.WriteFloat(stretch); } };
int main()
{
    M m = { 2.5f }; Out o; m.WriteToFile(o); if (o.n != 2 || o.v[0] != 1 || o.v[1] != 2.5f) { std::printf("00aa4c00_TEST FAIL\n"); return 1; }
    std::printf("00aa4c00_TEST PASS\n");
    return 0;
}
