// behaviour test for 0x00996F00 CDataOutputStream's scalar deleting destructor (model: member first, then the bases)
#include <cstdio>
static int g_order = 0, g_member = 0, g_base = 0;
struct CMember { ~CMember() { g_member = ++g_order; } };
struct CBase { virtual ~CBase() { g_base = ++g_order; } };
struct CDerived : CBase { CMember m; };
int main()
{
    CBase* p = new CDerived;
    delete p;
    int ok = g_member == 1 && g_base == 2;
    if (!ok) { std::printf("00996f00_TEST FAIL\n"); return 1; }
    std::printf("00996f00_TEST PASS\n");
    return 0;
}