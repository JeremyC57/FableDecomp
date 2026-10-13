#include <cstdio>
struct Ctx; static void __fastcall Helper(Ctx*, long v, long* r) { *r = v + 1; }
struct X { virtual void v00() {} unsigned char* p; virtual void T(Ctx* c, long v) { unsigned char* t = p; Helper(c, v, &v); *t = (unsigned char)v; } };
int main()
{
    unsigned char out = 0; X x; x.p = &out; x.T(0, 4); if (out != 5) { std::printf("00e9e3e0_TEST FAIL\n"); return 1; }

    std::printf("00e9e3e0_TEST PASS\n");
    return 0;
}
