#include <cstdio>
static int g_p = -1; struct V { virtual void v0() {} virtual void Set(bool p) { g_p = p; } };
struct S { V* t; ~S() { t->Set(false); } };
int main()
{
    V v; { S s; s.t = &v; } if (g_p != 0) { std::printf("00cbe0b3_TEST FAIL\n"); return 1; }

    std::printf("00cbe0b3_TEST PASS\n");
    return 0;
}
