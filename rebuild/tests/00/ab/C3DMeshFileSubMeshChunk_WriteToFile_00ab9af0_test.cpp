#include <cstdio>

struct Out { long n; long v[8]; Out() : n(0) {} void WriteLong(long x) { v[n++] = x; } void WriteString(const char* s) { v[n++] = s[0]; } };
struct Sub { const char* name; long a, b, c, d; void Base(Out& o) const { o.v[o.n++] = 300; }
    void WriteToFile(Out& o) const { o.WriteString(name); o.WriteLong(a); o.WriteLong(b); o.WriteLong(c); o.WriteLong(d); Base(o); } };
int main()
{
    Sub s = { "n", 1, 2, 3, 4 }; Out o; s.WriteToFile(o);
    if (o.n != 6 || o.v[0] != 'n' || o.v[1] != 1 || o.v[4] != 4 || o.v[5] != 300) { std::printf("00ab9af0_TEST FAIL\n"); return 1; }
    std::printf("00ab9af0_TEST PASS\n");
    return 0;
}