#include <cstdio>
static int g_m = 0; struct CItem { virtual void v00() {} CItem() {} ~CItem() {} }; struct M { void* d; M() : d(0) { ++g_m; } ~M() {} };
struct CHashedItem : CItem { unsigned long m_hash; M m; virtual void v00() {} CHashedItem(unsigned long h) : CItem(), m_hash(h), m() {} };
int main()
{
    CHashedItem x(0xBEEFu); if (x.m_hash != 0xBEEFu || x.m.d != 0 || g_m != 1) { std::printf("00c1f640_TEST FAIL\n"); return 1; }

    std::printf("00c1f640_TEST PASS\n");
    return 0;
}
