// behaviour test for 0x009E2890 CVertexBufferWin32's scalar deleting destructor (model: cleanup runs before the base destructor, delete frees)
#include <cstdio>
static int g_order = 0, g_clean = 0, g_base = 0;
struct CBase { virtual ~CBase() { g_base = ++g_order; } };
struct CDerived : CBase { virtual ~CDerived() { Cleanup(); } void Cleanup() { g_clean = ++g_order; } };
int main()
{
    CBase* p = new CDerived;
    delete p;
    int ok = g_clean == 1 && g_base == 2;
    if (!ok) { std::printf("009e2890_TEST FAIL\n"); return 1; }
    std::printf("009e2890_TEST PASS\n");
    return 0;
}