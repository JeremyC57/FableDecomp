#include <cstdio>

struct Out { long n; float v[8]; Out() : n(0) {} void WriteULong(unsigned long x) { v[n++] = (float)x; } void WriteFloat(float x) { v[n++] = x; } void WriteMatrix() { v[n++] = 100; } void WriteBool(bool b) { v[n++] = b ? 1.0f : 0.0f; } };
struct DO { float x, y, z; bool flag; void Base(Out& o) const { o.v[o.n++] = 300; }
    void WriteToFile(Out& o) const { o.WriteULong(3); o.WriteFloat(x); o.WriteFloat(y); o.WriteFloat(z); o.WriteMatrix(); o.WriteBool(flag); Base(o); } };
int main()
{
    DO d = { 1, 2, 3, true }; Out o; d.WriteToFile(o);
    if (o.n != 7 || o.v[0] != 3 || o.v[3] != 3 || o.v[4] != 100 || o.v[5] != 1 || o.v[6] != 300) { std::printf("00aa9a70_TEST FAIL\n"); return 1; }
    std::printf("00aa9a70_TEST PASS\n");
    return 0;
}