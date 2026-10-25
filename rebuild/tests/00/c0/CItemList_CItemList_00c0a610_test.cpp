#include <cstdio>
struct CListImpl { long m_mode; CListImpl(long mode) : m_mode(mode) {} ~CListImpl() {} };
struct CTypedListBase { virtual void Slot0() {} CListImpl* m_pImpl; CTypedListBase(CListImpl* impl) : m_pImpl(impl) {} ~CTypedListBase() {} };
struct CItemList : CTypedListBase { CListImpl m_impl; virtual void Slot0() {} CItemList(long mode) : CTypedListBase(&m_impl), m_impl(mode) {} };
int main()
{
    CItemList x(1); if (x.m_pImpl != &x.m_impl || x.m_impl.m_mode != 1) { std::printf("00c0a610_TEST FAIL\n"); return 1; }

    std::printf("00c0a610_TEST PASS\n");
    return 0;
}
