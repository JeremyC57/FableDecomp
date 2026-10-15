#include <cstdio>
#include <cstring>
// Behaviour of the record constructor at 0x005271C0: six arguments land in their members (two strings copied).
static int g_copies = 0;
struct CCharString { void* data; CCharString(void* d) : data(d) {} CCharString(const CCharString& other) : data(other.data) { ++g_copies; } };
struct SFilterRecord_Retail {
    bool m_flag0; long m_value4; bool m_flag8; CCharString m_name0C; long m_value10; CCharString m_name14;
    SFilterRecord_Retail(bool flag0, const CCharString& name14, long value10, long value4, bool flag8, const CCharString& name0C)
        : m_flag0(flag0), m_value4(value4), m_flag8(flag8), m_name0C(name0C), m_value10(value10), m_name14(name14) {}
};
int main()
{
    int a = 1, b = 2; CCharString s14(&a), s0c(&b);
    SFilterRecord_Retail r(true, s14, 30, 40, false, s0c);
    if (!r.m_flag0 || r.m_value4 != 40 || r.m_flag8 || r.m_name0C.data != &b || r.m_value10 != 30 || r.m_name14.data != &a || g_copies != 2) { std::printf("005271c0_TEST FAIL\n"); return 1; }
    std::printf("005271c0_TEST PASS\n"); return 0;
}
