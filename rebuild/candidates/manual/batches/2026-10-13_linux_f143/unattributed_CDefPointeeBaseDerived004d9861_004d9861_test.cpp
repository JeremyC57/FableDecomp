#include <cstdio>
static int g_d = 0; struct S { S() {} ~S() { ++g_d; } }; struct Base { virtual void v00() {} ~Base() { g_d += 10; } };
struct Mid : Base { virtual void v00() {} ~Mid() {} }; struct D : Mid { unsigned char pad[0x24]; S a; S b; virtual void v00() {} ~D() {} };
int main()
{
    { D d; } if (g_d != 12) { std::printf("004d9861_TEST FAIL\n"); return 1; }

    std::printf("004d9861_TEST PASS\n");
    return 0;
}
