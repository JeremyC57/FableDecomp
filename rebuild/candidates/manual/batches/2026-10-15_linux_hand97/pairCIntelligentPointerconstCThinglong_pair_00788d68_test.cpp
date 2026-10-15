// Behaviour test for the converting pair constructor @ 0x00788D68: pointer copied, float truncated toward zero.
#include <cstdio>
struct CThing { long id; };
template<class T> struct CIntelligentPointer {
    T* m_pItem;
    CIntelligentPointer(T* p) : m_pItem(p) {}
    CIntelligentPointer(const CIntelligentPointer& other) : m_pItem(other.m_pItem) {}
};
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    template<class U1, class U2> pair(const pair<U1, U2>& p) : first(p.first), second((T2)p.second) {}
};
int main()
{
    CThing t; t.id = 1;
    pair<CIntelligentPointer<const CThing>, float> src(CIntelligentPointer<const CThing>(&t), 7.9f);
    pair<CIntelligentPointer<const CThing>, long> dst(src);
    pair<CIntelligentPointer<const CThing>, float> neg(CIntelligentPointer<const CThing>(&t), -2.5f);
    pair<CIntelligentPointer<const CThing>, long> dst2(neg);
    if (dst.first.m_pItem != &t || dst.second != 7 || dst2.second != -2) { std::printf("00788d68_TEST FAIL\n"); return 1; }
    std::printf("00788d68_TEST PASS\n");
    return 0;
}
