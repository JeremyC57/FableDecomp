#include <cstdio>

struct Out { long n; float v[8]; Out() : n(0) {} void WriteFloat(float x) { v[n++] = x; } };
struct B { float cx, cy, cz, dx, dy, dz; void WriteToFile(Out& o) const { o.WriteFloat(cx); o.WriteFloat(cy); o.WriteFloat(cz); o.WriteFloat(dx); o.WriteFloat(dy); o.WriteFloat(dz); } };
int main()
{
    B b = { 1, 2, 3, 4, 5, 6 }; Out o; b.WriteToFile(o); if (o.n != 6 || o.v[0] != 1 || o.v[5] != 6) { std::printf("00aa4b90_TEST FAIL\n"); return 1; }
    std::printf("00aa4b90_TEST PASS\n");
    return 0;
}