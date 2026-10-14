#include <cstdio>

struct Out { long n; float v[8]; Out() : n(0) {} void WriteFloat(float x) { v[n++] = x; } void WriteLong(long x) { v[n++] = (float)x; } void WriteString() { v[n++] = 200; } };
struct HP { float x, y, z; long a, b; void Base(Out& o) const { o.v[o.n++] = 300; }
    void WriteToFile(Out& o) const { o.WriteFloat(x); o.WriteFloat(y); o.WriteFloat(z); o.WriteLong(a); o.WriteLong(b); o.WriteString(); Base(o); } };
int main()
{
    HP h = { 1, 2, 3, 4, 5 }; Out o; h.WriteToFile(o);
    if (o.n != 7 || o.v[2] != 3 || o.v[4] != 5 || o.v[5] != 200 || o.v[6] != 300) { std::printf("00aa98b0_TEST FAIL\n"); return 1; }
    std::printf("00aa98b0_TEST PASS\n");
    return 0;
}