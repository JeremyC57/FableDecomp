#include <cstdio>
static int g_b = 0; struct Base { virtual void v00() {} Base() { ++g_b; } };
struct D : Base { unsigned char pad[0x3c]; void* s; void* f; virtual void v00() {} D(void* a, void* b) : Base(), s(b), f(a) {} };
static D* __fastcall Alloc(void* a, void* b) { return new D(a, b); }
int main()
{
    int x, y; D* d = Alloc(&x, &y); if (!d || g_b != 1 || d->s != &y || d->f != &x) { std::printf("00efb610_TEST FAIL\n"); return 1; }
    delete d;

    std::printf("00efb610_TEST PASS\n");
    return 0;
}
