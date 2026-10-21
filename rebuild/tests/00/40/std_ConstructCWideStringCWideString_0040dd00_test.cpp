#include <stdio.h>
inline void* operator new(unsigned int, void* where) throw() { return where; }
static int g_copies; static const void* g_src; static const void* g_dst;
struct CWideString {
    void* m_pData;
    CWideString(void* d) : m_pData(d) {}
    CWideString(const CWideString& other) : m_pData(other.m_pData) { ++g_copies; g_src = &other; g_dst = this; }
};
namespace std {
template <class T1, class T2> void __fastcall _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}
}
int main()
{
    CWideString src((void*)0x1234);
    unsigned char raw[sizeof(CWideString)] = { 0 };
    std::_Construct((CWideString*)raw, src);
    bool ok = g_copies == 1 && g_src == &src && g_dst == raw && ((CWideString*)raw)->m_pData == (void*)0x1234;
    std::_Construct((CWideString*)0, src);
    ok = ok && g_copies == 1;
    printf(ok ? "0040dd00_TEST PASS\n" : "0040dd00_TEST FAIL\n");
    return ok ? 0 : 1;
}