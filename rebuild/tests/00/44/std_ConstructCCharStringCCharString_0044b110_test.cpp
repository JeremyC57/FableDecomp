#include <stdio.h>
inline void* operator new(unsigned int, void* where) throw() { return where; }
static int g_copies; static const void* g_src; static const void* g_dst;
class CCharString { public:
    void* m_pData;
    CCharString(void* d) : m_pData(d) {}
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_copies; g_src = &other; g_dst = this; }
};
namespace std {
template <class T1, class T2> void __fastcall _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}
}
int main()
{
    CCharString src((void*)0x1234);
    unsigned char raw[sizeof(CCharString)] = { 0 };
    std::_Construct((CCharString*)raw, src);
    bool ok = g_copies == 1 && g_src == &src && g_dst == raw && ((CCharString*)raw)->m_pData == (void*)0x1234;
    std::_Construct((CCharString*)0, src);
    ok = ok && g_copies == 1;
    printf(ok ? "0044b110_TEST PASS\n" : "0044b110_TEST FAIL\n");
    return ok ? 0 : 1;
}