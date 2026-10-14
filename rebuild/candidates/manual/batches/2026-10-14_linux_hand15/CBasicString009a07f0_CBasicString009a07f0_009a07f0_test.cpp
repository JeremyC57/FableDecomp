#include <stdio.h>
#include <stddef.h>
static int g_calls;
#pragma pack(push, 1)
struct CBasicString_char {
    char* PData; unsigned long StringLength; unsigned long DataLength : 31; unsigned long m_reserved : 1; unsigned char UseFastExtend : 1;
    void AssignFromData(const char* text, unsigned long length) { ++g_calls; PData = (char*)text; StringLength = length; }
    CBasicString_char(const char* text, unsigned long length) : PData(0), StringLength(0), DataLength(0), UseFastExtend(1) { AssignFromData(text, length); }
};
struct CBasicString_009a07f0 : CBasicString_char {
    long m_mode;
    CBasicString_009a07f0(const char* text, unsigned long length);
};
#pragma pack(pop)
CBasicString_009a07f0::CBasicString_009a07f0(const char* text, unsigned long length) : CBasicString_char(text, length) { m_mode = 1; }
int main()
{
    CBasicString_009a07f0 s("abc", 3);
    bool ok = g_calls == 1 && s.m_mode == 1 && s.StringLength == 3 && s.UseFastExtend == 1 && offsetof(CBasicString_009a07f0, m_mode) == 0x0d;
    if (!ok) { printf("009a07f0_TEST FAIL\n"); return 1; }
    printf("009a07f0_TEST PASS\n"); return 0;
}
