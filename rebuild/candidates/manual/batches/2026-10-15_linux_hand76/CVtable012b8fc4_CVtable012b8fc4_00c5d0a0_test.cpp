#include <cstdio>
#include <cstring>
#include <new>
struct SZeroed_012b8fc4 {
    long m_4, m_8, m_c, m_10, m_14, m_18, m_1c;
    SZeroed_012b8fc4() : m_4(0), m_8(0), m_c(0), m_10(0), m_14(0), m_18(0), m_1c(0) { }
};
struct CVtable_012b8fc4 {
    virtual long v00() { return 7; }
    SZeroed_012b8fc4 m_data;
    CVtable_012b8fc4() { }
};
int main()
{
    static unsigned char buf[sizeof(CVtable_012b8fc4)];
    std::memset(buf, 0xCD, sizeof(buf));
    CVtable_012b8fc4* o = new (buf) CVtable_012b8fc4();
    bool ok = o->v00() == 7 && o->m_data.m_4 == 0 && o->m_data.m_10 == 0 && o->m_data.m_1c == 0 && o->m_data.m_c == 0;
    if (!ok) { std::printf("00c5d0a0_TEST FAIL\n"); return 1; }
    std::printf("00c5d0a0_TEST PASS\n");
    return 0;
}
