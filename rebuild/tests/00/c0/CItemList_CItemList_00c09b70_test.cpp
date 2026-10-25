#include <cstdio>
static int g_d = 0;
struct CListImpl { void* m_data; ~CListImpl() { ++g_d; } };
struct CTypedListBase { virtual void Slot0() {} ~CTypedListBase() {} };
struct CItemList : CTypedListBase { CListImpl* m_pImpl; CListImpl m_impl; virtual void Slot0() {} ~CItemList() {} };
int main()
{
    { CItemList d; } if (g_d != 1) { std::printf("00c09b70_TEST FAIL\n"); return 1; }

    std::printf("00c09b70_TEST PASS\n");
    return 0;
}
