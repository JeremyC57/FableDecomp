#include <cstdio>

struct Out { long n; long v[8]; Out() : n(0) {} void WriteLong(long x) { v[n++] = x; } void WriteString(const char* s) { v[n++] = s[0]; } };
struct Map { long i; const char* a; const char* b; void Base(Out& o) const { o.v[o.n++] = 300; }
    void WriteToFile(Out& o) const { o.WriteLong(i); o.WriteString(a); o.WriteString(b); Base(o); } };
int main()
{
    Map m = { 4, "x", "y" }; Out o; m.WriteToFile(o);
    if (o.n != 4 || o.v[0] != 4 || o.v[1] != 'x' || o.v[2] != 'y' || o.v[3] != 300) { std::printf("00abff70_TEST FAIL\n"); return 1; }
    std::printf("00abff70_TEST PASS\n");
    return 0;
}