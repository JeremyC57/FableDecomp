#include <cstdio>
#include <stdio.h>
#ifndef _MSC_VER
#define _snprintf snprintf
#endif
struct SKey16 { unsigned long m_words[4]; };
struct CRigidBodyFileRecord_0081e4f0 {
    void*         m_vtable;
    unsigned char m_flag;
    SKey16        m_key;
    char          m_name[0x20];
    long          m_valueA, m_valueB, m_count0, m_count1, m_count2;
    void CopyFrom(const CRigidBodyFileRecord_0081e4f0& other)
    {
        m_key = other.m_key;
        _snprintf(m_name, sizeof(m_name), "%s", other.m_name);
        m_name[sizeof(m_name) - 1] = 0;
        m_valueA = other.m_valueA;
        m_valueB = other.m_valueB;
        m_count0 = 0;
        m_count1 = 0;
        m_count2 = 0;
    }
};
int main()
{
    CRigidBodyFileRecord_0081e4f0 a, b;
    int i;
    for (i = 0; i < 4; ++i) { a.m_key.m_words[i] = 0; b.m_key.m_words[i] = 100 + i; }
    a.m_flag = 7; b.m_flag = 1; a.m_vtable = 0; b.m_vtable = 0;
    for (i = 0; i < 0x20; ++i) a.m_name[i] = 'x';
    for (i = 0; i < 0x1f; ++i) b.m_name[i] = (char)('a' + (i % 26));
    b.m_name[0x1f] = 0;
    b.m_valueA = 11; b.m_valueB = -12; a.m_count0 = a.m_count1 = a.m_count2 = 5;
    a.CopyFrom(b);
    int bad = 0;
    for (i = 0; i < 4; ++i) if (a.m_key.m_words[i] != (unsigned long)(100 + i)) bad = 1;
    for (i = 0; i < 0x1f; ++i) if (a.m_name[i] != b.m_name[i]) bad = 1;
    if (a.m_name[0x1f] != 0 || a.m_flag != 7 || a.m_valueA != 11 || a.m_valueB != -12 || a.m_count0 || a.m_count1 || a.m_count2) bad = 1;
    // a short name is copied whole
    b.m_name[0] = 'Q'; b.m_name[1] = 0;
    a.CopyFrom(b);
    if (a.m_name[0] != 'Q' || a.m_name[1] != 0) bad = 1;
    if (bad) { std::printf("0081e4f0_TEST FAIL\n"); return 1; }
    std::printf("0081e4f0_TEST PASS\n");
    return 0;
}