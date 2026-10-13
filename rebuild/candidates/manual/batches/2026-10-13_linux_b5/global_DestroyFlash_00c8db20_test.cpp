#include <cstdio>
#include <new>
static int g_d = 0; struct CFlash { virtual ~CFlash() { ++g_d; } }; struct A {};
static void __fastcall Destroy(A&, CFlash* p) { p->CFlash::~CFlash(); }
int main()
{
    A al; char buf[sizeof(CFlash)]; CFlash* p = new (buf) CFlash; Destroy(al, p);
    if (g_d != 1) { std::printf("00c8db20_TEST FAIL\n"); return 1; }

    std::printf("00c8db20_TEST PASS\n");
    return 0;
}
