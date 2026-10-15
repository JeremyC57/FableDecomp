#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
#include <string.h>
#include <new>
struct CVtable_012b5860 { virtual void v00() { } long m_body[3]; CVtable_012b5860() { m_body[0] = m_body[1] = 7; m_body[2] = 0; } };
struct SZeroRecord { long m_words[7]; SZeroRecord() { memset(this, 0, sizeof(*this)); } };
struct CVtable_012b4f74 : CVtable_012b5860 {
    SZeroRecord m_record;
    long        m_value;
    virtual void v00() { }
    CVtable_012b4f74() : m_value(0) { }
};
int main()
{
    unsigned char raw[sizeof(CVtable_012b4f74)]; memset(raw, 0xcd, sizeof(raw));
    CVtable_012b4f74* p = new (raw) CVtable_012b4f74;
    for (int i = 0; i < 7; ++i) if (p->m_record.m_words[i] != 0) { std::printf("00c3c3b0_TEST FAIL\n"); return 1; }
    if (p->m_value != 0 || p->m_body[0] != 7) { std::printf("00c3c3b0_TEST FAIL\n"); return 1; }
    std::printf("00c3c3b0_TEST PASS\n");
    return 0;
}
