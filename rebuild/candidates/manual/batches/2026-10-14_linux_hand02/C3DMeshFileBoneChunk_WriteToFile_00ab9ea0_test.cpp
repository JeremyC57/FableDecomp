#include <cstdio>

struct Out { long n; long v[8]; Out() : n(0) {} void WriteLong(long x) { v[n++] = x; } void WriteMatrix() { v[n++] = 100; } void WriteString() { v[n++] = 200; } };
struct Bone { long a, b, c, d; void Base(Out& o) const { o.v[o.n++] = 300; }
    void WriteToFile(Out& o) const { o.WriteLong(a); o.WriteLong(b); o.WriteLong(c); o.WriteLong(d); o.WriteMatrix(); o.WriteString(); Base(o); } };
int main()
{
    Bone b = { 1, 2, 3, 4 }; Out o; b.WriteToFile(o);
    if (o.n != 7 || o.v[0] != 1 || o.v[3] != 4 || o.v[4] != 100 || o.v[5] != 200 || o.v[6] != 300) { std::printf("00ab9ea0_TEST FAIL\n"); return 1; }
    std::printf("00ab9ea0_TEST PASS\n");
    return 0;
}
