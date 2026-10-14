#include <cstdio>

struct Out { long n; long v[4]; Out() : n(0) {} void WriteMatrix() { v[n++] = 100; } };
struct T { void Base(Out& o) const { o.v[o.n++] = 300; } void WriteToFile(Out& o) const { o.WriteMatrix(); Base(o); } };
int main()
{
    T t; Out o; t.WriteToFile(o); if (o.n != 2 || o.v[0] != 100 || o.v[1] != 300) { std::printf("00ab9cb0_TEST FAIL\n"); return 1; }
    std::printf("00ab9cb0_TEST PASS\n");
    return 0;
}