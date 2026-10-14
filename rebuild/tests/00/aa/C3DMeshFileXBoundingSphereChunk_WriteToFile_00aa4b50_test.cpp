#include <cstdio>

struct Out { long n; float v[8]; Out() : n(0) {} void WriteFloat(float x) { v[n++] = x; } };
struct B { float cx, cy, cz, r; void WriteToFile(Out& o) const { o.WriteFloat(cx); o.WriteFloat(cy); o.WriteFloat(cz); o.WriteFloat(r); } };
int main()
{
    B b = { 1, 2, 3, 4 }; Out o; b.WriteToFile(o); if (o.n != 4 || o.v[0] != 1 || o.v[3] != 4) { std::printf("00aa4b50_TEST FAIL\n"); return 1; }
    std::printf("00aa4b50_TEST PASS\n");
    return 0;
}