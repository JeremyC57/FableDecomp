#include <cstdio>
static int g_d = 0; struct O; struct IP { void* l; O* d; O* G() { return d; } void S(O* o) { d = o; } ~IP() { ++g_d; } };
struct X { unsigned char pad[8]; IP m; void Set(IP i) { m.S(i.G()); } };
int main()
{
    X x; x.m.d = 0; IP i; i.d = (O*)&x; x.Set(i); if (x.m.d != (O*)&x || g_d != 1) { std::printf("008bd640_TEST FAIL\n"); return 1; }

    std::printf("008bd640_TEST PASS\n");
    return 0;
}
