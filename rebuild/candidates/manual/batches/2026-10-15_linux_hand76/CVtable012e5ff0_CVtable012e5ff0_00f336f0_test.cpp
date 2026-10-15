#include <cstdio>
#include <cstring>
#include <new>
static int g_baseCalls = 0;
struct CBase_0042d9e0 {
    virtual void v00() { }
    unsigned char _base[0x28];
    CBase_0042d9e0() { ++g_baseCalls; }
};
struct SZeroed_012e5ff0 {
    long m_2c, m_30, m_34, m_38, m_3c, m_40, m_44, m_48, m_4c, m_50, m_54, m_58;
    SZeroed_012e5ff0() : m_2c(0), m_30(0), m_34(0), m_38(0), m_3c(0), m_40(0), m_44(0), m_48(0), m_4c(0), m_50(0), m_54(0), m_58(0) { }
};
struct CVtable_012e5ff0 : CBase_0042d9e0 {
    SZeroed_012e5ff0 m_data;
    virtual void v00() { }
    CVtable_012e5ff0() : CBase_0042d9e0(), m_data() { }
};
int main()
{
    static unsigned char buf[sizeof(CVtable_012e5ff0)];
    std::memset(buf, 0xCD, sizeof(buf));
    CVtable_012e5ff0* d = new (buf) CVtable_012e5ff0();
    bool ok = g_baseCalls == 1 && d->m_data.m_2c == 0 && d->m_data.m_40 == 0 && d->m_data.m_58 == 0 && d->m_data.m_4c == 0;
    if (!ok) { std::printf("00f336f0_TEST FAIL\n"); return 1; }
    std::printf("00f336f0_TEST PASS\n");
    return 0;
}
