// behaviour test for 0x006FBCC0: every record in the range is destroyed once
#include <cstdio>
#include <new>
static int g_destroyed = 0;
struct R { int a; ~R() { ++g_destroyed; } };
static void DestroyAux(R* f, R* l) { for (; f != l; ++f) f->~R(); }
int main()
{
    void* buf = ::operator new(sizeof(R) * 3); R* r = (R*)buf; for (int i = 0; i < 3; ++i) new (r + i) R();
    DestroyAux(r, r + 3); DestroyAux(r, r);
    ::operator delete(buf);
    if (g_destroyed != 3) { std::printf("006fbcc0_TEST FAIL\n"); return 1; }
    std::printf("006fbcc0_TEST PASS\n");
    return 0;
}