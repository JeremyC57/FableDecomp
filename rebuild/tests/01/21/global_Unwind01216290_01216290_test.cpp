#include <cstdio>
#include <cstdlib>
static int g_c = 0; struct B { virtual void v() {} unsigned char b[0x84]; B() { ++g_c; } }; struct D : B { static void* operator new(unsigned int n) { return std::malloc(n); } static void operator delete(void* p) { std::free(p); } D() {} };
static D* C() { return new D; }
static bool Check() { D* d = C(); bool ok = d && g_c == 1; delete d; return ok; }

int main()
{
    if (!Check()) { std::printf("00c56cd0_TEST FAIL\n"); return 1; }
    std::printf("00c56cd0_TEST PASS\n");
    return 0;
}